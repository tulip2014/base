#ifndef __MPMAIN_COMMON_CALCCRC32_H__
#define __MPMAIN_COMMON_CALCCRC32_H__

// 最原始的计算函数，可以用来计算多个缓冲区
// 得到的结果不是标准的CRC32，必须取反才对
// 由于历史原因，部分原来使用的计算CRC32的函数就是这个
void CalcCRC32(unsigned long crc,unsigned long cbBuffer,void * pvBuffer,unsigned long * pNewCrc);

// 标准的计算CRC32的函数
void CalcCRC32(unsigned long cbBuffer,void * pvBuffer,unsigned long * pNewCrc);
unsigned long CalcCRC32(unsigned long cbBuffer,void * pvBuffer);
unsigned long CalcCRC32(void * pvBuffer,unsigned long cbBuffer);

// 计算一个文件的标准的CRC32
// pFilePath为文件全路径，lDistanceToMove/dwMoveMethod为计算的开始位置，默认值为计算整个文件的CRC32
// 函数返回0代表成功
int CalcCRC32(PDWORD pCRC32Out, LPCTSTR pFilePath,long lDistanceToMove=0,DWORD dwMoveMethod=FILE_BEGIN);

#endif	// __MPMAIN_COMMON_CALCCRC32_H__
