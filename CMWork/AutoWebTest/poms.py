from selenium import webdriver
import time
import xlrd

class POMS:
    def __init__(self):
        self.url = 'http://poms.gmcc.net'
        self.username = 'linchunyan'
        self.password = 'EDCrfv2017'
        self.driver = webdriver.Chrome()
        self.login()
        self.fuzzy_checked = True

    def login(self):
        self.driver.get(self.url)
        self.driver.find_element_by_id('id.staffcode').send_keys(self.username)
        self.driver.find_element_by_id('id.staffpassword').send_keys(self.password)
        self.driver.find_element_by_css_selector("input[id='loginType'][value='3']").click()
        input('登陆后按enter...')

    def shebei_fugai(self):
        target1 = self.driver.find_elements_by_css_selector("a[menuid='ADDRMGR_STANDARD']")
        target1[1].click()
        self.driver.switch_to.frame(0)

    def switch_to_shebei_chaxun(self):
        self.driver.find_elements_by_css_selector('ul[class="tabs"]>li')[0].click()

    def switch_to_dizhi_chaxun(self):
        self.driver.find_elements_by_css_selector('ul[class="tabs"]>li')[1].click()

    def dizhi_jiebang(self, addr):
        inputBox = self.driver.find_elements_by_css_selector('form[id="queryfromq"]>span[class="combo"]>input')[0]
        inputBox.clear()
        inputBox.send_keys(addr)
        self.driver.find_elements_by_css_selector('form[id="queryfromq"]>a')[0].click()
        time.sleep(0.3)
        tables = self.driver.find_elements_by_css_selector("div[class='datagrid-view2']")
        shebei = tables[1].find_elements_by_css_selector("div[class='datagrid-body'] tr")
        print(len(shebei))

        log_file = open('log.txt', 'a')

        for s in shebei:
            if len(s.find_elements_by_css_selector('td')) <= 20:
                continue
            if s.find_elements_by_css_selector('td')[3].text == '光分纤盒':
                continue
            log_file.write(addr+' '+s.find_elements_by_css_selector('td')[1].text+'\n')
            s.click()
            time.sleep(0.3)
            self.driver.find_elements_by_css_selector('ul[id="ocfPortList"] span[class="tree-hit tree-collapsed"]')[0].click()
            items = self.driver.find_elements_by_css_selector('ul[id="ocfPortList"] span[class="tree-title"]')
            for i in items:
                if i.text == '已绑定地址：' + addr:
                    i.click()
                    break
            self.driver.find_element_by_id('debindBtn').click()
            self.driver.switch_to_default_content()
            time.sleep(0.3)
            m_box = self.driver.find_elements_by_css_selector('div[class="messager-button"]>a')
            #m_box[1].click()
            m_box[0].click()
            self.driver.switch_to_default_content()
            time.sleep(0.3)
            self.driver.find_elements_by_css_selector('div[class="messager-button"]>a')[0].click()
            self.driver.switch_to.frame(0)

        log_file.close()

    def shebei_dizhi_jiebang(self, name, addrs):
        inputBox = self.driver.find_elements_by_id('ocfName')[0]
        inputBox.clear()
        if self.fuzzy_checked:
            self.driver.find_elements_by_id('fuzzy')[0].click()
            self.fuzzy_checked = False
        inputBox.send_keys(name)
        self.driver.find_elements_by_css_selector('form[id="queryfrom"]>a')[0].click()
        shebei = []
        cnt = 0
        while len(shebei) == 0:
            time.sleep(1)
            tables = self.driver.find_elements_by_css_selector("div[class='datagrid-view2']")
            shebei = tables[0].find_elements_by_css_selector("div[class='datagrid-body'] tr")
            cnt = cnt + 1
            if cnt > 10:
                print(name)
                break

        for s in shebei:
            if len(s.find_elements_by_css_selector('td')) <= 20:
                continue
            s.click()
            time.sleep(0.5)
            zhankai = self.driver.find_elements_by_css_selector('ul[id="ocfPortList"] span[class="tree-hit tree-collapsed"]')
            if len(zhankai) == 0:
                continue
            zhankai[0].click()
            items = self.driver.find_elements_by_css_selector('ul[id="ocfPortList"] span[class="tree-title"]')
            for i in items:
                for a in addrs:
                    if i.text == '已绑定地址：' + a:
                        i.click()
            self.driver.find_element_by_id('debindBtn').click()
            time.sleep(0.5)
            m_box = []
            while len(m_box) == 0:
                self.driver.switch_to.default_content()
                m_box = self.driver.find_elements_by_css_selector('div[class="messager-button"]>a')
            m_box[0].click()
            time.sleep(0.5)
            m_box = []
            while len(m_box) == 0:
                self.driver.switch_to.default_content()
                m_box = self.driver.find_elements_by_css_selector('div[class="messager-button"]>a')
            m_box[0].click()
            self.driver.switch_to.frame(0)

def test1():
    p = POMS()
    p.shebei_fugai()
    p.switch_to_dizhi_chaxun()
    f = open('task.txt')
    for item in f.readlines():
        addr = item.rstrip('\n')
        p.dizhi_jiebang(addr)

def test2():
    p = POMS()
    p.shebei_fugai()
    p.switch_to_shebei_chaxun()
    addrs = ['江门市新会区罗坑镇迎宾西路北片区38号101', '江门市新会区罗坑镇迎宾西路北片区4-1号101']
    p.shebei_dizhi_jiebang('江门新会罗坑接入间3F接入1-POS11506-1:8-03', addrs)

def debind_addr(p):
    addr = input('输入地址覆盖表的路径：')
    workbook = xlrd.open_workbook(addr)
    sheet = workbook.sheets()[0]
    addr_map = {}
    for i in range(1, sheet.nrows):
        if str(sheet.row_values(i)[0]).strip(' ') == '':
            break
        if sheet.row_values(i)[0] in addr_map:
            addr_map[sheet.row_values(i)[0]].append(sheet.row_values(i)[1])
        else:
            addr_map[sheet.row_values(i)[0]] = [sheet.row_values(i)[1]]
    '''
    for a in addr_map:
        print(a, addr_map[a])
    '''
    p.shebei_fugai()
    p.switch_to_shebei_chaxun()
    for a in addr_map:
        p.shebei_dizhi_jiebang(a, addr_map[a])

if __name__ == '__main__':
    #test1()
    #test2()
    p = POMS()
    while True:
        debind_addr(p)