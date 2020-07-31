import qysdk;
QY = qysdk.QY;
class Handler(qysdk.MessageHandler):
	def onEnable(self):
		QY.log("Python插件Demo","插件被启用!")
		pass
	def onDisable(self):
		QY.log("Python插件Demo","插件被禁用!")
	pass

# 设置消息处理类
qysdk.setHandler(Handler);
# 执行main方法
qysdk.main();