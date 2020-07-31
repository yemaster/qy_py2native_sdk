// event.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "resource.h"
#define USE_H
#include "get_source_code.hpp"
#include "include/Python.h"
#pragma comment(lib,"libs/python3")
#pragma comment(lib,"libs/python38")

#include <sstream>
#include <thread>

#ifndef QYEVENT
#define QYEVENT(TYPE) extern "C" __declspec(dllexport) TYPE __stdcall 
#endif

PyObject *py_obj_handler = nullptr;
PyObject *module_dict = nullptr;

//给Python文件调用，传入py_obj_handler
PyObject *cpp_main(PyObject *self, PyObject *args);
//包装添加日志
PyObject *qy_log(PyObject *self, PyObject *args);
PyObject *qy_warning(PyObject *self, PyObject *args);
PyObject *qy_error(PyObject *self, PyObject *args);
//包装发送消息的函数
PyObject *qy_sendFriendMsg(PyObject *self, PyObject *args);

//包装编码转换
PyObject *qy_utf8Decode(PyObject *self, PyObject *args);

/*
 * 返回插件基础信息
 * 本函数【禁止】处理其他任何代码
 */
QYEVENT(CSTRING) AppInfo()
{
	return QY::getJsonInfo();
}


/*
 * 接收应用AuthCode
 * 本函数【禁止】处理其他任何代码，请不要改动本函数
 */
QYEVENT(INT32) Initialize(
	INT32	AuthCode,		// 应用标识
	INT32	ProtocolType	// 协议类型 1 / 安卓QQ协议 2 / PCQQ协议
)
{
	QY::setAuthCode(AuthCode);
	QY::setProtocolType(ProtocolType);
	
	return 0;
}

/*
 * [1001]主程序启动
 * 本子程序会在【主线程】中被调用。
 * 无论本应用是否被启用，本函数都会在主程序启动后执行一次，请在这里执行插件初始化代码。
 * 请务必尽快返回本子程序，否则会卡住其他插件以及主程序的加载。
 * 这里禁止直接载入窗口
 */
QYEVENT(INT32) QY_Event_Startup()
{
	PyObject *obj_module = nullptr;
	PyObject *global = PyModule_GetDict(obj_module);

	PyMethodDef method_cpp_main = { "cpp_main",cpp_main,METH_VARARGS,"Execute Cpp Main" };
	PyMethodDef method_cpp_log = { "cpp_log",qy_log,METH_VARARGS,"Add a info log" };
	PyMethodDef method_cpp_warning = { "cpp_warning",qy_warning,METH_VARARGS,"Add a warning log" };
	PyMethodDef method_cpp_error = { "cpp_error",qy_error,METH_VARARGS,"Add a error log" };
	PyMethodDef method_cpp_sendFriendMsg = { "cpp_sendFriendMsg",qy_sendFriendMsg,METH_VARARGS,"Send a friend message" };
	PyMethodDef method_cpp_utf8Decode = { "cpp_utf8Decode",qy_utf8Decode,METH_VARARGS,"Utf8Decode to ansi" };

	//模块方法列表
	PyMethodDef methods[] = {
		method_cpp_main,
		method_cpp_log,
		method_cpp_warning,
		method_cpp_error,
		method_cpp_sendFriendMsg,
		method_cpp_utf8Decode
	};
	//模块定义
	PyModuleDef module_def =
	{
		PyModuleDef_HEAD_INIT,
		"qysdk_base",
		NULL,
		-1,
		methods
	};
	//创建模块
	obj_module = PyModule_Create(&module_def);
	module_dict = PyModule_GetDict(obj_module);
	PyState_AddModule(obj_module, &module_def);

	string python_code = pysdk::read_resource(IDR_PYMAIN);
	if (PyRun_SimpleStringFlags(python_code.c_str(), NULL) == -1) {
		using sstream = std::stringstream;
		sstream error;
		error << "脚本执行错误!\r\n" << std::endl;
		std::thread([&]() {
			AllocConsole();
			PyErr_Print();
			system("pause");
			HWND console = GetConsoleWindow();
			CloseWindow(console);
		}).detach();
		QY::setAppSelfDiscontinue();
	}
	return 0; //请固定返回0。
}

/*
 * [1002]主程序退出
 * 本子程序会在【主线程】中被调用。
 * 无论本应用是否被启用或卸载，本函数都会在退出前执行一次，请在这里执行插件关闭代码。
 */
QYEVENT(INT32) QY_Event_Exit()
{
	return 0; //请固定返回0。
}

/*
 * [1003]应用被启用
 * 本子程序会在【主线程】中被调用。
 * 当应用被启用后，将收到此事件。
 * 如果载入时应用已被启用，则在_eventStartup(Type=1001,启动)被调用后，本函数也将被调用一次。
 * 如非必要，不建议在这里加载窗口。（可以添加菜单，让用户手动打开窗口）
 */
QYEVENT(INT32) QY_Event_Enable()
{

	QY::setAppEnable(true);
	if (py_obj_handler) {
		PyObject *onEnable = PyObject_GetAttrString(py_obj_handler,"onEnable");
		PyObject_Call(onEnable,PyTuple_New(0) , NULL);
	}
	return 0; //请固定返回0。
}

/*
 * [1004]应用被停用
 * 当应用被停用前，将收到此事件。
 * 如果载入时应用已被停用或卸载，则本函数【不会】被调用。
 * 无论本应用是否被启用，主程序关闭前本函数都【不会】被调用。
 */
QYEVENT(INT32) QY_Event_Disable()
{
	QY::setAppEnable(false);
	if (py_obj_handler) {
		PyObject *onDisable = PyObject_GetAttrString(py_obj_handler, "onDisable");
		PyObject_Call(onDisable, PyTuple_New(0), NULL);
	}
	return 0; //请固定返回0。
}

/*
 * [166]私聊消息
 * 本子程序会在【线程】中被调用，请注意使用对象等需要初始化(CoInitialize,CoUninitialize)。
 */
QYEVENT(INT32) QY_Event_PrivateMsg(
	INT64		robotID,		// 机器人QQ，用于区分多号登录
	INT32		subType,		// 消息渠道，11/来自好友 1/来自在线状态 2/来自群 3/来自讨论组 4/来自公众号
	INT64		sendTime,		// 发送时间(13位时间戳)
	INT64		fromQQ,			// 来源对象
	INT64		fromGroup,		// 来源群组(仅子类型为2/3时使用)
	CSTRING		fromInfo,		// 来源者信息
	CSTRING		msg,			// 消息内容
	CSTRING		info,			// 信息
	INT32		intTest			// 用于测试(我也不知道机器人作者想干嘛
)
{
	if (py_obj_handler) {
		PyObject *onPrivateMsg = PyObject_GetAttrString(py_obj_handler, "onPrivateMsg");
		
		PyObject *args = Py_BuildValue("(LiLLLsssi)",
			robotID,
			subType,
			sendTime,
			fromQQ,
			fromGroup,
			QY::convertAnsiToUtf8(fromInfo).cStr(),
			QY::convertAnsiToUtf8(msg).cStr(),
			QY::convertAnsiToUtf8(info).cStr(),
			intTest);
		
		PyObject_Call(onPrivateMsg, args, nullptr);
	}
	// 如果要回复消息，请调用API发送，并且这里 return QY::EVENT_BLOCK - 截断本条消息，不再继续处理
	// 注意：应用优先级设置为"最高"(10000)时，不得使用本返回值
	// 如果不回复消息，交由之后的应用/过滤器处理，这里 return QY::EVENT_IGNORE - 忽略本条消息
	return QY::EVENT_IGNORE;
}

/*
 * [82]群消息
 * 本子程序会在【线程】中被调用，请注意使用对象等需要初始化(CoInitialize,CoUninitialize)。
 */
QYEVENT(INT32) QY_Event_GroupMsg(
	INT64		robotID,		// 机器人QQ，用于区分多号登录
	INT32		subType,		// 子类型，目前固定为1
	INT64		sendTime,		// 发送时间(13位时间戳)
	INT64		fromGroup,		// 来源群号
	INT64		fromQQ,			// 来源QQ号
	CSTRING		fromInfo,		// 来源者信息
	CSTRING		msg,			// 消息内容
	CSTRING		info,			// 事件总信息
	INT32		intTest			// 用于测试
)
{
	PyObject *onPrivateMsg = PyObject_GetAttrString(py_obj_handler, "onPrivateMsg");

	PyObject *args = Py_BuildValue("(LiLLLsssi)",
		robotID,
		subType,
		sendTime,
		fromGroup,
		fromQQ,
		QY::convertAnsiToUtf8(fromInfo).cStr(),
		QY::convertAnsiToUtf8(msg).cStr(),
		QY::convertAnsiToUtf8(info).cStr(),
		intTest);

	PyObject_Call(onPrivateMsg, args, nullptr);
	return QYAPI::EVENT_IGNORE; //关于返回值说明, 见「QY_Event_PrivateMsg」函数
}

/*
 * [83]讨论组消息
 * 本子程序会在【线程】中被调用，请注意使用对象等需要初始化(CoInitialize,CoUninitialize)。
 */
QYEVENT(INT32) QY_Event_DiscussMsg(
	INT64		robotID,		// 机器人QQ，用于区分多号登录
	INT32		subType,		// 子类型，目前固定为1
	INT64		sendTime,		// 发送时间(13位时间戳)
	INT64		fromDiscuss,	// 来源讨论组
	INT64		fromQQ,			// 来源QQ号
	CSTRING		fromInfo,		// 来源者信息
	CSTRING		msg,			// 消息内容
	CSTRING		info,			// 事件总信息
	INT32		intTest			// 用于测试
)
{
	return QY::EVENT_IGNORE; //关于返回值说明, 见「QY_Event_PrivateMsg」函数
}

/*
 * [101]推送消息
 * 本子程序会在【线程】中被调用，请注意使用对象等需要初始化(CoInitialize,CoUninitialize)。
 */
QYEVENT(INT32) QY_Event_PushMsgEvent(
	INT64		robotID,		// 机器人QQ，用于区分多号登录
	INT32		subType,		// 子类型，目前固定为1
	INT64		sendTime,		// 发送时间(13位时间戳)
	INT64		fromGroup,		// 来源群号
	INT64		fromQQ,			// 操作对象
	INT64		param,			// 操作参数，部分情况下会是QQ号
	CSTRING		msg,			// 事件信息
	CSTRING		info,			// 事件总信息
	INT32		intTest			// 用于测试
)
{
	
	return QY::EVENT_IGNORE; //关于返回值说明, 见「QY_Event_PrivateMsg」函数
}

/*
 * [102]群事件-管理员变动
 * 本子程序会在【线程】中被调用，请注意使用对象等需要初始化(CoInitialize,CoUninitialize)。
 */
QYEVENT(INT32) QY_Event_System_GroupAdmin(
	INT64	robotID,		// 机器人QQ，用于区分多号登录
	INT32	subType,		// 子类型，1/被取消管理员 2/被设置管理员
	INT64	sendTime,		// 发送时间(13位时间戳)
	INT64	fromGroup,		// 来源群组
	INT64	beingOperateQQ	// 被操作对象
)
{
	return QY::EVENT_IGNORE; //关于返回值说明, 见「QY_Event_PrivateMsg」函数
}

/*
 * [103]群事件-群成员减少
 * 本子程序会在【线程】中被调用，请注意使用对象等需要初始化(CoInitialize,CoUninitialize)。
 */
QYEVENT(INT32) QY_Event_System_GroupMemberDecrease(
	INT64	robotID,		// 机器人QQ，用于区分多号
	INT32	subType,		// 子类型，1/群解散 2/群员离开 3/群员被踢。
	INT64	sendTime,		// 发送时间(13位时间戳)
	INT64	fromGroup,		// 来源群组
	INT64	fromQQ,			// 操作对象(仅子类型为2、3时存在)
	INT64	beingOperateQQ	// 被操作对象
)
{
	return QY::EVENT_IGNORE; //关于返回值说明, 见「QY_Event_PrivateMsg」函数
}

/*
 * [104]群事件-群成员增加
 * 本子程序会在【线程】中被调用，请注意使用对象等需要初始化(CoInitialize,CoUninitialize)。
 */
QYEVENT(INT32) QY_Event_System_GroupMemberIncrease(
	INT64	robotID,		// 机器人QQ，用于区分多号
	INT32	subType,		// 子类型，1/管理员已同意 2/管理员邀请。
	INT64	sendTime,		// 发送时间(13位时间戳)
	INT64	fromGroup,		// 来源群组
	INT64	fromQQ,			// 操作对象(仅子类型为2、3时存在)
	INT64	beingOperateQQ	// 被操作对象
)
{
	return QY::EVENT_IGNORE; //关于返回值说明, 见「QY_Event_PrivateMsg」函数
}

/*
 * [201]添加他人为好友结果事件
 * 本子程序会在【线程】中被调用，请注意使用对象等需要初始化(CoInitialize,CoUninitialize)。
 */
QYEVENT(INT32) QY_Event_FriendEvent(
	INT64		robotID,		// 机器人QQ，用于区分多号
	INT32		subType,		// 子类型，1/对方同意添加 2/对方拒绝添加
	INT64		sendTime,		// 发送时间(13位时间戳)
	INT64		fromQQ,			// 来源对象
	CSTRING		info			// 信息
)
{
	return QY::EVENT_IGNORE; //关于返回值说明, 见「QY_Event_PrivateMsg」函数
}

/*
 * [301]请求-他人添加好友
 * 本子程序会在【线程】中被调用，请注意使用对象等需要初始化(CoInitialize,CoUninitialize)。
 */
QYEVENT(INT32) QY_Event_Request_AddFriend(
	INT64		robotID,		// 机器人QQ，用于区分多号
	INT32		subType,		// 子类型，目前固定为1
	INT64		sendTime,		// 请求时间(13位时间戳)
	INT64		fromQQ,			// 来源对象
	CSTRING		source,			// 来源信息
	CSTRING		msg,			// 附言
	CSTRING		responseFlag	// 反馈标识(处理请求用)
)
{

	return QY::EVENT_IGNORE; //关于返回值说明, 见「QY_Event_PrivateMsg」函数
}

/*
 * [302]请求-群添加
 * 本子程序会在【线程】中被调用，请注意使用对象等需要初始化(CoInitialize,CoUninitialize)。
 */
QYEVENT(INT32) QY_Event_Request_AddGroup(
	INT64		robotID,		// 机器人QQ，用于区分多号
	INT32		responseType,	// 请求类型，1/他人申请入群 2/群成员邀请他人入群 3/自己(即登录号)受邀入群
	INT64		sendTime,		// 请求时间(13位时间戳)
	INT64		fromGroup,		// 来源群组
	INT64		fromQQ,			// 来源对象
	CSTRING		source,			// 来源信息，请求类型为1时是来源渠道、为2或3时是邀请人QQ
	CSTRING		msg,			// 附言
	CSTRING		responseFlag	// 反馈标识(处理请求用)
)
{

	return QY::EVENT_IGNORE; //关于返回值说明, 见「QY_Event_PrivateMsg」函数
}

/*
 * 菜单函数，可在 info.json 资源文件中设置菜单数目、函数名
 * 如果不使用菜单，请在「 info.json、events.def 以及 此处 」删除无用函数
 */
QYEVENT(INT32) _menuA()
{
	if (py_obj_handler) {
		PyObject *onEnable = PyObject_GetAttrString(py_obj_handler, "onMenuA");
		PyObject_Call(onEnable, PyTuple_New(0), NULL);
	}
	return 0; //请固定返回0
}

QYEVENT(INT32) _menuB()
{
	if (py_obj_handler) {
		PyObject *onEnable = PyObject_GetAttrString(py_obj_handler, "onMenuB");
		PyObject_Call(onEnable, PyTuple_New(0), NULL);
	}
	return 0; //请固定返回0
}

QYEVENT(INT32) _menuC()
{
	if (py_obj_handler) {
		PyObject *onEnable = PyObject_GetAttrString(py_obj_handler, "onMenuC");
		PyObject_Call(onEnable, PyTuple_New(0), NULL);
	}
	return 0; //请固定返回0
}

#undef QYEVENT



PyObject *cpp_main(PyObject *self, PyObject *args) {
	PyArg_ParseTuple(args, "O", py_obj_handler);
	PyObject *isHandler;
	isHandler = PyObject_GetAttrString(py_obj_handler, "O");
	if (!PyObject_IsTrue(isHandler)) {
		QYLOG::Fatal(0, "Python", "Handler不正确!");
		QY::setAppSelfDiscontinue();
	}
	return Py_None;
}


PyObject *qy_log(PyObject *self, PyObject *args) {
	const char *event_name,*log_content;
	if (!PyArg_ParseTuple(args, "ss", &event_name, &log_content)) {
		QYLOG::Error(0, "Python", "QY.log调用错误！");
	}
	else {
		QYLOG::Info(0,event_name,log_content);
	}
	return Py_None;
}

PyObject *qy_warning(PyObject *self, PyObject *args) {
	const char *event_name,*log_content;
	if (!PyArg_ParseTuple(args, "ss", &event_name, &log_content)) {
		QYLOG::Error(0, "Python", "QY.warning调用错误！");
	}
	else {
		QYLOG::Warning(0,event_name,log_content);
	}
	return Py_None;
}

PyObject *qy_error(PyObject *self, PyObject *args) {
	const char *event_name, *log_content;
	if (!PyArg_ParseTuple(args, "ss", &event_name, &log_content)) {
		QYLOG::Error(0, "Python", "QY.error调用错误！");
	}
	else {
		QYLOG::Error(0, event_name, log_content);
	}
	return Py_None;
}

PyObject *qy_sendFriendMsg(PyObject *self, PyObject *args) {
	unsigned long long RobotID, DestQQ;
	const char* content;
	if (!PyArg_ParseTuple(args, "KKs", &RobotID, &DestQQ,&content)) {
		QYLOG::Error(0, "Python", "QY.sendFriendMsg调用错误！");
	}
	else {
		QY::sendFriendMsg(RobotID, DestQQ, QY::convertUtf8ToAnsi(content));
	}
	return Py_None;
}

PyObject *qy_utf8Decode(PyObject *self, PyObject *args) {
	const char* content;
	if (!PyArg_ParseTuple(args, "s",  &content)) {
		QYLOG::Error(0, "Python", "QY.utf8Decode调用错误！");
	}
	else {
		const char* ret = QY::convertUtf8ToAnsi(content);
		return Py_BuildValue("s", ret);
	}
	return Py_None;
}