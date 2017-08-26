import pyautogui
import pyperclip

pyautogui.FAILSAFE = False

screenWidth, screenHeight = pyautogui.size()
#pyautogui.moveTo(screenWidth / 2, screenHeight / 2)
pyautogui.moveTo(577, 12)
pyautogui.click()
pyautogui.moveTo(337, 133)
pyautogui.click()

pyautogui.hotkey('ctrl', 'a')
pyautogui.press('backspace')
#pyautogui.typewrite('江门恩平综合楼5F接入1-POS10406-1:8-01')
pyperclip.copy('江门恩平综合楼5F接入1-POS10406-1:8-01')
#pyperclip.paste()
pyautogui.hotkey('ctrl', 'v')
loc = pyautogui.locateOnScreen('资源配置系统-查询.png')
x, y = pyautogui.center(loc)
pyautogui.moveTo(x, y)
pyautogui.click()