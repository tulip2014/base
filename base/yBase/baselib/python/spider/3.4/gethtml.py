
import urllib.request
import re

def getHtml(url) :
    page = urllib.request.urlopen(url);
    html = page.read();
    return html;

def getImg(html):
    reg = r'src="(.+?\.jpg)" pic_ext'
    imgre = re.compile(reg)
    imglist = re.findall(imgre,html)
    x = 0
    for imgurl in imglist:
        urllib.request.urlretrieve(imgurl,'d:\\pic\\%s.jpg' % x)
        print('d:\\%s.jpg' % x);
        x+=1
        
html = getHtml("http://tieba.baidu.com/p/2460150866");
html = html.decode("utf-8");
print(type(html))
getImg(html);


print("end")

