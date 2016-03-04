import smtplib
import email.mime.multipart
import email.mime.text

msg=email.mime.multipart.MIMEMultipart()
msg['from']='youjiehaoo@163.com'
msg['to']='363234806@qq.com'
msg['subject']='测试'
content='''
	一下，tmd


'''
txt=email.mime.text.MIMEText(content)
msg.attach(txt)

smtp=smtplib
smtp=smtplib.SMTP()
smtp.connect('smtp.163.com','25')
smtp.login('youjiehaoo@163.com','s11111111')
smtp.sendmail('youjiehaoo@163.com','363234806@qq.com',str(msg))
smtp.quit()
