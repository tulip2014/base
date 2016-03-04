

/************************************************************************/
/* const �÷�
/************************************************************************/

/*
const��C����������һ���Ƚ��µ������������ǳ�֮Ϊ�������η����⼴��������
�Ķ���Ϊ����(immutable)��

1�������������ξֲ�������
����
void func(){
const int a=0;
}

���ȣ������Ȱ�const������ʺ��Բ�������ôa��һ��int���͵ľֲ��Զ�������
���Ǹ��������ʼֵ0��

Ȼ���ٿ�const.

const��Ϊһ�������޶��ʣ���int����ͬ�ĵ�λ��
const int a;
int const a;
�ǵȼ۵ġ����Ǵ˴�����һ��Ҫ���������ף�const���εĶ�����˭����a,��intû
�й�ϵ��const Ҫ���������εĶ���Ϊ���������ɱ��ı䣬���ɱ���ֵ��������Ϊ
��ֵ��l-value)��
������д��Ҳ�Ǵ���ġ�
const int a;
a=0;
����һ���ܳ�����ʹ�÷�ʽ��
const double pi=3.14;
�ڳ���ĺ��������ͼ��pi�ٴθ�ֵ�����޸ľͻ������

Ȼ��һ����΢���ӵ����ӡ�
const int* p;
������ȥ��const ���η��š�
ע�⣬���������ǵȼ۵ġ�
int* p;
int *p;
��ʵ������Ҫ˵���ǣ�*p��int���͡���ô��Ȼ��p����ָ��int��ָ�롣
ͬ��
const int* p;
��ʵ�ȼ���
const int (*p);
int const (*p);
����*p�ǳ�����Ҳ����˵��pָ��������ǳ�����
����
p+=8; //�Ϸ�
*p=3; //�Ƿ���pָ��������ǳ�����

��ô�������һ�������ǳ���ָ���أ���������const�����ܵĿ���p;
int* const p;
const����ֻ��p,��Ȼ�������ε���p,˵��p���ɱ����ġ�Ȼ���constȥ��������
����p��һ��ָ�� int��ʽ������ָ�롣
����
p+=8; //�Ƿ�
*p=3; //�Ϸ�

�ٿ�һ�������ӵ����ӣ�����������ߵ��ۺ�
const int* const p;
˵��p�Լ��ǳ�������pָ��ı���Ҳ�ǳ�����
����
p+=8; //�Ƿ�
*p=3; //�Ƿ�

const ����һ�����þ����������γ�����̬�ַ�����
���磺
const char* name="David";
���û��const,���ǿ��ܻ��ں������������дname[4]='x'��������䣬������
���¶�ֻ���ڴ�����ĸ�ֵ��Ȼ�����������쳣��ֹ������ const,��������
���ڳ��򱻱����ʱ��������������������const�ĺô������߼������ڱ���
�ڱ����֡�

const ������������������
const char s[]="David";
�����������Ƶ����á�

2���ں�������ʱ���β���
����ʵ���е�һ�����ӡ�
NAME
memmove -- copy byte string

LIBRARY
Standard C Library (libc, -lc)

SYNOPSIS
#include <string.h>

void *
memmove(void *dst, const void *src, size_t len);

���Ǳ�׼���е�һ�����������ڰ��ֽڷ�ʽ�����ַ������ڴ棩��
���ĵ�һ���������ǽ��ַ������Ƶ�����ȥ��dest),��Ŀ�ĵأ�����ڴ��������
�ǿ�д��
���ĵڶ�����������Ҫ��ʲô�����ַ������Ƴ�ȥ�����Ƕ�����ڴ�����ֻ����
ȡ����д��
���ǣ�����վ����������Լ��ĽǶ�������src ���ָ�룬����ָ����ڴ�������
������������������ִ�еĹ������ǲ��䡣����src��ָ��������ǳ��������Ǿ�
��Ҫ��const���Ρ�
���磬������������ʹ������
const char* s="hello";
char buf[100];
memmove(buf,s,6); //������ʵӦ����strcpy��memcpy����

������Ƿ�����д��
memmove(s,buf,6);
��ô������һ���ᱨ������ʵ�����Ǿ�����Ѹ��ֺ����Ĳ���˳��д������ʵ�Ǳ�
�����ڴ�ʱ�������Ǵ�æ����������������ĵĲ�������(�ں���������ȥ��
const����),��ô������������е�ʱ��һ���������

���ﻹҪ˵����һ�����ں�������������constһ����������ָ������Ǳ���������
���磬�����size_t len,�ں���ʵ�ֵ�ʱ�������ȫ���ø���len��ֵ����ô�Ƿ�
Ӧ�ð�lenҲ����Ϊ�����أ����ԣ�������ô����������������ô����ʲô���ӡ�
�������const,��ô�������������ʵ���ߣ����Է�ֹ����ʵ�����������ʱ����
�Ĳ���Ҫ�޸ĵ�ֵ(len),�����ܺá�
���Ƕ������������ʹ���ߣ�
1��������η��ź������壬���ǿ��Դ���һ��������������һ���ǳ���������
ȥ�������Է���õ�ֻ�����Ǵ��ݵ�һ��copy��
2����¶��ʵ�֡��Ҳ���Ҫ֪������ʵ�����������ʱ���Ƿ��޸Ĺ�len��ֵ��

���ԣ�constһ��ֻ��������ָ�롣

�ٿ�һ�����ӵ�����
int execv(const char *path, char *const argv[]);
���ؿ����������argv.������ʲô��
���ȥ��const,���ǿ��Կ���
char * argv[];
argv��һ�����飬����ÿ��Ԫ�ض���char *���͵�ָ�롣
�������const.��ôconst���ε���˭�أ������ε���һ�����飬argv[],��˼����
˵��������Ԫ����ֻ���ġ���ô�����Ԫ�ص���ʲô�����أ���char *���͵�ָ
��.Ҳ����˵ָ���ǳ���������ָ������ݲ��ǡ�
����
argv[1]=NULL; //�Ƿ�
argv[0][0]='a'; //�Ϸ�


3��ȫ�ֱ�����
���ǵ�ԭ����Ȼ�ǣ��������ٵ�ʹ��ȫ�ֱ�����
���ǵĵڶ������� ���ǣ������ܶ��ʹ��const��
���һ��ȫ�ֱ���ֻ�ڱ��ļ���ʹ�ã���ô�÷���ǰ����˵�ĺ����ֲ�����û��ʲ
ô����
�����Ҫ�ڶ���ļ��乲������ô��ǣ����һ���洢���͵����⡣

�����ַ�ʽ��
1.ʹ��extern
����
// file1.h
extern const double pi;
// file1.c 
const double pi=3.14;
Ȼ��������Ҫʹ��pi��������ģ�����file1.h
#include "file1.h"
	���ߣ��Լ����Ǿ���������һ��ͺá�
	�������Ľ���ǣ����������������������Ҫʹ��pi��������Ĺ���һ���洢����

	2.ʹ��static,��̬�ⲿ�洢��
	// constant.h 
	static const pi=3.14;
��Ҫʹ�����������*.c�ļ��У�����������ͷ�ļ���
	ǰ���staticһ�������١��������ӵ�ʱ��ᱨ��˵�ñ�������ζ��塣
	�������Ľ���ǣ�ÿ��������constant.h��*.c�ļ�������һ�ݸñ����Լ���copy,
	�ñ���ʵ���ϻ��Ǳ������˶�Σ�ռ���˶���洢�ռ䣬�����ڼ���static�ؼ���
	�󣬽�����ļ����ض���ĳ�ͻ��
	�������˷��˴洢�ռ䣬�����������Ŀ�ִ���ļ���󡣵���ͨ���������СС
	���ֽڵı仯���������⡣
	�ô��ǣ��㲻�ù���������������ĸ��ļ��б���ʼ���ġ�


	���˵˵const�����á�
	const �ĺô����������˳����ĸ�������ǲ�Ҫȥ�޸Ĳ����޸ĵ��ڴ档ֱ�ӵ�
	���þ����ø�����߼������ڱ����ڱ����֡���������Ҫ�����ܵĶ�ʹ��const��
	���Ǻܶ��˲���ϰ��ʹ�������������ߣ������������� ��д������ ���Ų�
	const������Ǹ�������������const,�кá�����Ǹ� ȫ�֣��ֲ�������const,��
	ô������ô��Ϊʱ�������޷����ô��뿴������Ư���ˡ�
*/