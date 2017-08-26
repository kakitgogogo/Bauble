import os
import xlrd, xlwt

d = input('输入文件夹：')

l = os.listdir(d)

wb = xlwt.Workbook()
ws = wb.add_sheet('设备覆盖地址_导入（新）', cell_overwrite_ok=True)
source = xlrd.open_workbook('设备覆盖地址模板.xls')
sourceSheet = source.sheet_by_index(0)

for i in range(1, 6):
    ws.col(i).width = 6000
i = 1

for i in range(0, 4):
    for j in range(0, 6):
        ws.write(i, j, sourceSheet.row_values(i)[j])
ws.write(4, 0, sourceSheet.row_values(4)[0])

r = 4
for item in l:
    if item.startswith('问题数据导出'):
        sheet = xlrd.open_workbook(d+'\\'+item).sheet_by_index(0)
        for i in range(4, sheet.nrows):
            for j in range(1, 6):
                ws.write(r, j, sheet.row_values(i)[j])
            r = r + 1

wb.save(d+'\\问题集合.xls')




