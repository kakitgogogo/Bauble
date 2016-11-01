#!/usr/bin/env python
#-*- coding:utf-8 -*-
import os
import sys
import argparse
import logging
import re

parser = argparse.ArgumentParser()

parser.add_argument('file')
parser.add_argument('-v','--variant', default = "")
parser.add_argument('-t','--target', default = "")
parser.add_argument('-c','--comment', help = 'delete comments', action = 'store_true')

args = parser.parse_args()

file = args.file
variant = args.variant
target = args.target
comment = args.comment

logger = logging.getLogger(__name__)

tag_map = {
	".c" :	["//", "/\*", "\*/"],
	".cpp" :	["//", "/\*", "\*/"],
	".asm":	["\;", "null", "null"],
	".py":	["#", "'''", "'''"],
	".html":	["null", "<!--", "-->"]
}

def initLogger():
	formatter = logging.Formatter('%(levelname)s: %(message)s')
	handler = logging.StreamHandler()
	handler.setFormatter(formatter)
	logger.setLevel(logging.INFO)
	logger.addHandler(handler)

def delete_block_comment(text, tag):
	if tag[1] != "null":
		pattern = re.compile(tag[1] + ".*?" + tag[2], re.S)
		return re.sub(pattern, "", text)
	return text

def delete_line_comment(text, tag):
	if tag[0] != "":
		pattern = re.compile(tag[0] + ".*")
		return re.sub(pattern, "", text)
	return text

def delete_comment(text):
	suffix = os.path.splitext(file)[-1]
	text = delete_line_comment(text, tag_map[suffix])
	return delete_block_comment(text, tag_map[suffix])

def tidy(text):
	pattern = re.compile("[\n]{3,}")
	return re.sub(pattern, "\n\n", text)

def fuck():
	fd = open(file, "r")
	text = fd.read()
	fd.close()

	pattern = re.compile(variant)
	text = re.sub(pattern, target, text)
	logger.info("%s -> %s"%(variant, target))

	if comment:
		text = delete_comment(text)
		logger.info("Delete Comments")

	text = tidy(text)

	fd = open("new_"+file, "w")
	fd.write(text)
	fd.close()

def main():
	initLogger()
	fuck()

if __name__=='__main__':
	main()