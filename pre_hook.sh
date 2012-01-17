#!/usr/bin/python

import sys
import os
import commands



TMP_DIR = "/tmp/hook_tmp"
os.system("mkdir -p " + TMP_DIR)
os.system("mkdir -p " + TMP_DIR + "/.git")

line = sys.stdin.read()
(oldrev, newrev, ref) = line.strip().split()
print "STARTING [" + oldrev + " " + newrev + " " + ref + "]\n"
output_lines = commands.getoutput("git diff-tree -r  --diff-filter=ACMR " + oldrev + ".." + newrev).split("\n")
diff_files = []
allow = 1
if len(output_lines) == 0 or output_lines[0] == "":
    sys.exit(0) 
for line in output_lines:
    diff_files.append(line.split()[-1])
for file in diff_files:
    if file.split('.')[-1] != "h" and file.split('.')[-1] != "cc" and file.split('.')[-1] != "cpp":
        continue
    if file.split('/')[-1] == "visitors.h":
        continue
    tmp_file = TMP_DIR + "/" + file
    folder = commands.getoutput("dirname " + tmp_file)
    os.system("mkdir -p " + folder)
    os.system("git show " + newrev + ":" + file + " > " + tmp_file)
    lint_lines = commands.getoutput("cd " + TMP_DIR + "; python /home/arbabenko/cpplint.py --filter=-readability/streams,-runtime/threadsafe_fn " + tmp_file).split("\n")
    for line in lint_lines:
        print line+"\n"
    if len(lint_lines) == 0 or len(lint_lines[-1].split()) == 0 or int(lint_lines[-1].split()[-1]) != 0:
        allow = 0
os.system("rm -rf " + TMP_DIR)
if allow == 0:
    sys.exit(1)
else:
    sys.exit(0)