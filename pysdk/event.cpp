// event.cpp : ���� DLL Ӧ�ó���ĵ���������
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

//��Python�ļ����ã�����py_obj_handler
PyObject *cpp_main(PyObject *self, PyObject *args);
//��װ�����־
PyObject *qy_log(PyObject *self, PyObject *args);
PyObject *qy_warning(PyObject *self, PyObject *args);
PyObject *qy_error(PyObject *self, PyObject *args);
//��װ������Ϣ�ĺ���
PyObject* qy_sendFriendMsg(PyObject* self, PyObject* args);
PyObject* qy_sendGroupMsg(PyObject* self, PyObject* args);
//��װ��Ƭ��
PyObject* qy_sendLikeFavorite(PyObject* self, PyObject* args);
//��װȺ�Ĳ���
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
//��װ����ת��
PyObject *qy_utf8Decode(PyObject *self, PyObject *args);

/*
 * ���ز��������Ϣ
 * ����������ֹ�����������κδ���
 */
QYEVENT(CSTRING) AppInfo()
{
	return QY::getJsonInfo();
}


/*
 * ����Ӧ��AuthCode
 * ����������ֹ�����������κδ��룬�벻Ҫ�Ķ�������
 */
QYEVENT(INT32) Initialize(
	INT32	AuthCode,		// Ӧ�ñ�ʶ
	INT32	ProtocolType	// Э������ 1 / ��׿QQЭ�� 2 / PCQQЭ��
)
{
	QY::setAuthCode(AuthCode);
	QY::setProtocolType(ProtocolType);
	
	return 0;
}

/*
 * [1001]����������
 * ���ӳ�����ڡ����̡߳��б����á�
 * ���۱�Ӧ���Ƿ����ã�������������������������ִ��һ�Σ���������ִ�в����ʼ�����롣
 * ����ؾ��췵�ر��ӳ��򣬷���Ῠס��������Լ�������ļ��ء�
 * �����ֱֹ�����봰��
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
	PyMethodDef method_cpp_sendLikeFavorite = { "cpp_sendLikeFavorite",qy_sendLikeFavorite,METH_VARARGS,"Send somebody Likes" };
	PyMethodDef method_cpp_setGroupMembersKick = { "cpp_setGroupMembersKick",qy_setGroupMembersKick,METH_VARARGS,"Set group members kick" };
	PyMethodDef method_cpp_setGroupWholeBanSpeak = { "cpp_setGroupWholeBanSpeak",qy_setGroupWholeBanSpeak,METH_VARARGS,"Set group whole ban speak" };
	PyMethodDef method_cpp_setGroupMemberBanSpeak = { "cpp_setGroupMemberBanSpeak",qy_setGroupMemberBanSpeak,METH_VARARGS,"Set group member ban speak" };
	PyMethodDef method_cpp_setGroupAnonymousBan = { "cpp_setGroupAnonymousBan",qy_setGroupAnonymousBan,METH_VARARGS,"Set group anonymous ban" };
	PyMethodDef method_cpp_setGroupPrivateSession = { "cpp_setGroupPrivateSession",qy_setGroupPrivateSession,METH_VARARGS,"Set group private session" };
	PyMethodDef method_cpp_setGroupManyPeopleChat = { "cpp_setGroupManyPeopleChat",qy_setGroupManyPeopleChat,METH_VARARGS,"Set group many people chat" };
	PyMethodDef method_cpp_setGroupAdmini = { "cpp_setGroupAdmini",qy_setGroupAdmini,METH_VARARGS,"Set group admini" };
	PyMethodDef method_cpp_setGroupMemberCard = { "cpp_setGroupMemberCard",qy_setGroupMemberCard,METH_VARARGS,"Set a group member's card" };
	PyMethodDef method_cpp_setGroupSpecialTitle = { "cpp_setGroupSpecialTitle",qy_setGroupSpecialTitle,METH_VARARGS,"Set Group Exit" };
	PyMethodDef method_cpp_setGroupExit = { "cpp_setGroupExit",qy_setGroupExit,METH_VARARGS,"Send somebody Likes" };
	PyMethodDef method_cpp_utf8Decode = { "cpp_utf8Decode",qy_utf8Decode,METH_VARARGS,"Utf8Decode to ansi" };

	//ģ�鷽���б�
	PyMethodDef methods[] = {
		method_cpp_main,
		method_cpp_log,
		method_cpp_warning,
		method_cpp_error,
		method_cpp_sendFriendMsg,
		method_cpp_sendGroupMsg,
		method_cpp_sendLikeFavorite,
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
		method_cpp_utf8Decode
	};
	//ģ�鶨��
	PyModuleDef module_def =
	{
		PyModuleDef_HEAD_INIT,
		"qysdk_base",
		NULL,
		-1,
		methods
	};
	//����ģ��
	obj_module = PyModule_Create(&module_def);
	module_dict = PyModule_GetDict(obj_module);
	PyState_AddModule(obj_module, &module_def);

	string python_code = pysdk::read_resource(IDR_PYMAIN);
	if (PyRun_SimpleStringFlags(python_code.c_str(), NULL) == -1) {
		using sstream = std::stringstream;
		sstream error;
		error << "�ű�ִ�д���!\r\n" << std::endl;
		std::thread([&]() {
			AllocConsole();
			PyErr_Print();
			system("pause");
			HWND console = GetConsoleWindow();
			CloseWindow(console);
		}).detach();
		QY::setAppSelfDiscontinue();
	}
	return 0; //��̶�����0��
}

/*
 * [1002]�������˳�
 * ���ӳ�����ڡ����̡߳��б����á�
 * ���۱�Ӧ���Ƿ����û�ж�أ��������������˳�ǰִ��һ�Σ���������ִ�в���رմ��롣
 */
QYEVENT(INT32) QY_Event_Exit()
{
	return 0; //��̶�����0��
}

/*
 * [1003]Ӧ�ñ�����
 * ���ӳ�����ڡ����̡߳��б����á�
 * ��Ӧ�ñ����ú󣬽��յ����¼���
 * �������ʱӦ���ѱ����ã�����_eventStartup(Type=1001,����)�����ú󣬱�����Ҳ��������һ�Ρ�
 * ��Ǳ�Ҫ����������������ش��ڡ���������Ӳ˵������û��ֶ��򿪴��ڣ�
 */
QYEVENT(INT32) QY_Event_Enable()
{

	QY::setAppEnable(true);
	if (py_obj_handler) {
		PyObject *onEnable = PyObject_GetAttrString(py_obj_handler,"onEnable");
		PyObject_Call(onEnable,PyTuple_New(0) , NULL);
	}
	return 0; //��̶�����0��
}

/*
 * [1004]Ӧ�ñ�ͣ��
 * ��Ӧ�ñ�ͣ��ǰ�����յ����¼���
 * �������ʱӦ���ѱ�ͣ�û�ж�أ��򱾺��������᡿�����á�
 * ���۱�Ӧ���Ƿ����ã�������ر�ǰ�������������᡿�����á�
 */
QYEVENT(INT32) QY_Event_Disable()
{
	QY::setAppEnable(false);
	if (py_obj_handler) {
		PyObject *onDisable = PyObject_GetAttrString(py_obj_handler, "onDisable");
		PyObject_Call(onDisable, PyTuple_New(0), NULL);
	}
	return 0; //��̶�����0��
}

/*
 * [166]˽����Ϣ
 * ���ӳ�����ڡ��̡߳��б����ã���ע��ʹ�ö������Ҫ��ʼ��(CoInitialize,CoUninitialize)��
 */
QYEVENT(INT32) QY_Event_PrivateMsg(
	INT64		robotID,		// ������QQ���������ֶ�ŵ�¼
	INT32		subType,		// ��Ϣ������11/���Ժ��� 1/��������״̬ 2/����Ⱥ 3/���������� 4/���Թ��ں�
	INT64		sendTime,		// ����ʱ��(13λʱ���)
	INT64		fromQQ,			// ��Դ����
	INT64		fromGroup,		// ��ԴȺ��(��������Ϊ2/3ʱʹ��)
	CSTRING		fromInfo,		// ��Դ����Ϣ
	CSTRING		msg,			// ��Ϣ����
	CSTRING		info,			// ��Ϣ
	INT32		intTest			// ���ڲ���(��Ҳ��֪�����������������
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
	// ���Ҫ�ظ���Ϣ�������API���ͣ��������� return QY::EVENT_BLOCK - �ضϱ�����Ϣ�����ټ�������
	// ע�⣺Ӧ�����ȼ�����Ϊ"���"(10000)ʱ������ʹ�ñ�����ֵ
	// ������ظ���Ϣ������֮���Ӧ��/�������������� return QY::EVENT_IGNORE - ���Ա�����Ϣ
	return QY::EVENT_IGNORE;
}

/*
 * [82]Ⱥ��Ϣ
 * ���ӳ�����ڡ��̡߳��б����ã���ע��ʹ�ö������Ҫ��ʼ��(CoInitialize,CoUninitialize)��
 */
QYEVENT(INT32) QY_Event_GroupMsg(
	INT64		robotID,		// ������QQ���������ֶ�ŵ�¼
	INT32		subType,		// �����ͣ�Ŀǰ�̶�Ϊ1
	INT64		sendTime,		// ����ʱ��(13λʱ���)
	INT64		fromGroup,		// ��ԴȺ��
	INT64		fromQQ,			// ��ԴQQ��
	CSTRING		fromInfo,		// ��Դ����Ϣ
	CSTRING		msg,			// ��Ϣ����
	CSTRING		info,			// �¼�����Ϣ
	INT32		intTest			// ���ڲ���
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
	return QYAPI::EVENT_IGNORE; //���ڷ���ֵ˵��, ����QY_Event_PrivateMsg������
}

/*
 * [83]��������Ϣ
 * ���ӳ�����ڡ��̡߳��б����ã���ע��ʹ�ö������Ҫ��ʼ��(CoInitialize,CoUninitialize)��
 */
QYEVENT(INT32) QY_Event_DiscussMsg(
	INT64		robotID,		// ������QQ���������ֶ�ŵ�¼
	INT32		subType,		// �����ͣ�Ŀǰ�̶�Ϊ1
	INT64		sendTime,		// ����ʱ��(13λʱ���)
	INT64		fromDiscuss,	// ��Դ������
	INT64		fromQQ,			// ��ԴQQ��
	CSTRING		fromInfo,		// ��Դ����Ϣ
	CSTRING		msg,			// ��Ϣ����
	CSTRING		info,			// �¼�����Ϣ
	INT32		intTest			// ���ڲ���
)
{
	PyObject *onPrivateMsg = PyObject_GetAttrString(py_obj_handler, "onDiscussMsg");

	PyObject *args = Py_BuildValue("(LiLLLsssi)",
		robotID,
		subType,
		sendTime,
		fromDiscuss,	// ��Դ������
		fromQQ,
		QY::convertAnsiToUtf8(fromInfo).cStr(),
		QY::convertAnsiToUtf8(msg).cStr(),
		QY::convertAnsiToUtf8(info).cStr(),
		intTest);

	PyObject_Call(onPrivateMsg, args, nullptr);
	return QY::EVENT_IGNORE; //���ڷ���ֵ˵��, ����QY_Event_PrivateMsg������
}

/*
 * [101]������Ϣ
 * ���ӳ�����ڡ��̡߳��б����ã���ע��ʹ�ö������Ҫ��ʼ��(CoInitialize,CoUninitialize)��
 */
QYEVENT(INT32) QY_Event_PushMsgEvent(
	INT64		robotID,		// ������QQ���������ֶ�ŵ�¼
	INT32		subType,		// �����ͣ�Ŀǰ�̶�Ϊ1
	INT64		sendTime,		// ����ʱ��(13λʱ���)
	INT64		fromGroup,		// ��ԴȺ��
	INT64		fromQQ,			// ��������
	INT64		param,			// ������������������»���QQ��
	CSTRING		msg,			// �¼���Ϣ
	CSTRING		info,			// �¼�����Ϣ
	INT32		intTest			// ���ڲ���
)
{
	
	return QY::EVENT_IGNORE; //���ڷ���ֵ˵��, ����QY_Event_PrivateMsg������
}

/*
 * [102]Ⱥ�¼�-����Ա�䶯
 * ���ӳ�����ڡ��̡߳��б����ã���ע��ʹ�ö������Ҫ��ʼ��(CoInitialize,CoUninitialize)��
 */
QYEVENT(INT32) QY_Event_System_GroupAdmin(
	INT64	robotID,		// ������QQ���������ֶ�ŵ�¼
	INT32	subType,		// �����ͣ�1/��ȡ������Ա 2/�����ù���Ա
	INT64	sendTime,		// ����ʱ��(13λʱ���)
	INT64	fromGroup,		// ��ԴȺ��
	INT64	beingOperateQQ	// ����������
)
{
	return QY::EVENT_IGNORE; //���ڷ���ֵ˵��, ����QY_Event_PrivateMsg������
}

/*
 * [103]Ⱥ�¼�-Ⱥ��Ա����
 * ���ӳ�����ڡ��̡߳��б����ã���ע��ʹ�ö������Ҫ��ʼ��(CoInitialize,CoUninitialize)��
 */
QYEVENT(INT32) QY_Event_System_GroupMemberDecrease(
	INT64	robotID,		// ������QQ���������ֶ��
	INT32	subType,		// �����ͣ�1/Ⱥ��ɢ 2/ȺԱ�뿪 3/ȺԱ���ߡ�
	INT64	sendTime,		// ����ʱ��(13λʱ���)
	INT64	fromGroup,		// ��ԴȺ��
	INT64	fromQQ,			// ��������(��������Ϊ2��3ʱ����)
	INT64	beingOperateQQ	// ����������
)
{
	return QY::EVENT_IGNORE; //���ڷ���ֵ˵��, ����QY_Event_PrivateMsg������
}

/*
 * [104]Ⱥ�¼�-Ⱥ��Ա����
 * ���ӳ�����ڡ��̡߳��б����ã���ע��ʹ�ö������Ҫ��ʼ��(CoInitialize,CoUninitialize)��
 */
QYEVENT(INT32) QY_Event_System_GroupMemberIncrease(
	INT64	robotID,		// ������QQ���������ֶ��
	INT32	subType,		// �����ͣ�1/����Ա��ͬ�� 2/����Ա���롣
	INT64	sendTime,		// ����ʱ��(13λʱ���)
	INT64	fromGroup,		// ��ԴȺ��
	INT64	fromQQ,			// ��������(��������Ϊ2��3ʱ����)
	INT64	beingOperateQQ	// ����������
)
{
	return QY::EVENT_IGNORE; //���ڷ���ֵ˵��, ����QY_Event_PrivateMsg������
}

/*
 * [201]�������Ϊ���ѽ���¼�
 * ���ӳ�����ڡ��̡߳��б����ã���ע��ʹ�ö������Ҫ��ʼ��(CoInitialize,CoUninitialize)��
 */
QYEVENT(INT32) QY_Event_FriendEvent(
	INT64		robotID,		// ������QQ���������ֶ��
	INT32		subType,		// �����ͣ�1/�Է�ͬ����� 2/�Է��ܾ����
	INT64		sendTime,		// ����ʱ��(13λʱ���)
	INT64		fromQQ,			// ��Դ����
	CSTRING		info			// ��Ϣ
)
{
	return QY::EVENT_IGNORE; //���ڷ���ֵ˵��, ����QY_Event_PrivateMsg������
}

/*
 * [301]����-������Ӻ���
 * ���ӳ�����ڡ��̡߳��б����ã���ע��ʹ�ö������Ҫ��ʼ��(CoInitialize,CoUninitialize)��
 */
QYEVENT(INT32) QY_Event_Request_AddFriend(
	INT64		robotID,		// ������QQ���������ֶ��
	INT32		subType,		// �����ͣ�Ŀǰ�̶�Ϊ1
	INT64		sendTime,		// ����ʱ��(13λʱ���)
	INT64		fromQQ,			// ��Դ����
	CSTRING		source,			// ��Դ��Ϣ
	CSTRING		msg,			// ����
	CSTRING		responseFlag	// ������ʶ(����������)
)
{

	return QY::EVENT_IGNORE; //���ڷ���ֵ˵��, ����QY_Event_PrivateMsg������
}

/*
 * [302]����-Ⱥ���
 * ���ӳ�����ڡ��̡߳��б����ã���ע��ʹ�ö������Ҫ��ʼ��(CoInitialize,CoUninitialize)��
 */
QYEVENT(INT32) QY_Event_Request_AddGroup(
	INT64		robotID,		// ������QQ���������ֶ��
	INT32		responseType,	// �������ͣ�1/����������Ⱥ 2/Ⱥ��Ա����������Ⱥ 3/�Լ�(����¼��)������Ⱥ
	INT64		sendTime,		// ����ʱ��(13λʱ���)
	INT64		fromGroup,		// ��ԴȺ��
	INT64		fromQQ,			// ��Դ����
	CSTRING		source,			// ��Դ��Ϣ����������Ϊ1ʱ����Դ������Ϊ2��3ʱ��������QQ
	CSTRING		msg,			// ����
	CSTRING		responseFlag	// ������ʶ(����������)
)
{

	return QY::EVENT_IGNORE; //���ڷ���ֵ˵��, ����QY_Event_PrivateMsg������
}

/*
 * �˵����������� info.json ��Դ�ļ������ò˵���Ŀ��������
 * �����ʹ�ò˵������ڡ� info.json��events.def �Լ� �˴� ��ɾ�����ú���
 */
QYEVENT(INT32) _menuA()
{
	if (py_obj_handler) {
		PyObject *onEnable = PyObject_GetAttrString(py_obj_handler, "onMenuA");
		PyObject_Call(onEnable, PyTuple_New(0), NULL);
	}
	return 0; //��̶�����0
}

QYEVENT(INT32) _menuB()
{
	if (py_obj_handler) {
		PyObject *onEnable = PyObject_GetAttrString(py_obj_handler, "onMenuB");
		PyObject_Call(onEnable, PyTuple_New(0), NULL);
	}
	return 0; //��̶�����0
}

QYEVENT(INT32) _menuC()
{
	if (py_obj_handler) {
		PyObject *onEnable = PyObject_GetAttrString(py_obj_handler, "onMenuC");
		PyObject_Call(onEnable, PyTuple_New(0), NULL);
	}
	return 0; //��̶�����0
}

#undef QYEVENT



PyObject *cpp_main(PyObject *self, PyObject *args) {
	PyArg_ParseTuple(args, "O", py_obj_handler);
	PyObject *isHandler;
	isHandler = PyObject_GetAttrString(py_obj_handler, "O");
	if (!PyObject_IsTrue(isHandler)) {
		QYLOG::Fatal(0, "Python", "Handler����ȷ!");
		QY::setAppSelfDiscontinue();
	}
	return Py_None;
}


PyObject *qy_log(PyObject *self, PyObject *args) {
	const char *event_name,*log_content;
	if (!PyArg_ParseTuple(args, "ss", &event_name, &log_content)) {
		QYLOG::Error(0, "Python", "QY.log���ô���");
	}
	else {
		QYLOG::Info(0,event_name,log_content);
	}
	return Py_None;
}

PyObject *qy_warning(PyObject *self, PyObject *args) {
	const char *event_name,*log_content;
	if (!PyArg_ParseTuple(args, "ss", &event_name, &log_content)) {
		QYLOG::Error(0, "Python", "QY.warning���ô���");
	}
	else {
		QYLOG::Warning(0,event_name,log_content);
	}
	return Py_None;
}

PyObject *qy_error(PyObject *self, PyObject *args) {
	const char *event_name, *log_content;
	if (!PyArg_ParseTuple(args, "ss", &event_name, &log_content)) {
		QYLOG::Error(0, "Python", "QY.error���ô���");
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
		QYLOG::Error(0, "Python", "QY.sendFriendMsg���ô���");
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
		QYLOG::Error(0, "Python", "QY.sendGroupMsg���ô���");
	}
	else {
		QY::sendGroupMsg(RobotID, DestGroup, QY::convertUtf8ToAnsi(content));
	}
	return Py_None;
}
PyObject* qy_sendLikeFavorite(PyObject* self, PyObject* args) {
	long long RobotID, DestQQ;
	int count;
	if (!PyArg_ParseTuple(args, "LLi", &RobotID, &DestQQ, &count)) {
		QYLOG::Error(0, "Python", "QY.sendLikeFavorite���ô���");
	}
	else {
		QY::sendLikeFavorite(RobotID, DestQQ, count);
	}
	return Py_None;
}
PyObject* qy_setGroupMembersKick(PyObject* self, PyObject* args) {
	long long RobotID, DestGroup, DestQQ;
	bool rejectAddRequest;
	if (!PyArg_ParseTuple(args, "LLLp", &RobotID, &DestGroup, &DestQQ, &rejectAddRequest)) {
		QYLOG::Error(0, "Python", "QY.setGroupMemberKick���ô���");
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
		QYLOG::Error(0, "Python", "QY.setGroupWholeBanSpeak���ô���");
	}
	else {
		QY::setGroupWholeBanSpeak(RobotID, DestGroup, newStatus);
	}
	return Py_None;
}
PyObject* qy_setGroupMemberBanSpeak(PyObject* self, PyObject* args) {
	long long RobotID, DestGroup, DestQQ, timeStamp;
	if (!PyArg_ParseTuple(args, "LLLL", &RobotID, &DestGroup, &DestQQ, &timeStamp)) {
		QYLOG::Error(0, "Python", "QY.setGroupMemberBanSpeak���ô���");
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
		QYLOG::Error(0, "Python", "QY.setGroupAnonymousBan���ô���");
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
		QYLOG::Error(0, "Python", "QY.setGroupPrivateSession���ô���");
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
		QYLOG::Error(0, "Python", "QY.setGroupManyPeopleChat���ô���");
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
		QYLOG::Error(0, "Python", "QY.setGroupAdmini���ô���");
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
		QYLOG::Error(0, "Python", "QY.setGroupMemberCard���ô���");
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
		QYLOG::Error(0, "Python", "QY.setGroupSpecialTitle���ô���");
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
		QYLOG::Error(0, "Python", "QY.setGroupExit���ô���");
	}
	else {
		QY::setGroupExit(RobotID, DestGroup, disband);
	}
	return Py_None;
}
PyObject *qy_utf8Decode(PyObject *self, PyObject *args) {
	const char* content;
	if (!PyArg_ParseTuple(args, "s",  &content)) {
		QYLOG::Error(0, "Python", "QY.utf8Decode���ô���");
	}
	else {
		const char* ret = QY::convertUtf8ToAnsi(content);
		return Py_BuildValue("s", ret);
	}
	return Py_None;
}