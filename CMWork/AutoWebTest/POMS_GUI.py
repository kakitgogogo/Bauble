from selenium import webdriver
from selenium.webdriver.common.keys import Keys
import time
import xlrd, xlwt, xlutils
import sys
import os
import re
from PyQt5.QtWidgets import *
from PyQt5.QtCore import *

curTime = time.strftime('%Y-%m-%d-%H-%M',time.localtime(time.time()))

class POMS:
    def __init__(self):
        self.url = 'http://poms.gmcc.net'
        #self.username = 'linchunyan'
        #self.password = 'EDCrfv2017'

        if not os.path.exists('D:\\地址绑定工作目录'):
            os.mkdir('D:\\地址绑定工作目录')
        if not os.path.exists('D:\\地址绑定工作目录\\'+curTime):
            os.mkdir('D:\\地址绑定工作目录\\'+curTime)

        options = webdriver.ChromeOptions()
        prefs = {'profile.default_content_settings.popups': 0, 'download.default_directory': 'D:\\地址绑定工作目录\\'+curTime}
        options.add_experimental_option('prefs', prefs)
        self.driver = webdriver.Chrome(chrome_options=options)

        self.login()
        self.fuzzy_checked = True

    def login(self):
        self.driver.get(self.url)
        #self.driver.find_element_by_id('id.staffcode').send_keys(self.username)
        #self.driver.find_element_by_id('id.staffpassword').send_keys(self.password)
        self.driver.find_element_by_css_selector("input[id='loginType'][value='3']").click()

    def shebei_fugai(self):
        target1 = self.driver.find_elements_by_css_selector("a[menuid='ADDRMGR_STANDARD']")
        target1[1].click()
        self.driver.switch_to.frame(0)

    def biaozhun_dizhi(self):
        target1 = self.driver.find_elements_by_css_selector("a[menuid='ADDRMGR_STANDARD']")
        target1[0].click()
        self.driver.switch_to.frame(0)

    def switch_to_shebei_chaxun(self):
        self.driver.find_elements_by_css_selector('ul[class="tabs"]>li')[0].click()

    def switch_to_dizhi_chaxun(self):
        self.driver.find_elements_by_css_selector('ul[class="tabs"]>li')[1].click()

    def refresh(self):
        self.driver.refresh()
        time.sleep(1)
        self.shebei_fugai()
        time.sleep(0.5)
        self.fuzzy_checked = True

    def dizhi_jiebang(self, device, addrs):
        try:
            inputBox = self.driver.find_elements_by_id('ocfName')
            if len(inputBox) == 0:
                try:
                    self.driver.switch_to.frame(0)
                except Exception as e:
                    print('切换框架')
                    pass
            inputBox = self.driver.find_elements_by_id('ocfName')[0]
            inputBox.clear()
            if self.fuzzy_checked:
                self.driver.find_elements_by_id('fuzzy')[0].click()
                self.fuzzy_checked = False
            inputBox.send_keys(device)
            self.driver.find_elements_by_css_selector('form[id="queryfrom"]>a')[0].click()
            waiting = self.driver.find_elements_by_css_selector('div[class="datagrid-mask-msg"]')
            while len(waiting) > 0:
                time.sleep(0.5)
                waiting = self.driver.find_elements_by_css_selector('div[class="datagrid-mask-msg"]')
            tables = self.driver.find_elements_by_css_selector("div[class='datagrid-view2']")
            shebei = tables[0].find_elements_by_css_selector("div[class='datagrid-body'] tr")
            if len(shebei) == 0:
                return False, '搜索不到分光器'
            s = shebei[0].find_elements_by_css_selector('td')[1]
            if s.text != device:
                print(device+'：搜出的分光器名称可能不正确('+s.text+')')
            s.click()
            deviceTree = self.driver.find_elements_by_css_selector('ul[id="ocfPortList"] span[class="tree-title"]')
            while len(deviceTree) == 0 or not deviceTree[1].text.endswith(device):
                time.sleep(0.3)
                deviceTree = self.driver.find_elements_by_css_selector('ul[id="ocfPortList"] span[class="tree-title"]')
            zhankai = self.driver.find_elements_by_css_selector('ul[id="ocfPortList"] span[class="tree-hit tree-collapsed"]')
            if len(zhankai) == 0:
                return True, '设备没有地址'
            zhankai[0].click()
            items = self.driver.find_elements_by_css_selector('ul[id="ocfPortList"] span[class="tree-title"]')
            cnt = 0
            addrs.sort()
            for i in items:
                for a in addrs:
                    if i.text == '已绑定地址：' + a:
                        try:
                            i.click()
                            cnt = cnt + 1
                            addrs.remove(a)
                            break
                        except Exception as e:
                            return False, '点击地址失败：' + str(e)
            if cnt == 0:
                return True, '没有需要解绑的地址'
            time.sleep(0.5)
            self.driver.find_element_by_id('debindBtn').click()
            m_box = []
            while len(m_box) == 0:
                time.sleep(0.3)
                self.driver.switch_to.default_content()
                m_box = self.driver.find_elements_by_css_selector('div[class="messager-button"]>a')
            m_box[0].click()
            sleepTime = 0.5
            if cnt > 20:
                sleepTime = 0.05 * cnt
            time.sleep(sleepTime)
            m_box = []
            while len(m_box) == 0:
                time.sleep(0.3)
                self.driver.switch_to.default_content()
                m_box = self.driver.find_elements_by_css_selector('div[class="messager-button"]>a')
            m_box[0].click()
            time.sleep(0.5)
            self.driver.switch_to.frame(0)

            return True, '顺利解绑'
        except Exception as e:
            return False, '程序问题：' + str(e)

    def dizhi_bangding(self, devicePath):
        time.sleep(0.5)
        try:
            self.driver.find_element_by_css_selector('a[menuid="ADDRMGR_DPSEAREA_IMPORT"]').click()
        except Exception as e:
            return False, '点击覆盖地址导入失败'
        addrInput = []
        while len(addrInput) == 0:
            time.sleep(0.2)
            addrInput = self.driver.find_elements_by_css_selector('a[href="javascript:showTempTree()"]')
        addrInput[0].click()
        time.sleep(0.2)
        self.driver.find_elements_by_css_selector('form[id="upLoadForm"]>input')[0].send_keys(devicePath)
        self.driver.find_elements_by_css_selector('a[id="btn_upload"]')[0].click()
        time.sleep(0.5)
        insureButton = []
        while len(insureButton) == 0:
            time.sleep(0.5)
            insureButton = self.driver.find_elements_by_css_selector('div[class="messager-button"]>a')
        insureButton[0].click()
        insureButton = []
        while len(insureButton) == 0:
            time.sleep(0.5)
            insureButton = self.driver.find_elements_by_css_selector('div[class="messager-button"]>a')
        box = self.driver.find_elements_by_css_selector('div[class="messager-body panel-body panel-body-noborder window-body"]>div')
        msg = box[1].text
        failNum = int(re.findall('\d+', msg)[1])
        box[3].click()
        self.driver.find_elements_by_css_selector('div[id="addrImpState"]>div[class="dialog-button"]>a')[0].click()
        if failNum > 0:
            self.driver.find_element_by_css_selector('a[href="javascript:saveBatchNoData();"]').click()
        time.sleep(0.5)
        self.driver.find_element_by_css_selector('a[class="panel-tool-close"]').click()
        time.sleep(0.5)

        return True, msg

    def single_dizhi_jiebang(self, addr):
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
            time.sleep(1)
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

    def single_dizhi_bangding(self, addr, device, AB = None):
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
        time.sleep(1)
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

    def changeFTTMode(self, addr):
        time.sleep(0.5)
        p.driver.find_element_by_css_selector('input[name="查询级别"]').send_keys()

class SelectDialog(QDialog):
    def __init__(self):
        super().__init__()
        self.path = os.getcwd()
        self.inited = False
        self.logFile = 'D:\\地址绑定工作目录\\'+curTime+'\\绑定记录.txt'
        self.errFile = 'D:\\地址绑定工作目录\\' + curTime + '\\成功记录.txt'
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

    def debindOnly(self, sheet):
        f = open(self.logFile, 'a')
        ferr = open(self.errFile, 'a')
        addr_map = {}
        for i in range(1, sheet.nrows):
            if str(sheet.row_values(i)[0]).strip(' ') == '':
                break
            if sheet.row_values(i)[0] in addr_map:
                addr_map[sheet.row_values(i)[0]].append(sheet.row_values(i)[1])
            else:
                addr_map[sheet.row_values(i)[0]] = [sheet.row_values(i)[1]]

        try:
            for device in addr_map:
                cur = device
                ok, errMsg = self.p.dizhi_jiebang(device, addr_map[device])
                if not ok:
                    f.write('解绑失败：'+device+':'+errMsg+'\n')
                    if errMsg != '搜索不到分光器':
                        self.p.refresh()
                        continue
                time.sleep(0.5)
                ferr.write(device+'\n')
            QMessageBox.information(self, "成功提示", '已完成解绑', QMessageBox.Yes)
        except Exception as e:
            QMessageBox.information(self, "错误提示(分光器：{})\n".format(cur), str(e), QMessageBox.Yes)
        finally:
            f.close()
            ferr.close()

    def singleDebindOnly(self, sheet):
        self.p.switch_to_dizhi_chaxun()
        f = open(self.logFile, 'a')
        ferr = open(self.errFile, 'a')
        for i in range(1, sheet.nrows):
            addr = sheet.row_values(i)[0]
            try:
                ok, msg = self.p.single_dizhi_jiebang(addr)
                if not ok:
                    f.write('解绑失败：'+addr+':'+msg+'\n')
                    continue
                ferr.write(addr + '\n')
            except Exception as e:
                QMessageBox.information(self, "错误提示", str(e), QMessageBox.Yes)
                break
        QMessageBox.information(self, "成功提示", '已完成解绑', QMessageBox.Yes)
        f.close()
        ferr.close()

    def singleBind(self, sheet):
        self.p.switch_to_dizhi_chaxun()
        f = open(self.logFile, 'a')
        ferr = open(self.errFile, 'a')
        for i in range(1, sheet.nrows):
            addr = sheet.row_values(i)[0]
            newDevice = sheet.row_values(i)[1]
            AB = None
            if len(sheet.row_values(i)) >= 3 and str(sheet.row_values(i)[2]).strip() != '':
                s = str(sheet.row_values(i)[2]).strip()
                if s == 'A' or s == 'a':
                    AB = 0
                elif s == 'B' or s == 'b':
                    AB = 1
                else:
                    f.write('AB面信息错误：' + addr + '\n')
            try:
                ok, msg = self.p.single_dizhi_jiebang(addr)
                if not ok:
                    f.write('解绑失败：'+addr+':'+msg+'\n')
                    continue
                ok, msg = self.p.single_dizhi_bangding(addr, newDevice, AB)
                if not ok:
                    f.write('绑定失败：'+addr+':'+msg+'\n')
                    self.p.switch_to_dizhi_chaxun()
                    continue
                ferr.write(addr+'\n')
                self.p.switch_to_dizhi_chaxun()
            except Exception as e:
                QMessageBox.information(self, "错误提示", str(e), QMessageBox.Yes)
                break
        QMessageBox.information(self, "成功提示", '已完成解绑绑定', QMessageBox.Yes)
        f.close()
        ferr.close()

    def makeTable(self, boxes, addrs, AB, filepath):
        source = xlrd.open_workbook('设备覆盖地址模板.xls')
        sourceSheet = source.sheet_by_index(0)
        wb = xlwt.Workbook()
        ws = wb.add_sheet('设备覆盖地址_导入（新）', cell_overwrite_ok=True)
        for i in range(0, 4):
            for j in range(0, 6):
                ws.write(i, j, sourceSheet.row_values(i)[j])
        ws.write(4, 0, sourceSheet.row_values(4)[0])
        i = 4
        for a in addrs:
            ws.write(i, 1, boxes[a])
            ws.write(i, 2, a)
            ws.write(i, 3, '是')
            ws.write(i, 4, '2')
            if a in AB:
                ws.write(i, 5, AB[a])
            i = i + 1
        wb.save(filepath)

    def acceptAction(self):
        if not self.inited:
            self.p.shebei_fugai()
            self.p.switch_to_shebei_chaxun()
            self.inited = True
        table = self.path
        workbook = xlrd.open_workbook(table)
        sheet = workbook.sheets()[0]
        if len(sheet.row_values(0)) == 2:
            self.debindOnly(sheet)
            return
        if len(sheet.row_values(0)) == 1:
            self.singleDebindOnly(sheet)
            return
        if len(sheet.row_values(0)) == 3:
            self.singleBind(sheet)
            return
        f = open(self.logFile, 'a')
        ferr = open(self.errFile, 'a')
        addr_map = {}
        box_map = {}
        AB_map = {}
        for i in range(1, sheet.nrows):
            if str(sheet.row_values(i)[0]).strip(' ') == '':
                break
            if sheet.cell(i, 2).ctype == xlrd.XL_CELL_ERROR:
                continue
            if sheet.row_values(i)[0] in addr_map:
                addr_map[sheet.row_values(i)[0]].append(sheet.row_values(i)[1])
            else:
                addr_map[sheet.row_values(i)[0]] = [sheet.row_values(i)[1]]
            if sheet.row_values(i)[1] in box_map:
                QMessageBox.information(self, "错误提示", '出现重复地址: 行号：' + str(i), QMessageBox.Yes)
                return
            box_map[sheet.row_values(i)[1]] = sheet.row_values(i)[2]
            if len(sheet.row_values(i)) >= 4:
                AB_map[sheet.row_values(i)[1]] = sheet.row_values(i)[3]

        filepath = {}
        for device in addr_map:
            filepath[device] = 'D:\\地址绑定工作目录\\'+device.replace(':', '$')+'.xls'
            try:
                self.makeTable(box_map, addr_map[device], AB_map, filepath[device])
            except Exception as e:
                QMessageBox.information(self, "错误提示", str(e), QMessageBox.Yes)
                return

        cur = ''
        try:
            for device in addr_map:
                cur = device
                ok, msg = self.p.dizhi_jiebang(device, addr_map[device])
                if not ok:
                    f.write('解绑失败：' + device + ':' + msg + '\n')
                    if msg.startswith('程序问题'):
                        break
                    self.p.refresh()
                    continue
                time.sleep(0.5)
                ok, msg = self.p.dizhi_bangding(filepath[device])
                if not ok:
                    f.write('绑定失败：' + device + ':' + msg + '\n')
                    self.p.refresh()
                    continue
                f.write('绑定情况：' + device + ':' + msg+'\n')
                ferr.write(device+'\n')
            QMessageBox.information(self, "成功提示", '已成功解绑绑定', QMessageBox.Yes)
        except Exception as e:
            ferr.write(cur + '\n')
            QMessageBox.information(self, "错误提示(分光器：{})\n".format(cur), str(e), QMessageBox.Yes)
        finally:
            f.close()
            ferr.close()


if __name__ == '__main__':
    app = QApplication(sys.argv)
    dialog = SelectDialog()
    dialog.exec_()