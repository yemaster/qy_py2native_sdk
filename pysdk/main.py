import qysdk;
QY = qysdk.QY;
class Handler(qysdk.MessageHandler):
	def onEnable(self):
		QY.log("Python���Demo","���������!")
		pass
	def onDisable(self):
		QY.log("Python���Demo","���������!")
	pass

# ������Ϣ������
qysdk.setHandler(Handler);
# ִ��main����
qysdk.main();