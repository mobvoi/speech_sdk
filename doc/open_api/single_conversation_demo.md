## 单轮对话开发示例
以onebox接口为例：


1.  脚本接收一个输入文件地址（可以是相对路径，也可以是绝对路径），文件内容是一些样例query，每一句query占一行；
2.	脚本结果输出到当前执行目录的result文件里，也可以在执行脚本时手动执行 -o 参数，把结果输出到指定文件；
3.  执行脚本前，请先自行修改appkey值和secret值；
4.	该脚本只在mac系统下测试过，使用python 2.7版本。

```python
# coding=utf-8
import argparse
import hashlib
import requests
import os
import sys
import time

def func(input,out):
  appkey = '<app key，开放平台获取>'
  secret = ‘<app secret，开发平台获取>’
  address = '北京市,北京市,,,,,39.91488908,116.40387397'

  with open(input, 'rb') as fo:
    with open(output, 'wb') as fw:
      for line in fo.readlines():
        # ------ 生成加密字符串 ------
        timestamp = str(int(time.time()))

        message = '+'.join([appkey, secret, timestamp])
        m = hashlib.md5()
        m.update(message)
        signature = m.hexdigest()
        url = 'https://open.mobvoi.com/api/onebox/get?query={line}&appkey={appkey}&signature={signature}&timestamp={timestamp}&address={address}&output=lite&version=43000&ignore_context=true'
        url = url.format(line=line, appkey=appkey, signature=signature, timestamp=timestamp, address=address)

        print url
        # ----------------------------

        rv = requests.get(url)
        if not rv.status_code == 200:
          print 'interupted. rv.text'
          break
        fw.write(rv.text.encode('utf-8'))
        fw.write('\n')
        time.sleep(0.1)

if __name__ == '__main__':
  parser = argparse.ArgumentParser()
  parser.add_argument('input', help='specify the input file')
  parser.add_argument('-o', '--output', default='result', help='specify the output file, default is ./result')
  args = parser.parse_args()
  input,output = args.input,args.output
  if not os.path.isabs(input):
    curr_dir = os.path.dirname(os.path.realpath(__file__))
    input = os.path.join(curr_dir,input)
  if not os.path.exists(input) or not os.path.isfile(input):
    print 'there is no such input file.'
    sys.exit(1)
  func(input, output)
```
