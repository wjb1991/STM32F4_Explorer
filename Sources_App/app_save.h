#ifndef SAVE_H
#define SAVE_H

#include "bsp.h"
#include "app_cfg.h"

#define  START_ADD    (INT32U)0x10 //��ʼ��ַ
//========================= Macro,Typestruct, & Class =====================//
#define N_MAX_EEPROM_ARR  1024 //���EEPROM������洢����

#define MW1  1  //INT8U ռ��1���ֽڵ���С�洢��Ԫ
#define MW2  2  //INT16Uռ��2���ֽڵ���С�洢��Ԫ
#define MW4  4  //INT16Uռ��4���ֽڵ���С�洢��Ԫ
#define MW8  8  //INT16Uռ��4���ֽڵ���С�洢��Ԫ

//12�ֻ�������������
#define  KIND_INT8U   0x01
#define  KIND_INT8S   0x02
#define  KIND_INT16U  0x03
#define  KIND_INT16S  0x04
#define  KIND_INT32U  0x05
#define  KIND_INT32S  0x06
#define  KIND_FP32    0x07
#define  KIND_FP64    0x08
#define  KIND_STRING  0x09
#define  KIND_COMBOX  0x0A
#define  KIND_BOOL    0x0B
#define  KIND_STRUCT  0x0C


//�޶���ʽ����
#define  LIMIT_NULL   0x00 
#define  LIMIT_RANGE  0x01 
//��ʼ����ʽ
#define  INIT_NORMAL  0x00
#define  INIT_DEFAULT 0x01

 
//Eeprom�洢����
typedef struct
{
    INT32U  ul_SaveAddr;         //EEprom�洢��ַ
	INT32U  ul_VariableAddr;     //������ַ
	INT8U   uch_TypeFlag;        //��������
	INT16U  uin_Len;             //���ݳ���
    INT8U   uch_LimitFlag;       //�޶���ʽ
    FP32    f_Min;               //�޶���Сֵ
    FP32    f_Max;               //�޶����ֵ
	FP32    f_Default;           //Ĭ��ֵ
}MYSAVEPARA;      

//========================= Variable,Object & Fuction =====================//
void    InitParaFromEeprom(INT8U uch_InitFlag);
BOOL SaveToEeprom(INT32U ul_VariableAddr);
BOOL SaveToEepromExt(INT32U ul_VariableAddr,INT16U uin_Len);

void   WriteCharToEeprom(INT32U uin_Addr,INT8U uch_Data);
void   WriteIntToEeprom(INT32U uin_Addr,INT16U uin_Data);
void   WriteLongToEeprom(INT32U uin_Addr,INT32U ul_Data);
void   WriteFloatToEeprom(INT32U uin_Addr,FP32 f_Data);
void   WriteDoubleToEeprom(INT32U uin_Addr,FP64 f_Data);

INT8U  ReadCharFromEeprom(INT32U uin_Addr);
INT16U ReadIntFromEeprom(INT32U uin_Addr);
INT32U ReadLongFromEeprom(INT32U uin_Addr);
FP32   ReadFloatFromEeprom(INT32U uin_Addr);
FP64   ReadDoubleFromEeprom(INT32U uin_Addr);

#endif
