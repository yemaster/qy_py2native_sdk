# ��C++����Native API
try:
 from qysdk_base import *;
except :
 pass
__NowHandler = None;
QY = None;

# �������ڼ̳� & ������Ϣ
class MessageHandler():
	"""
		�¼�˵�������������
		�¼����ƣ�onEnable
		�¼���������
	"""
	def onEnable(self):
		pass

	"""
		�¼�˵�������������
		�¼����ƣ�onDisable
		�¼���������
	"""
	def onDisable(self):
		pass

	"""
		�¼�˵�����˵�A
		�¼����ƣ�onMenuA
		�¼���������
	"""
	def onMenuA(self):
		pass

	"""
		�¼�˵�����˵�B
		�¼����ƣ�onMenuB
		�¼���������
	"""
	def onMenuB(self):
		pass

	"""
		�¼�˵�����˵�C
		�¼����ƣ�onMenuC
		�¼���������
	"""
	def onMenuC(self):
		pass
	
	"""
		�¼�˵����˽����Ϣ
		�¼����ƣ�onPrivateMsg
		�¼�������
			robotID   int       ������QQ��
			subType   int       ��Ϣ���� 11/���Ժ��� 1/��������״̬ 2/����Ⱥ 3/���������� 4/���Թ��ں�
			sendTime  int       ����ʱ��(13λʱ���)
			fromQQ    int       ������QQ��
			fromGroup int       ��ԴȺ��(��������Ϊ2/3ʱʹ��)
			fromInfo  String    ��Դ����Ϣ
			msg       String	��Ϣ����
			info      String    �¼�����Ϣ
			intTest   String	���ڲ���(��Ҳ��֪�����������������
	"""
	def onPrivateMsg(self,
				robotID,
				subType,
				sendTime,
				fromQQ,
				fromGroup,
				fromInfo,
				msg,
				info,
				intTest):
		pass
		
	"""
		�¼�˵����Ⱥ��Ϣ
		�¼����ƣ�onGroupMsg
		�¼�������
			robotID   int       ������QQ��
			subType   int       �����ͣ�Ŀǰ�̶�Ϊ1
			sendTime  int       ����ʱ��(13λʱ���)
			fromGroup int       ��ԴȺ��
			fromQQ    int       ������QQ��
			fromInfo  String    ��Դ����Ϣ
			msg       String	��Ϣ����
			info      String    �¼�����Ϣ
			intTest   String	���ڲ���(��Ҳ��֪�����������������
	"""
	def onGroupMsg(self,
				robotID,
				subType,
				sendTime,
				fromGroup,
				fromQQ,
				fromInfo,
				msg,
				info,
				intTest):
		pass

	is_Handler = True; # ��Ҫ�����������������ڲ�ʹ�ã�
	pass


# �÷���������ã�����������Ϣ������
def setHandler(handler):
	__NowHandler = handler()
	pass

# �÷���������ã�����֪ͨC++
def main():
	try:
		cpp_main(__NowHandler);
	except:
		pass
	pass

# �����ṩ��Լ��Native API�����ɼ̳�
class cpp_api():
	"""
	��������:QY.log
	��������:
		eventName  String
		logContent String
	"""
	def log(self,eventName,logContent):
		try:
			cpp_log(eventName,logContent)
		except:
			pass
		pass
	"""
	��������:QY.warning
	��������:
		eventName  String
		logContent String
	"""
	def warning(self,eventName,logContent):
		try:
			cpp_warning(eventName,logContent)
		except:
			pass
		pass
	pass
	"""
	��������:QY.error
	��������:
		eventName  String
		logContent String
	"""
	def error(self,eventName,logContent):
		try:
			cpp_error(eventName,logContent)
		except:
			pass
		pass
	"""
	��������:QY.sendFriendMsg
	��������:
		robotID  int
		destQQ   int
		content  String(UTF8)
	"""
	def sendFriendMsg(self,robotID,destQQ,content):
		try:
			cpp_sendFriendMsg(robotID,destQQ,content)
		except:
			pass
		pass
	pass
	
	"""
	��������:QY.utf8Decode
	��������:
		content  String(UTF8)
	����:
		content String(Ansi)
	"""
	def sendFriendMsg(self,content):
		try:
			cpp_utf8Decode(content)
		except:
			pass
		pass
	pass

#�ṩ������Լ��api������QY.log
QY = cpp_api();