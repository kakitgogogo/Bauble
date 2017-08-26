from selenium import webdriver
import time

driver = webdriver.Chrome()

driver.get('http://poms.gmcc.net')

driver.find_element_by_id('id.staffcode').send_keys('linchunyan')
driver.find_element_by_id('id.staffpassword').send_keys('EDCrfv2017')
driver.find_element_by_css_selector("input[id='loginType'][value='3']").click()

input('登陆后按enter...')

print(driver.title)
#print(driver.page_source)
target1 = driver.find_elements_by_css_selector("a[menuid='ADDRMGR_STANDARD']")
#target1[0].click()
target1[1].click()

#driver.execute_script("document.getElementsByClassName('tabs-scroller-left')[0].style.display='block'")

#driver.switch_to.frame(driver.find_element_by_css_selector('iframe'))
driver.switch_to.frame(0)

driver.find_elements_by_css_selector("div[class='tabs-wrap'].ul.li")[1].click()

while True:
    input('按enter...')
    print(driver.page_source)

#driver.quit()