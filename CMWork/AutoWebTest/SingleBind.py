from selenium import webdriver
from selenium.webdriver.common.keys import Keys
import time
import xlrd, xlwt, xlutils
import sys
import os
import re
from PyQt5.QtWidgets import *
from PyQt5.QtCore import *


class POMS:
    def __init__(self):
        self.url = 'http://poms.gmcc.net'
        self.username = 'linchunyan'
        self.password = 'EDCrfv2017'

        if not os.path.exists('D:\\地址解绑绑定工作目录'):
            os.mkdir('D:\\地址解绑绑定工作目录')

        self.driver = webdriver.Chrome()

        self.login()
        self.fuzzy_checked = True

    def login(self):
        self.driver.get(self.url)
        self.driver.find_element_by_id('id.staffcode').send_keys(self.username)
        self.driver.find_element_by_id('id.staffpassword').send_keys(self.password)
        self.driver.find_element_by_css_selector("input[id='loginType'][value='3']").click()

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
        self.driver.find_elements_by_css_selector('form[id="queryfromq"]>a')[0].click()
        shebei = []
        cnt = 0
        while len(shebei) == 0:
            time.sleep(0.3)
            tables = self.driver.find_elements_by_css_selector("div[class='datagrid-view2']")
            shebei = tables[1].find_elements_by_css_selector("div[class='datagrid-body'] tr")
            cnt = cnt + 1
            if cnt > 10:
                return True, '搜索分光器超时'

        for s in shebei:
            if len(s.find_elements_by_css_selector('td')) <= 10:
                continue
            s.click()
            zhankai = []
            cnt = 0
            shouldContinue = False
            while len(zhankai) == 0:
                time.sleep(0.3)
                zhankai = self.driver.find_elements_by_css_selector( 'ul[id="ocfPortList"] span[class="tree-hit tree-collapsed"]')
                cnt = cnt + 1
                if cnt > 10:
                    shouldContinue = True
                    break
            if shouldContinue:
                continue
            zhankai[0].click()
            items = self.driver.find_elements_by_css_selector('ul[id="ocfPortList"] span[class="tree-title"]')
            for i in items:
                if i.text == '已绑定地址：' + addr:
                    i.click()
                    break
            self.driver.find_element_by_id('debindBtn').click()
            m_box = []
            while len(m_box) == 0:
                time.sleep(0.3)
                self.driver.switch_to.default_content()
                m_box = self.driver.find_elements_by_css_selector('div[class="messager-button"]>a')
            m_box[0].click()
            m_box = []
            while len(m_box) == 0:
                time.sleep(0.3)
                self.driver.switch_to.default_content()
                m_box = self.driver.find_elements_by_css_selector('div[class="messager-button"]>a')
            m_box[0].click()
            time.sleep(0.5)
            self.driver.switch_to.frame(0)

        return True, ''

    def dizhi_bangding(self, addr, device, AB = None):
        time.sleep(0.3)
        inputBox = self.driver.find_element_by_css_selector('input[name="查询级别"]')
        inputBox.clear()
        time.sleep(0.2)
        inputBox.send_keys(Keys.CONTROL + 'a')
        inputBox.send_keys(addr)
        self.driver.find_element_by_css_selector('span[class="searchbox-button"]').click()
        time.sleep(1.5)
        waiting = self.driver.find_elements_by_class_name('datagrid-mask-msg')
        cnt = 0
        while len(waiting) > 0:
            time.sleep(0.5)
            waiting = self.driver.find_elements_by_class_name('datagrid-mask-msg')
            cnt = cnt + 1
            if cnt > 10:
                return False, '搜索地址超时'
        time.sleep(0.5)
        candidate = self.driver.find_elements_by_css_selector('div[style="width:387px;text-align:left;"]')
        notFind = True
        for c in candidate:
            if c.text == addr:
                while True:
                    try:
                        c.click()
                        break
                    except Exception as e:
                        print('显示点击失败')
                        pass
                notFind = False
                break
        if notFind:
            return False, '搜索地址失败'
        time.sleep(2)
        seven = []
        while len(seven) == 0:
            time.sleep(0.3)
            seven = self.driver.find_elements_by_css_selector('li li li li li li li>div')
        seven[0].click()
        time.sleep(0.1)

        self.switch_to_shebei_chaxun()
        inputBox = self.driver.find_element_by_id('ocfName')
        inputBox.clear()
        if self.fuzzy_checked:
            self.driver.find_elements_by_id('fuzzy')[0].click()
            self.fuzzy_checked = False
        inputBox.send_keys(device)
        self.driver.find_elements_by_css_selector('form[id="queryfrom"]>a')[0].click()
        shebei = []
        cnt = 0
        while len(shebei) == 0:
            time.sleep(0.5)
            tables = self.driver.find_elements_by_css_selector("div[class='datagrid-view2']")
            shebei = tables[0].find_elements_by_css_selector("div[class='datagrid-body'] tr")
            cnt = cnt + 1
            if cnt > 10:
                return False, '搜索分光器超时'

        shebei[0].click()
        time.sleep(0.5)
        deviceTree = self.driver.find_elements_by_css_selector('ul[id="ocfPortList"] span[class="tree-title"]')
        while len(deviceTree) == 0 or not deviceTree[1].text.endswith(device):
            time.sleep(0.3)
            deviceTree = self.driver.find_elements_by_css_selector('ul[id="ocfPortList"] span[class="tree-title"]')
        deviceTree[1].click()

        self.driver.find_element_by_id('binding1').click()
        m_box = []
        while len(m_box) == 0:
            time.sleep(0.3)
            m_box = self.driver.find_elements_by_id('IF_ADDR')
        m_box[0].click()
        self.driver.find_elements_by_id('DP_TYPE1')[0].click()
        self.driver.find_elements_by_id('DP_TYPE2')[0].click()
        if AB != None:
            if isinstance(AB, int):
                self.driver.find_elements_by_id('gl_id')[AB].click()
            else:
                return False, 'AB参数应为整数0或1'
        time.sleep(0.2)
        try:
            self.driver.find_elements_by_id('binding1')[1].click()
        except Exception as e:
            return False, '绑定时点击确定失败'
        m_box = []
        while len(m_box) == 0:
            time.sleep(0.3)
            self.driver.switch_to.default_content()
            m_box = self.driver.find_elements_by_css_selector('div[class="messager-button"]>a')
        if len(m_box) == 2:
            m_box[0].click()
            m_box = []
            while len(m_box) == 0:
                time.sleep(0.3)
                self.driver.switch_to.default_content()
                m_box = self.driver.find_elements_by_css_selector('div[class="messager-button"]>a')
            m_box[0].click()
        elif len(m_box) == 1:
            errMsg = self.driver.find_elements_by_css_selector('div[class="messager-body panel-body panel-body-noborder window-body"]>div')[1].text
            m_box[0].click()
            return False, errMsg
        time.sleep(0.5)
        self.driver.switch_to.frame(0)
        return True, ''

class SelectDialog(QDialog):
    def __init__(self):
        super().__init__()
        self.path = os.getcwd()
        self.inited = False
        self.logFile = 'D:\\地址解绑绑定工作目录\\'+time.strftime('%Y-%m-%d-%H-%M',time.localtime(time.time()))+'.txt'
        self.p = POMS()
        self.initUI()

    def initUI(self):
        self.setWindowTitle('选择覆盖表(登陆后再进行操作)')
        self.resize(240, 100)

        grid = QGridLayout()
        grid.addWidget(QLabel('路径'), 0, 0)
        self.editLine = QLineEdit()
        self.editLine.setFixedWidth(200)
        self.editLine.setText(self.path)
        grid.addWidget(self.editLine, 0, 1)
        changPathButton = QPushButton('更改')
        changPathButton.clicked.connect(self.changePath)
        grid.addWidget(changPathButton, 0, 2)

        buttonBox = QDialogButtonBox()
        buttonBox.setOrientation(Qt.Horizontal)
        buttonBox.setStandardButtons(QDialogButtonBox.Ok | QDialogButtonBox.Cancel)
        buttonBox.accepted.connect(self.acceptAction)
        buttonBox.rejected.connect(self.reject)
        grid.addWidget(buttonBox, 1, 1)
        self.setLayout(grid)

    def changePath(self):
        self.path = QFileDialog().getOpenFileName()[0]
        self.editLine.setText(self.path)

    def acceptAction(self):
        if not self.inited:
            self.p.shebei_fugai()
            self.p.switch_to_dizhi_chaxun()
            self.inited = True
        f = open(self.logFile, 'a')
        table = self.path
        workbook = xlrd.open_workbook(table)
        sheet = workbook.sheets()[0]
        for i in range(1, sheet.nrows):
            oldDevice = sheet.row_values(i)[0]
            addr = sheet.row_values(i)[1]
            newDevice = sheet.row_values(i)[2]
            AB = None
            if len(sheet.row_values(i)) >= 4 and str(sheet.row_values(i)[3]).strip() != '':
                s = str(sheet.row_values(i)[3]).strip()
                if s == 'A' or s == 'a':
                    AB = 0
                elif s == 'B' or s == 'b':
                    AB = 1
                else:
                    f.write('AB面信息错误：' + addr + '\n')
            try:
                ok, msg = self.p.dizhi_jiebang(addr)
                if not ok:
                    f.write('解绑失败：'+addr+':'+msg+'\n')
                    continue
                ok, msg = self.p.dizhi_bangding(addr, newDevice, AB)
                if not ok:
                    f.write('绑定失败：'+addr+':'+msg+'\n')
                    self.p.switch_to_dizhi_chaxun()
                    continue
                self.p.switch_to_dizhi_chaxun()
            except Exception as e:
                QMessageBox.information(self, "错误提示", str(e), QMessageBox.Yes)
                break

        f.close()


if __name__ == '__main__':
    app = QApplication(sys.argv)
    dialog = SelectDialog()
    dialog.exec_()