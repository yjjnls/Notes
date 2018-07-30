# -*- coding:utf-8 -*-
# from urllib.request import urlopen
# from bs4 import BeautifulSoup
import re
import shutil
import os


class Translator(object):
    def __init__(self, tutor, folder, repo):
        self.tutor = tutor
        self.folder = folder
        self.repo = repo
        self.html = folder + str(tutor) + '.html'
        self.resource = folder + str(tutor) + '_files'
        self.img = folder + 'img'
        self.md_fp = ''
        self.img_number = 1
        self.is_started = False
        self.order_lable = False
        self.label = 1
        self.is_code = False

    def translate(self):
        try:
            fp = open(self.html, 'r', encoding='UTF-8')
            all_lines = fp.readlines()
            for line in all_lines:
                if '' in line:
                    line = line.replace('', '')
                if '﻿' in line:
                    line = line.replace('﻿', '')
                if "audio title=" in line:
                    self.trans_title(line)
                if ">总结<" in line:
                    break
                if "<p>" in line:
                    self.trans_paragraph(line)
                if "<h" in line:
                    self.trans_subtitle(line)
                if "<ol>" in line:
                    self.order_lable = True
                    self.label = 1
                if "</ol>" in line:
                    self.order_lable = False
                if "<li>" in line:
                    self.trans_label(line)
                if "<img" in line:
                    self.trans_img(line)
                if "<code>" in line:
                    self.md_fp.write('```\n')
                    self.is_code = True
                    continue
                if "</code>" in line:
                    self.md_fp.write('```\n')
                    self.is_code = False
                if self.is_code:
                    self.md_fp.write(line)
        except IOError as e:
            print(e.message)
        finally:
            fp.close()
            self.md_fp.close()
            self.is_started = False

    def trans_title(self, line):
        searchObj = re.search('<audio title=\"(.*)\" src', line)
        if searchObj:
            title = searchObj.group(1).split(" ")[2].strip()
            md = self.folder + str(self.tutor) + '.' + title + '.md'
            self.md_fp = open(md, 'w', encoding='UTF-8')
            self.md_fp.seek(0)
            self.md_fp.truncate()
            self.md_fp.write('# ' + title + '\n')
            self.is_started = True

    def trans_paragraph(self, line):
        if self.is_started:
            searchObj = re.search('<p>(.*)</p>', line)
            if searchObj:
                paragraph = searchObj.group(1).strip()
                self.md_fp.write(paragraph + '  \n\n')

    def trans_subtitle(self, line):
        if self.is_started:
            searchObj = re.search('>(.*)</h(\d)>', line)
            if searchObj:
                i = 0
                while (i < int(searchObj.group(2))):
                    self.md_fp.write('#')
                    i += 1
                self.md_fp.write(' ' + searchObj.group(1) + '\n')

    def trans_label(self, line):
        if self.is_started:
            searchObj = re.search('>(.*)<', line)
            if searchObj:
                if not self.order_lable:
                    self.md_fp.write('*\t' + searchObj.group(1) + '\n')
                else:
                    self.md_fp.write(
                        str(self.label) + '.\t' + searchObj.group(1) + '\n')
                    self.label += 1
            else:
                if not self.order_lable:
                    self.md_fp.write('*\t')
                else:
                    self.md_fp.write(str(self.label) + '.\t')
                    self.label += 1

    def trans_img(self, line):
        if self.is_started:
            searchObj = re.search('<img src=\"./(.*)/(.*)\.(.*)\" alt=', line)
            if searchObj:
                origin_img = self.folder + searchObj.group(
                    1) + '/' + searchObj.group(2) + '.' + searchObj.group(3)
                new_img_name = str(self.tutor) + '.' + str(
                    self.img_number) + '.' + searchObj.group(3)
                new_img = self.img + '\\' + str(self.tutor) + '.' + str(
                    self.img_number) + '.' + searchObj.group(3)
                self.img_number += 1
                if not os.path.exists(self.img):
                    os.makedirs(self.img)
                shutil.copyfile(origin_img, new_img)
                self.md_fp.write('![](%s%s)\n\n' % (self.repo, new_img_name))


if __name__ == "__main__":
    tutor = 35
    folder = "E:\\Administrator\\Desktop\\geek\\"
    repo = "https://github.com/yjjnls/blockchain-tutorial-cn/blob/master/img/"
    trans = Translator(tutor, folder, repo)
    trans.translate()
