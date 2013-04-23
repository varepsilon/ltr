#!/usr/bin/python

import sys
import os
import commands
import popen2

ins, outs = os.popen2("git diff HEAD^ HEAD --name-only --diff-filter=AM")
output_lines =  outs.read().split('\n')
ins.close()
outs.close()
diff_files = []
allow = 1
if len(output_lines) == 0 or output_lines[0] == "":
    sys.exit(0) 
for line in output_lines:
    diff_files.append(line.encode('utf-8'))
for file in diff_files:
    if "contrib" in file.split('/'):
        continue
    if "doc" in file.split('/'):
        continue
    if file.split('.')[-1] != "h" and file.split('.')[-1] != "cc" and file.split('.')[-1] != "cpp" and file.split('.')[-1] != "hpp" :
        continue
    if file.split('/')[-1] == "visitors.h":
        continue
    if file.split('/')[-1] == "train_visitor.h":
        continue
    filters = "--filter=-readability/streams,-runtime/threadsafe_fn,-runtime/rtti,-readability/check"
    if file.split('/')[-1] == "factory.h":
        filters = filters + ",-build/header_guard"
    r, w, e = popen2.popen3('python cpplint.py ' + filters + ' ' + file)
    lint_lines = e.readlines()
    for line in lint_lines:
        print line
    if len(lint_lines) == 0 or len(lint_lines[-1].split()) == 0 or int(lint_lines[-1].split()[-1]) != 0:
        allow = 0
if allow == 0:
    sys.exit(1)
else:
    sys.exit(0)