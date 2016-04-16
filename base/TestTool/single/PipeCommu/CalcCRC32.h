#ifndef __MPMAIN_COMMON_CALCCRC32_H__
#define __MPMAIN_COMMON_CALCCRC32_H__

// ��ԭʼ�ļ��㺯������������������������
// �õ��Ľ�����Ǳ�׼��CRC32������ȡ���Ŷ�
// ������ʷԭ�򣬲���ԭ��ʹ�õļ���CRC32�ĺ����������
void CalcCRC32(unsigned long crc,unsigned long cbBuffer,void * pvBuffer,unsigned long * pNewCrc);

// ��׼�ļ���CRC32�ĺ���
void CalcCRC32(unsigned long cbBuffer,void * pvBuffer,unsigned long * pNewCrc);
unsigned long CalcCRC32(unsigned long cbBuffer,void * pvBuffer);
unsigned long CalcCRC32(void * pvBuffer,unsigned long cbBuffer);

// ����һ���ļ��ı�׼��CRC32
// pFilePathΪ�ļ�ȫ·����lDistanceToMove/dwMoveMethodΪ����Ŀ�ʼλ�ã�Ĭ��ֵΪ���������ļ���CRC32
// ��������0����ɹ�
int CalcCRC32(PDWORD pCRC32Out, LPCTSTR pFilePath,long lDistanceToMove=0,DWORD dwMoveMethod=FILE_BEGIN);

#endif	// __MPMAIN_COMMON_CALCCRC32_H__
