
#pragma once

int Temp_TreatQueryUserInfo(unsigned __int32 a1, unsigned __int32 a2, unsigned __int32 a3, PPCL_CLIENTINFO a4);
int Temp_Close();
int Temp_CalcSelfClientID(unsigned __int32 a1, unsigned __int32 *a2);
int Temp_TreatSendCommand();
int Temp_TreatOpReturn();
int TryCloseAllThreadInfo1();
int TryCloseAllThreadInfo1Ex();
int ClearThreadInfo(struct tagTHREADINFO *a1, int a2, int a3);
int TryCloseAllThreadInfo2();
int CloseAllThreadInfo();
int ClearGlobalData();
int InitialGlobalData();
int CRC16(unsigned __int8 *a1, unsigned __int32 a2);
void* Temp_malloc(size_t Size);
void* Temp_calloc(size_t NumOfElements, size_t SizeOfElements);
void Temp_free(void *Memory);
void* Temp_realloc(void *Memory, size_t NewSize);
int Temp_SendCommand(unsigned __int32 a1, char a2, unsigned __int32 a3, char a4, char a5, struct tagPCL_BUFFER *a6, struct tagPCL_BUFFER *a7);
unsigned __int64 GetCycleCount();
int CreateSecurityAttributes(_SECURITY_ATTRIBUTES *a1, unsigned __int32 a2);
int FreeSecurityAttributes(struct _SECURITY_ATTRIBUTES *a1);