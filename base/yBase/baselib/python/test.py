import re
import urllib.request

def getHtml(url):
    page = urllib.request.urlopen(url)
    html = page.read()
    return html.decode("utf-8");

def getImg(html):
    reg = r'src="(.+?\.jpg)" pic_ext'
    imgre = re.compile(reg)
    imglist = re.findall(imgre,html)
    x = 0
    for imgurl in imglist:
        urllib.request.urlretrieve(imgurl,'d:\\1\\%s.jpg' % x)
        x+=1


html = getHtml("http://tieba.baidu.com/p/2460150866")

print (getImg(html))
