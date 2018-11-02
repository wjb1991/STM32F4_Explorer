//==================================================================================================
//| �ļ����� | Mod_GreenLight.cpp
//|--------- |--------------------------------------------------------------------------------------
//| �ļ����� | �̹⴦��
//|--------- |--------------------------------------------------------------------------------------
//| ��Ȩ���� | 
//|----------|--------------------------------------------------------------------------------------
//|  �汾    |  ʱ��       |  ����     | ����
//|--------- |-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2018.11.02  |  wjb      | ���� ����֤�Ƿ�����DMA������ȡLT1867
//==================================================================================================

typedef struct __GreenCalibPoint {
    FP32 f_Volt;
    FP32 f_Grey;
}GreenCalibPoint;

typedef struct __GreenLight {
    /* ��·���ݵ� */
    FP32 f_Volt;
    FP32 f_Grey;
    GreenCalibPoint CalibPoint[10];     /* �궨�� */
}GreenLight;

GreenLight st_Green[10];