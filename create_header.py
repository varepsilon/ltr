#!/usr/bin/python
import os, sys

def splitall(path):
    allparts = []
    while 1:
        parts = os.path.split(path)
        if parts[0] == path:  # sentinel for absolute paths
            allparts.insert(0, parts[0])
            break
        elif parts[1] == path: # sentinel for relative paths
            allparts.insert(0, parts[1])
            break
        else:
            path = parts[0]
            allparts.insert(0, parts[1])
    return allparts

def add_guards(header_name, text):
    """Embraces text with ifdef-guards

    :param header_name: ifdef-guard name
    :param text: main header text
    :return: embraced text

    """

    return "#ifndef " + header_name + "\n" +\
        "#define " + header_name + "\n" +\
        "\n" +\
        text + "\n" +\
        "\n" +\
        "#endif\n"

def extract_headers_from_dir(path, files):
    """Finds all .h/.hpp files in a directory

    Only ltr files are proceeded. Contrib libraries, boost files and
    test modules are ignored.

    :param path: path to the directory
    :param files: regular files in the directory
    :return: concatenation of all appropriate header names

    """

    path = "/".join(splitall(path)[1:])
    if path.find("boost") != -1 or path.find("test") != -1:
        return ""
    if path.find("utility") != -1 and not path.endswith("utility/"):
        return ""

    headers = ""

    for file in files:
        if not (file.endswith(".h") or file.endswith(".hpp")):
            continue
        headers += "#include \"%s\"\n" % (path + "/" + file)

    return headers

start_dir = ""
if len(sys.argv) >= 2:
    start_dir = sys.argv[1]

includes = ""
for path, subdirs, files in os.walk(start_dir):
    includes += extract_headers_from_dir(path, files)

print add_guards("LTR", includes)
