#!/usr/bin/env python
#-*- coding:utf-8 -*-
import os
import sys
import argparse
import logging

parser=argparse.ArgumentParser()

parser.add_argument('directory')
parser.add_argument('-s','--suffix')
parser.add_argument('-t','--target')
parser.add_argument('-c','--dochange',help='change file name(1~n)',action='store_true')

args=parser.parse_args()

directory=args.directory
suffix=args.suffix
target=args.target
doChange=args.dochange

logger = logging.getLogger(__name__)

def initLogger():
	formatter=logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
	handler=logging.StreamHandler()
	handler.setFormatter(formatter)
	logger.setLevel(logging.INFO)
	logger.addHandler(handler)

def rename():
	os.chdir(directory)
	cwd=os.getcwd()
	logger.info("current directory: %s"%cwd)
	files=os.listdir(cwd)
	for i,file in enumerate(files):
		if file.endswith('.'+suffix):		
			if doChange:
				newFileName=str(i)+'.'+target
			else:
				fileNameLen=len(file)-len(suffix)-1
				newFileName=file[0:fileNameLen]+'.'+target
			if os.path.exists(newFileName):
				continue
			try:
				os.rename(file,newFileName)
				logger.info("rename %s to %s"%(file,newFileName))
			except os.error:
				logger.error("rename %s failed"%file)

def main():
	initLogger()
	rename()

if __name__=='__main__':
	main()