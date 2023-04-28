#该文件用于生成glfw的键盘宏->字符串表.用在编写glfw_macro_table.cpp上
import re

f = open("key_macro.txt")
macros = f.read()
f.close()
macros = re.split("[ \t\n]", macros)
macros = list(filter(None, macros))
macros = [macros[i:i+3] for i in range(0, len(macros), 3)]


def gen_macro2str_dic():
    print("{", end="")
    for macro in macros[:-1]:
        print(f"{{{macro[1]}, \"{macro[1][9:].lower()}\"}}", end=", ")
    print(f"{{{macros[-1][1]}, \"{macros[-1][1][9:].lower()}\"}}", end="")
    print("}", end="\n")


def gen_str2macro_dic():
    print("{", end="")
    for macro in macros[:-1]:
        print(f"{{\"{macro[1][9:].lower()}\", {macro[1]}}}", end=", ")
    print(f"{{\"{macros[-1][1][9:].lower()}\", {macros[-1][1]}}}", end="")
    print("}", end="\n")
