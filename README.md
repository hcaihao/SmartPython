# 项目简介

多年前做的小工具，放出来给有需要的人研究吧，技术交流6850825@qq.com。

* 支持代码高亮显示
* 支持代码单步调试
* 支持代码中断执行
* 支持导入第三方Py库
* 支持导入自定义模块
* 支持调用C扩展
* 支持调用函数
* 支持输入输出重定向
* 支持直接编译生成Exe


# 测试脚本
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

# 功能演示
![image](https://github.com/hcaihao/SmartPython/blob/main/demo.gif)
