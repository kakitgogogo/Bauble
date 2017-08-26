import pyautogui
import time
import random

pyautogui.moveTo(600, 12)
time.sleep(0.5)
pyautogui.click()

loc = None
while loc == None:
	loc = pyautogui.locateOnScreen('御魂-人头.png')
	time.sleep(random.uniform(0.5, 1))

pyautogui.press('d')