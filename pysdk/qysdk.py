# 从C++导入Native API
try:
 from qysdk_base import *;
except :
 pass
__NowHandler = None;
QY = None;

# 该类用于继承 & 处理消息
class MessageHandler():
	"""
		事件说明：插件被启用
		事件名称：onEnable
		事件参数：无
	"""
	def onEnable(self):
		pass

	"""
		事件说明：插件被禁用
		事件名称：onDisable
		事件参数：无
	"""
	def onDisable(self):
		pass

	"""
		事件说明：菜单A
		事件名称：onMenuA
		事件参数：无
	"""
	def onMenuA(self):
		pass

	"""
		事件说明：菜单B
		事件名称：onMenuB
		事件参数：无
	"""
	def onMenuB(self):
		pass

	"""
		事件说明：菜单C
		事件名称：onMenuC
		事件参数：无
	"""
	def onMenuC(self):
		pass
	
	"""
		事件说明：私聊消息
		事件名称：onPrivateMsg
		事件参数：
			robotID   int       机器人QQ号
			subType   int       消息渠道 11/来自好友 1/来自在线状态 2/来自群 3/来自讨论组 4/来自公众号
			sendTime  int       发送时间(13位时间戳)
			fromQQ    int       发送者QQ号
			fromGroup int       来源群组(仅子类型为2/3时使用)
			fromInfo  String    来源者信息
			msg       String	消息内容
			info      String    事件总信息
			intTest   String	用于测试(我也不知道机器人作者想干嘛
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
		事件说明：群消息
		事件名称：onGroupMsg
		事件参数：
			robotID   int       机器人QQ号
			subType   int       子类型，目前固定为1
			sendTime  int       发送时间(13位时间戳)
			fromGroup int       来源群号
			fromQQ    int       发送者QQ号
			fromInfo  String    来源者信息
			msg       String	消息内容
			info      String    事件总信息
			intTest   String	用于测试(我也不知道机器人作者想干嘛
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

	is_Handler = True; # 不要更改这个变量！框架内部使用！
	pass


# 该方法必须调用，用于设置消息处理类
def setHandler(handler):
	__NowHandler = handler()
	pass

# 该方法必须调用，用于通知C++
def main():
	try:
		cpp_main(__NowHandler);
	except:
		pass
	pass

# 该类提供契约的Native API，不可继承
class cpp_api():
	"""
	函数名称:QY.log
	函数参数:
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
	函数名称:QY.warning
	函数参数:
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
	函数名称:QY.error
	函数参数:
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
	函数名称:QY.sendFriendMsg
	函数参数:
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
	函数名称:QY.utf8Decode
	函数参数:
		content  String(UTF8)
	返回:
		content String(Ansi)
	"""
	def sendFriendMsg(self,content):
		try:
			cpp_utf8Decode(content)
		except:
			pass
		pass
	pass

#提供各种契约的api，例如QY.log
QY = cpp_api();