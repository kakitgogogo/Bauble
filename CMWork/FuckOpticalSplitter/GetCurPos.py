import win32gui
import pyautogui

def getPos():
	return win32gui.GetCursorPos()

def getPixel():
	print(getPos(), pyautogui.pixel(getPos()[0], getPos()[1]))