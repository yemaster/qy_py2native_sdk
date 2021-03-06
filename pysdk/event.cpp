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
PyObject* qy_sendFriendMsg(PyObject* self, PyObject* args);
PyObject* qy_sendGroupMsg(PyObject* self, PyObject* args);
PyObject* qy_sendGroupTmpMsg(PyObject* self, PyObject* args);
PyObject* qy_sendDiscussMsg(PyObject* self, PyObject* args);
PyObject* qy_sendDiscussTmpMsg(PyObject* self, PyObject* args);
PyObject* qy_setMessageSvcMsgWithDraw(PyObject* self, PyObject* args);
//包装好友操作
PyObject* qy_sendLikeFavorite(PyObject* self, PyObject* args);
PyObject* qy_setFriendDelete(PyObject* self, PyObject* args);
PyObject* qy_setFriendAddRequest(PyObject* self, PyObject* args);
//包装群聊操作
PyObject* qy_setGroupMembersKick(PyObject* self, PyObject* args);
PyObject* qy_setGroupWholeBanSpeak(PyObject* self, PyObject* args);
PyObject* qy_setGroupMemberBanSpeak(PyObject* self, PyObject* args);
PyObject* qy_setGroupAnonymousBan(PyObject* self, PyObject* args);
PyObject* qy_setGroupPrivateSession(PyObject* self, PyObject* args);
PyObject* qy_setGroupManyPeopleChat(PyObject* self, PyObject* args);
PyObject* qy_setGroupAdmini(PyObject* self, PyObject* args);
PyObject* qy_setGroupMemberCard(PyObject* self, PyObject* args);
PyObject* qy_setGroupSpecialTitle(PyObject* self, PyObject* args);
PyObject* qy_setGroupExit(PyObject* self, PyObject* args);
PyObject* qy_setGroupAdd(PyObject* self, PyObject* args);
PyObject* qy_setGroupAddRequest(PyObject* self, PyObject* args);
//包装文件操作
PyObject* qy_sendOfflineFile(PyObject* self, PyObject* args);
PyObject* qy_setGroupFileUpload(PyObject* self, PyObject* args);
PyObject* qy_setGroupFileDelete(PyObject* self, PyObject* args);
//包装讨论组操作
PyObject* qy_setDiscussExit(PyObject* self, PyObject* args);
PyObject* qy_setDiscussMembersKick(PyObject* self, PyObject* args);
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
	PyMethodDef method_cpp_sendGroupMsg = { "cpp_sendGroupMsg",qy_sendGroupMsg,METH_VARARGS,"Send a group message" };
	PyMethodDef method_cpp_sendGroupTmpMsg = { "cpp_sendGroupTmpMsg",qy_sendGroupTmpMsg,METH_VARARGS,"Send a group tmp message" };
	PyMethodDef method_cpp_sendDiscussMsg = { "cpp_sendDiscussMsg",qy_sendDiscussMsg,METH_VARARGS,"Send a discuss message" };
	PyMethodDef method_cpp_sendDiscussTmpMsg = { "cpp_sendDiscussTmpMsg",qy_sendDiscussTmpMsg,METH_VARARGS,"Send a discuss tmp message" };
	PyMethodDef method_cpp_setMessageSvcMsgWithDraw = { "cpp_setMessageSvcMsgWithDraw",qy_setMessageSvcMsgWithDraw,METH_VARARGS,"Delete a message" };
	PyMethodDef method_cpp_sendLikeFavorite = { "cpp_sendLikeFavorite",qy_sendLikeFavorite,METH_VARARGS,"Send somebody Likes" };
	PyMethodDef method_cpp_setFriendDelete = { "cpp_setFriendDelete",qy_setFriendDelete,METH_VARARGS,"Delete a Friend" };
	PyMethodDef method_cpp_setFriendAddRequest = { "cpp_setFriendAddRequest",qy_setFriendAddRequest,METH_VARARGS,"Add a friend" };
	PyMethodDef method_cpp_setGroupMembersKick = { "cpp_setGroupMembersKick",qy_setGroupMembersKick,METH_VARARGS,"Set group members kick" };
	PyMethodDef method_cpp_setGroupWholeBanSpeak = { "cpp_setGroupWholeBanSpeak",qy_setGroupWholeBanSpeak,METH_VARARGS,"Set group whole ban speak" };
	PyMethodDef method_cpp_setGroupMemberBanSpeak = { "cpp_setGroupMemberBanSpeak",qy_setGroupMemberBanSpeak,METH_VARARGS,"Set group member ban speak" };
	PyMethodDef method_cpp_setGroupAnonymousBan = { "cpp_setGroupAnonymousBan",qy_setGroupAnonymousBan,METH_VARARGS,"Set group anonymous ban" };
	PyMethodDef method_cpp_setGroupPrivateSession = { "cpp_setGroupPrivateSession",qy_setGroupPrivateSession,METH_VARARGS,"Set group private session" };
	PyMethodDef method_cpp_setGroupManyPeopleChat = { "cpp_setGroupManyPeopleChat",qy_setGroupManyPeopleChat,METH_VARARGS,"Set group many people chat" };
	PyMethodDef method_cpp_setGroupAdmini = { "cpp_setGroupAdmini",qy_setGroupAdmini,METH_VARARGS,"Set group admini" };
	PyMethodDef method_cpp_setGroupMemberCard = { "cpp_setGroupMemberCard",qy_setGroupMemberCard,METH_VARARGS,"Set a group member's card" };
	PyMethodDef method_cpp_setGroupSpecialTitle = { "cpp_setGroupSpecialTitle",qy_setGroupSpecialTitle,METH_VARARGS,"Set group special title" };
	PyMethodDef method_cpp_setGroupExit = { "cpp_setGroupExit",qy_setGroupExit,METH_VARARGS,"Set Group Exit" };
	PyMethodDef method_cpp_setGroupAdd = { "cpp_setGroupAdd",qy_setGroupAdd,METH_VARARGS,"Set Group ADd" };
	PyMethodDef method_cpp_setGroupAddRequest = { "cpp_setGroupAddRequest",qy_setGroupAddRequest,METH_VARARGS,"Set group add request" };
	PyMethodDef method_cpp_sendOfflineFile = { "cpp_sendOfflineFile",qy_sendOfflineFile,METH_VARARGS,"Send offline file" };
	PyMethodDef method_cpp_setGroupFileUpload = { "cpp_setGroupFileUpload",qy_setGroupFileUpload,METH_VARARGS,"Set Group file upload" };
	PyMethodDef method_cpp_setGroupFileDelete = { "cpp_setGroupFileDelete",qy_setGroupFileDelete,METH_VARARGS,"Set Group file delete" };
	PyMethodDef method_cpp_setDiscussExit = { "cpp_setDiscussExit",qy_setDiscussExit,METH_VARARGS,"Set discuss exit" };
	PyMethodDef method_cpp_setDiscussMembersKick = { "cpp_setDiscussMembersKick",qy_setDiscussMembersKick,METH_VARARGS,"Set discuss member kick" };
	PyMethodDef method_cpp_utf8Decode = { "cpp_utf8Decode",qy_utf8Decode,METH_VARARGS,"Utf8Decode to ansi" };

	//模块方法列表
	PyMethodDef methods[] = {
		method_cpp_main,
		method_cpp_log,
		method_cpp_warning,
		method_cpp_error,
		method_cpp_sendFriendMsg,
		method_cpp_sendGroupMsg,
		method_cpp_sendGroupTmpMsg,
		method_cpp_sendDiscussMsg,
		method_cpp_sendDiscussTmpMsg,
		method_cpp_setMessageSvcMsgWithDraw,
		method_cpp_sendLikeFavorite,
		method_cpp_setFriendDelete,
		method_cpp_setFriendAddRequest,
		method_cpp_setGroupMembersKick,
		method_cpp_setGroupWholeBanSpeak,
		method_cpp_setGroupMemberBanSpeak,
		method_cpp_setGroupAnonymousBan,
		method_cpp_setGroupPrivateSession,
		method_cpp_setGroupManyPeopleChat,
		method_cpp_setGroupAdmini,
		method_cpp_setGroupMemberCard,
		method_cpp_setGroupSpecialTitle,
		method_cpp_setGroupExit,
		method_cpp_setGroupAdd,
		method_cpp_setGroupAddRequest,
		method_cpp_sendOfflineFile,
		method_cpp_setGroupFileUpload,
		method_cpp_setGroupFileDelete,
		method_cpp_setDiscussExit,
		method_cpp_setDiscussMembersKick,
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
	PyObject *onPrivateMsg = PyObject_GetAttrString(py_obj_handler, "onGroupMsg");

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
	PyObject *onPrivateMsg = PyObject_GetAttrString(py_obj_handler, "onDiscussMsg");

	PyObject *args = Py_BuildValue("(LiLLLsssi)",
		robotID,
		subType,
		sendTime,
		fromDiscuss,	// 来源讨论组
		fromQQ,
		QY::convertAnsiToUtf8(fromInfo).cStr(),
		QY::convertAnsiToUtf8(msg).cStr(),
		QY::convertAnsiToUtf8(info).cStr(),
		intTest);

	PyObject_Call(onPrivateMsg, args, nullptr);
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
	long long RobotID, DestQQ;
	const char* content;
	if (!PyArg_ParseTuple(args, "LLs", &RobotID, &DestQQ,&content)) {
		QYLOG::Error(0, "Python", "QY.sendFriendMsg调用错误！");
	}
	else {
		QY::sendFriendMsg(RobotID, DestQQ, QY::convertUtf8ToAnsi(content));
	}
	return Py_None;
}

PyObject* qy_sendGroupMsg(PyObject* self, PyObject* args) {
	long long RobotID, DestGroup;
	const char* content;
	if (!PyArg_ParseTuple(args, "LLs", &RobotID, &DestGroup, &content)) {
		QYLOG::Error(0, "Python", "QY.sendGroupMsg调用错误！");
	}
	else {
		QY::sendGroupMsg(RobotID, DestGroup, QY::convertUtf8ToAnsi(content));
	}
	return Py_None;
}
PyObject* qy_sendGroupTmpMsg(PyObject* self, PyObject* args) {
	long long RobotID, DestGroup, DestQQ;
	const char* content;
	if (!PyArg_ParseTuple(args, "LLLs", &RobotID, &DestGroup, &DestQQ, &content)) {
		QYLOG::Error(0, "Python", "QY.sendGroupTmpMsg调用错误！");
	}
	else {
		QY::sendGroupTmpMsg(RobotID, DestGroup, DestQQ, QY::convertUtf8ToAnsi(content));
	}
	return Py_None;
}
PyObject* qy_sendDiscussMsg(PyObject* self, PyObject* args) {
	long long RobotID, DestDiscuss;
	const char* content;
	if (!PyArg_ParseTuple(args, "LLs", &RobotID, &DestDiscuss, &content)) {
		QYLOG::Error(0, "Python", "QY.sendDiscussMsg调用错误！");
	}
	else {
		QY::sendDiscussMsg(RobotID, DestDiscuss, QY::convertUtf8ToAnsi(content));
	}
	return Py_None;
}
PyObject* qy_sendDiscussTmpMsg(PyObject* self, PyObject* args) {
	long long RobotID, DestDiscuss, DestQQ;
	const char* content;
	if (!PyArg_ParseTuple(args, "LLLs", &RobotID, &DestDiscuss, &DestQQ, &content)) {
		QYLOG::Error(0, "Python", "QY.sendDiscussTmpMsg调用错误！");
	}
	else {
		QY::sendDiscussTmpMsg(RobotID, DestDiscuss, DestQQ, QY::convertUtf8ToAnsi(content));
	}
	return Py_None;
}
PyObject* qy_setMessageSvcMsgWithDraw(PyObject* self, PyObject* args) {
	long long RobotID;
	const char* MsgInfo;
	if (!PyArg_ParseTuple(args, "Ls", &RobotID, &MsgInfo)) {
		QYLOG::Error(0, "Python", "QY.setMessageSvcMsgWithDraw调用错误！");
	}
	else {
		QY::setMessageSvcMsgWithDraw(RobotID, QY::convertUtf8ToAnsi(MsgInfo));
	}
	return Py_None;
}
PyObject* qy_sendLikeFavorite(PyObject* self, PyObject* args) {
	long long RobotID, DestQQ;
	int count;
	if (!PyArg_ParseTuple(args, "LLi", &RobotID, &DestQQ, &count)) {
		QYLOG::Error(0, "Python", "QY.sendLikeFavorite调用错误！");
	}
	else {
		QY::sendLikeFavorite(RobotID, DestQQ, count);
	}
	return Py_None;
}
PyObject* qy_setFriendDelete(PyObject* self, PyObject* args) {
	long long RobotID, DestQQ;
	if (!PyArg_ParseTuple(args, "LL", &RobotID, &DestQQ)) {
		QYLOG::Error(0, "Python", "QY.setFriendDelete调用错误！");
	}
	else {
		QY::setFriendDelete(RobotID, DestQQ);
	}
	return Py_None;
}
PyObject* qy_setFriendAddRequest(PyObject* self, PyObject* args) {
	long long RobotID;
	int resultFlag;
	const char* responseFlag;
	const char* Remark;
	if (!PyArg_ParseTuple(args, "Lsis", &RobotID, &responseFlag, &resultFlag, &Remark)) {
		QYLOG::Error(0, "Python", "QY.setFriendAddRequest调用错误！");
	}
	else {
		QY::setFriendAddRequest(RobotID, responseFlag, resultFlag, Remark);
	}
	return Py_None;
}
PyObject* qy_setGroupMembersKick(PyObject* self, PyObject* args) {
	long long RobotID, DestGroup, DestQQ;
	bool rejectAddRequest;
	if (!PyArg_ParseTuple(args, "LLLp", &RobotID, &DestGroup, &DestQQ, &rejectAddRequest)) {
		QYLOG::Error(0, "Python", "QY.setGroupMemberKick调用错误！");
	}
	else {
		QY::setGroupMembersKick(RobotID, DestGroup, DestQQ, rejectAddRequest);
	}
	return Py_None;
}
PyObject* qy_setGroupWholeBanSpeak(PyObject* self, PyObject* args) {
	long long RobotID, DestGroup;
	bool newStatus;
	if (!PyArg_ParseTuple(args, "LLp", &RobotID, &DestGroup, &newStatus)) {
		QYLOG::Error(0, "Python", "QY.setGroupWholeBanSpeak调用错误！");
	}
	else {
		QY::setGroupWholeBanSpeak(RobotID, DestGroup, newStatus);
	}
	return Py_None;
}
PyObject* qy_setGroupMemberBanSpeak(PyObject* self, PyObject* args) {
	long long RobotID, DestGroup, DestQQ, timeStamp;
	if (!PyArg_ParseTuple(args, "LLLL", &RobotID, &DestGroup, &DestQQ, &timeStamp)) {
		QYLOG::Error(0, "Python", "QY.setGroupMemberBanSpeak调用错误！");
	}
	else {
		QY::setGroupMemberBanSpeak(RobotID, DestGroup, DestQQ, timeStamp);
	}
	return Py_None;
}
PyObject* qy_setGroupAnonymousBan(PyObject* self, PyObject* args) {
	long long RobotID, DestGroup;
	bool newStatus;
	if (!PyArg_ParseTuple(args, "LLp", &RobotID, &DestGroup, &newStatus)) {
		QYLOG::Error(0, "Python", "QY.setGroupAnonymousBan调用错误！");
	}
	else {
		QY::setGroupAnonymousBan(RobotID, DestGroup, newStatus);
	}
	return Py_None;
}
PyObject* qy_setGroupPrivateSession(PyObject* self, PyObject* args) {
	long long RobotID, DestGroup;
	bool newStatus;
	if (!PyArg_ParseTuple(args, "LLp", &RobotID, &DestGroup, &newStatus)) {
		QYLOG::Error(0, "Python", "QY.setGroupPrivateSession调用错误！");
	}
	else {
		QY::setGroupPrivateSession(RobotID, DestGroup, newStatus);
	}
	return Py_None;
}
PyObject* qy_setGroupManyPeopleChat(PyObject* self, PyObject* args) {
	long long RobotID, DestGroup;
	bool newStatus;
	if (!PyArg_ParseTuple(args, "LLp", &RobotID, &DestGroup, &newStatus)) {
		QYLOG::Error(0, "Python", "QY.setGroupManyPeopleChat调用错误！");
	}
	else {
		QY::setGroupManyPeopleChat(RobotID, DestGroup, newStatus);
	}
	return Py_None;
}
PyObject* qy_setGroupAdmini(PyObject* self, PyObject* args) {
	long long RobotID, DestGroup, DestQQ;
	bool newStatus;
	if (!PyArg_ParseTuple(args, "LLLp", &RobotID, &DestGroup, &DestQQ, &newStatus)) {
		QYLOG::Error(0, "Python", "QY.setGroupAdmini调用错误！");
	}
	else {
		QY::setGroupAdmini(RobotID, DestGroup, DestQQ, newStatus);
	}
	return Py_None;
}
PyObject* qy_setGroupMemberCard(PyObject* self, PyObject* args) {
	long long RobotID, DestGroup, DestQQ;
	const char* newCard;
	if (!PyArg_ParseTuple(args, "LLLs", &RobotID, &DestGroup, &DestQQ, &newCard)) {
		QYLOG::Error(0, "Python", "QY.setGroupMemberCard调用错误！");
	}
	else {
		QY::setGroupMemberCard(RobotID, DestGroup, DestQQ, QY::convertUtf8ToAnsi(newCard));
	}
	return Py_None;
}
PyObject* qy_setGroupSpecialTitle(PyObject* self, PyObject* args) {
	long long RobotID, DestGroup, DestQQ;
	int expireTime;
	const char* specialTitle;
	if (!PyArg_ParseTuple(args, "LLLsi", &RobotID, &DestGroup, &DestQQ, &specialTitle, &expireTime)) {
		QYLOG::Error(0, "Python", "QY.setGroupSpecialTitle调用错误！");
	}
	else {
		QY::setGroupSpecialTitle(RobotID, DestGroup, DestQQ, QY::convertUtf8ToAnsi(specialTitle), expireTime);
	}
	return Py_None;
}
PyObject* qy_setGroupExit(PyObject* self, PyObject* args) {
	long long RobotID, DestGroup;
	bool disband;
	if (!PyArg_ParseTuple(args, "LLp", &RobotID, &DestGroup, &disband)) {
		QYLOG::Error(0, "Python", "QY.setGroupExit调用错误！");
	}
	else {
		QY::setGroupExit(RobotID, DestGroup, disband);
	}
	return Py_None;
}
PyObject* qy_setGroupAdd(PyObject* self, PyObject* args) {
	long long RobotID, DestGroup;
	const char* AdditionalMsg;
	if (!PyArg_ParseTuple(args, "LLs", &RobotID, &DestGroup, &AdditionalMsg)) {
		QYLOG::Error(0, "Python", "QY.setGroupAdd调用错误！");
	}
	else {
		QY::setGroupAdd(RobotID, DestGroup, QY::convertUtf8ToAnsi(AdditionalMsg));
	}
	return Py_None;
}
PyObject* qy_setGroupAddRequest(PyObject* self, PyObject* args) {
	long long RobotID;
	const char* responseFlag;
	int requestType, resultFlag;
	const char* reason;
	if (!PyArg_ParseTuple(args, "Lsiis", &RobotID, &responseFlag, &requestType, &resultFlag, &reason)) {
		QYLOG::Error(0, "Python", "QY.setGroupAddRequest调用错误！");
	}
	else {
		QY::setGroupAddRequest(RobotID, QY::convertUtf8ToAnsi(responseFlag), requestType, resultFlag, QY::convertUtf8ToAnsi(reason));
	}
	return Py_None;
}
PyObject* qy_sendOfflineFile(PyObject* self, PyObject* args) {
	long long RobotID, DestGroup, DestQQ;
	int dwMsgType;
	const char* strLocalPath;
	if (!PyArg_ParseTuple(args, "LiLLs", &RobotID, &dwMsgType, &DestGroup, &DestQQ,&strLocalPath)) {
		QYLOG::Error(0, "Python", "QY.sendOfflineFile调用错误！");
	}
	else {
		QY::sendOfflineFile(RobotID, dwMsgType, DestGroup, DestQQ, QY::convertUtf8ToAnsi(strLocalPath));
	}
	return Py_None;
}
PyObject* qy_setGroupFileUpload(PyObject* self, PyObject* args) {
	long long RobotID, DestGroup;
	const char* strParentFolder;
	const char* strLocalPath;
	if (!PyArg_ParseTuple(args, "LLss", &RobotID, &DestGroup, &strParentFolder, &strLocalPath)) {
		QYLOG::Error(0, "Python", "QY.setGroupFileUpload调用错误！");
	}
	else {
		QY::setGroupFileUpload(RobotID, DestGroup, QY::convertUtf8ToAnsi(strParentFolder), QY::convertUtf8ToAnsi(strLocalPath));
	}
	return Py_None;
}
PyObject* qy_setGroupFileDelete(PyObject* self, PyObject* args) {
	long long RobotID, DestGroup, fileBusID;
	const char* strParentFolder;
	const char* fileID;
	if (!PyArg_ParseTuple(args, "LLLss", &RobotID, &DestGroup, &fileBusID, &strParentFolder, &fileID)) {
		QYLOG::Error(0, "Python", "QY.setGroupFileDelete调用错误！");
	}
	else {
		QY::setGroupFileDelete(RobotID, DestGroup, fileBusID, strParentFolder, fileID);
	}
	return Py_None;
}
PyObject* qy_setDiscussExit(PyObject* self, PyObject* args) {
	long long RobotID, DestDiscuss;
	if (!PyArg_ParseTuple(args, "LL", &RobotID, &DestDiscuss)) {
		QYLOG::Error(0, "Python", "QY.setDiscussExit调用错误！");
	}
	else {
		QY::setDiscussExit(RobotID, DestDiscuss);
	}
	return Py_None;
}
PyObject* qy_setDiscussMembersKick(PyObject* self, PyObject* args) {
	long long RobotID, DestDiscuss, DestQQ;
	if (!PyArg_ParseTuple(args, "LLL", &RobotID, &DestDiscuss, &DestQQ)) {
		QYLOG::Error(0, "Python", "QY.setDiscussMemberKick调用错误！");
	}
	else {
		QY::setDiscussMembersKick(RobotID, DestDiscuss, DestQQ);
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