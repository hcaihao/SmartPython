# -*- coding:utf-8 -*-
import datetime
import logging
import requests
import json
import base64
import re
import time
import random
import uuid
import configparser
from time import sleep
from http.client import HTTPConnection

from Crypto.Hash import MD5
from Crypto.Cipher import DES
from Crypto.Util.Padding import pad, unpad, PaddingError
from binascii import *
from Crypto.Util.py3compat import *

cf = configparser.ConfigParser()
cf.read("config.txt")
timeout = cf.getint("setting", "timeout")
interval = cf.getint("setting", "interval")

logging.basicConfig(level=logging.DEBUG, filename="log.txt")
logger = logging.getLogger(__name__)


def debug_requests_on():
    '''Switches on logging of the requests module.'''
    HTTPConnection.debuglevel = 1
    logging.basicConfig()
    logging.getLogger().setLevel(logging.DEBUG)
    requests_log = logging.getLogger("requests.packages.urllib3")
    requests_log.setLevel(logging.DEBUG)
    requests_log.propagate = True


def debug_requests_off():
    '''Switches off logging of the requests module, might be some side-effects'''
    HTTPConnection.debuglevel = 0
    root_logger = logging.getLogger()
    root_logger.setLevel(logging.WARNING)
    root_logger.handlers = []
    requests_log = logging.getLogger("requests.packages.urllib3")
    requests_log.setLevel(logging.WARNING)
    requests_log.propagate = False

class CJsonEncoder(json.JSONEncoder):
    def default(self, obj):
        if isinstance(obj, datetime.datetime):
            return obj.strftime('%Y-%m-%d %H:%M:%S')
        elif isinstance(obj, datetime.date):
            return obj.strftime("%Y-%m-%d")
        else:
            return json.JSONEncoder.default(self, obj)


def get_json(dict):
    return json.dumps(dict, cls=CJsonEncoder)


def get_nonce():
    return ''.join([str(random.randint(1, 9)) for i in range(4)]) + "." + ''.join(
        [str(random.randint(0, 9)) for i in range(12)])


def get_current_time():
    return datetime.datetime.strftime(datetime.datetime.now(), '%Y-%m-%d %H:%M:%S')


def get_current_timestamp():
    return str(round(time.time() * 1000))


def get_current_ip():
    ret = requests.get('http://gpt5.com/ip/my.php', verify = False, timeout=timeout)
    j = json.loads(ret.text)
    return j["ip"]


def get_rand_string(len):
    seed = "1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
    sa = []
    for i in range(len):
        sa.append(random.choice(seed))
    result = ''.join(sa)
    return result

def get_rand_phone():
    seed = ["186", "189", "188", "177", "159", "180", "152", "151", "131", "132", "134", "184", "176", "171", "183"]
    return random.choice(seed) + str(random.randint(00000000, 99999999))


def get_rand_device_id():
    return str(random.randint(100000000000000, 999999999999999))


def compare_version(version1, version2):
    v1 = version1.split(".")
    v2 = version2.split(".")

    if len(v1) != len(v2):
        return False

    for i in range(len(v1)):
        if int(v1[i]) < int(v2[i]):
            return False
        elif int(v1[i]) > int(v2[i]):
            return True

    return True


# 当在windows下用文本模式读取文件中的文本时，Python将\r\n自动转化为\n，相反写入文本时，将\n 转化成\r\n
def is_used_ip(ip):
    ret = False
    with open("ip.txt", "a+") as f:
        f.seek(0)
        for line in f.readlines():
            if line.strip() == ip:
                ret = True
                break
    return ret


def set_ip_used(ip):
    with open('ip.txt', "a+") as f:
        f.write(ip + "\n")


def clear_used_ip():
    with open('ip.txt', "r+") as f:
        f.seek(0)
        f.truncate()  # 清空文件


def write_log(log):
    print("[{}] {}".format(time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(time.time())), log))
    logger.info("[{}] {}".format(time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(time.time())), log))


def img_to_base64(path):
    with open(path, "rb") as f:
        return base64.b64encode(f.read()).decode()


def base64_encode(plain):
    chiper = base64.b64encode(plain.encode('utf-8'))
    return str(chiper, 'utf-8')


def base64_decode(cipher):
    plain = base64.b64decode(cipher.encode('utf-8'))
    return str(plain, 'utf-8')


def get_pay_img(seller_name, seller_alipay, pay_price):
    seller_name = seller_name + "(*" + seller_name[1:] + ")"

    if len(seller_alipay) == 11:
        seller_alipay = seller_alipay[0:3] + "******" + seller_alipay[-2:]
    if "@" in seller_alipay:
        [(account, domain)] = re.findall(r"(.*)@(.*)", seller_alipay)
        seller_alipay = account[0:3] + "***@" + domain

    url = "http://gpt5.com/transfer/alipay.php?name=%s&username=%s&amount=%s" % (seller_name, seller_alipay, pay_price)
    ret = requests.get(url, timeout=timeout)
    with open("pay.jpg", "wb+") as f:
        f.write(ret.content)


def get_md5(plain):
    obj = MD5.new()
    obj.update(plain.encode('utf-8'))  # gb2312 Or utf-8
    cipher = obj.hexdigest()
    return cipher

def get_uuid():
    return str(uuid.uuid1()).replace("-", "")
