# 项目简介 Introduction

多年前做的小工具，放出来给有需要的人研究吧，可以作为目前很火的RPA编辑器的架构原型，技术交流6850825@qq.com。This is a small tool I made years ago that is now being released for those who need it to study. It can serve as an architectural prototype for the currently popular RPA editor, and for technical exchanges, please contact 6850825@qq.com.

# 功能特性 Features

* 支持代码高亮显示 (Supports code highlighting.)
* 支持代码单步调试 (Supports single-step debugging.)
* 支持代码中断执行 (Supports interrupting code execution.)
* 支持导入第三方Py库 (Supports importing third-party Py libraries.)
* 支持导入自定义模块 (Supports importing custom modules.)
* 支持调用C扩展 (Supports calling C extensions.)
* 支持调用函数 (Supports calling functions.)
* 支持输入输出重定向 (Supports input and output redirection.)
* 支持直接编译生成Exe (Supports direct compilation to generate Exe files.)

# 测试脚本 Test
```
import json
import os
import requests
import sys
import utility
import chilkat
from time import sleep

sleep(1)
print("--Py模块测试--")
print(utility.get_uuid())

sleep(1)
print("--C扩展测试--")
import SmartPythonExt
print(SmartPythonExt.greet())

sleep(1)
print("--函数测试--")
def get_current_ip():
	#ret = requests.get('http://ipinfo.io/json')
	ret = requests.get('https://api.myip.com', timeout=30)
	j_ret = json.loads(ret.text)
	return j_ret["ip"]
print(get_current_ip())

sleep(1)
print("--IO测试--")
line = input("请输入：")
print(line)
```

# 功能演示 Demo
![image](https://github.com/hcaihao/SmartPython/blob/main/demo.gif)
