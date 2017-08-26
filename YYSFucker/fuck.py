import pyautogui
import time
import random

pyautogui.moveTo(600, 12)
time.sleep(0.5)
pyautogui.click()

cnt = 20
while cnt > 0:
	pyautogui.press('a')
	time.sleep(random.uniform(3, 5))
	loc = None
	while loc == None:
		loc = pyautogui.locateOnScreen('御魂-准备.png')
		time.sleep(random.uniform(0.5, 1))
	pyautogui.press('d')
	time.sleep(15)
	loc = None
	while loc == None:
		loc = pyautogui.locateOnScreen('御魂-蛇头.png')
		time.sleep(0.5)
	pyautogui.press('s')
	loc = None
	while loc == None:
		loc = pyautogui.locateOnScreen('御魂-魂.png')
		time.sleep(random.uniform(0.5, 1))
	pyautogui.press('a')
	time.sleep(random.uniform(1, 1.5))
	loc = None
	while loc == None:
		loc = pyautogui.locateOnScreen('御魂-挑战.png')
		time.sleep(random.uniform(0.5, 1))
	cnt = cnt - 1
	