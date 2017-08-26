import pyautogui
import pyperclip
import time
import re
import os

fr = open('list.txt')
curTime = time.strftime('%Y-%m-%d-%H',time.localtime(time.time()))
if not os.path.exists('检查结果'):
	os.mkdir('检查结果')
if not os.path.exists('检查结果\\'+curTime):
	os.mkdir('检查结果\\'+curTime)
if not os.path.exists('检查结果\\'+curTime+'\\光路拓扑图'):
	os.mkdir('检查结果\\'+curTime+'\\光路拓扑图')
fw = open('检查结果\\'+curTime+'\\检查记录.txt', 'a')
pyautogui.click(577, 12)

for shebei in fr.readlines():
	shebei = shebei.rstrip()
	ports = [shebei + '-IN-01']
	if shebei.endswith('2:32'):
		ports = [shebei + '-IN-01', shebei + '-01-IN.01', shebei+'-01-LAN.IN-01']
	goOn = False
	for port in ports:
		pyautogui.click(469, 82)
		pyautogui.hotkey('ctrl', 'a')
		pyautogui.press('backspace')
		pyperclip.copy(port)
		pyautogui.hotkey('ctrl', 'v')
		pyautogui.hotkey('alt', 'q')

		cnt = 0
		while True:
			cnt = cnt + 1
			if cnt >= 6:
				break
			time.sleep(0.5)
			for i in range(1118, 1168):
				if not pyautogui.pixelMatchesColor(i, 128, (255, 255, 255)):
					goOn = True
					break
			if goOn:
				break
		if goOn:
			break

	if not goOn:
		fw.write(shebei + '  ' + '可能查不到光路' + '\n')
		continue

	pyautogui.moveTo(1074, 128)
	pyautogui.click()
	pyautogui.click(button='right')
	pyautogui.moveRel(10, 10)
	pyautogui.click()

	fw.write(shebei+'  '+pyperclip.paste()+'\n')

	loc = pyautogui.locateOnScreen('光路管理系统图标.png')
	x, y = pyautogui.center(loc)
	pyautogui.click(x, y)

	pyautogui.click(29, 91)

	pyautogui.click(463, 162)
	pyautogui.hotkey('ctrl', 'a')
	pyautogui.press('backspace')
	pyautogui.hotkey('ctrl', 'v')

	pyautogui.click(999, 280)

	cnt = 0
	loc = None
	while loc == None:
		time.sleep(0.5)
		loc = pyautogui.locateOnScreen('光路管理系统-江门.png')
		cnt = cnt + 1
		if cnt >= 6:
			break
	if cnt >= 6:
		print(shebei + '：搜到的可能是光路设计名称')
		loc = pyautogui.locateOnScreen('光路管理系统-叉.png')
		x, y = pyautogui.center(loc)
		pyautogui.click(x, y)
		loc = pyautogui.locateOnScreen('光路管理系统图标.png')
		x, y = pyautogui.center(loc)
		pyautogui.click(x - 20, y)
		time.sleep(0.1)
		continue

	pyautogui.doubleClick(327, 363)

	time.sleep(1)
	pyautogui.click(182, 90)

	loc = None
	while loc == None:
		time.sleep(0.5)
		loc = pyautogui.locateOnScreen('光路管理系统-拼图.png')
	pyautogui.click(248, 329)
	pyautogui.click(399, 86)

	while not pyautogui.pixelMatchesColor(765, 357, (212, 208, 200)):
		time.sleep(0.5)

	im = pyautogui.screenshot(region=(246, 137, 1000, 500))
	im.save('检查结果\\'+curTime+'\\光路拓扑图\\' + re.sub(':', '$', shebei) + '.jpg')

	time.sleep(0.5)
	pyautogui.click(1354, 61)

	loc = pyautogui.locateOnScreen('光路管理系统图标.png')
	x, y = pyautogui.center(loc)
	pyautogui.click(x-20, y)
	time.sleep(0.1)


input('按enter退出...')

