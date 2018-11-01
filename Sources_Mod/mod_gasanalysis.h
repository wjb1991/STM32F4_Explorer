#ifndef __MOD_GASANALYSIS_H__
#define __MOD_GASANALYSIS_H__

#include "bsp.h"
#include "app_cfg.h"

#define     DEF_MAX_POINT_NUM   20
#define     DEF_WAVE_BUFF_SIZE  3648

typedef  enum { eOps_AdjZero, eOps_Calibration, eOps_Analysis }GasAnalysisOps_t;    //operation

//�궨��
typedef struct {
    BOOL    b_Use;                        //�Ƿ�ʹ��
    FP32    f_Concentration;               //Ũ��    
    FP32    f_Hi204_4;                     //204.4um ����߶�
    FP32    f_Hi214_8;                     //214.8um ����߶�
    FP32    f_Hi226_0;                     //226.0um ����߶�
}CaliPoint_t;

typedef struct {
    void*           pst_Dev;                 //�������豸

    GasAnalysisOps_t  e_Ops;                 //״̬�� ��ǰ����״̬

    INT16U          uin_Pixels;              //���׳���
    FP64*           plf_Wavelength;          //������ ��Ӧ��������
    FP64*           plf_BkSpectrum;          //��������
    FP64*           plf_Spectrum;            //��ǰ����
    FP64*           plf_AbsSpectrum;         //ȥ�������������
    
    CaliPoint_t*    pst_CaliPointList;       //У׼�㼯��
    uint8_t         uc_CaliPointLenth;       //У׼�㳤�� �Ѿ�����
    uint8_t         uc_CaliPointSize;        //У׼����󳤶�  
    
    FP32            f_RefConcentration;     //����Ũ��
    FP32            f_Concentration_204;    //204���μ����Ũ��
    FP32            f_Concentration_214;    //214���μ����Ũ��    
    FP32            f_Concentration_226;    //226���μ����Ũ��
    
    FP32            f_Hi204_4;
    FP32            f_Hi214_8;
    FP32            f_Hi226_0;
    
    uint8_t         uc_WorkLineOrder;        //��Ͻ���    
    FP32*           pf_a204;                //���ϵ��
    FP32*           pf_a214;                //���ϵ��
    FP32*           pf_a226;                //���ϵ��
}GasAnalysis_t;

extern FP64            alf_BkSpectrum[DEF_WAVE_BUFF_SIZE];
extern FP32            alf_WorkLine[3][4];  
extern CaliPoint_t     ast_CPortBuff[DEF_MAX_POINT_NUM];

extern GasAnalysis_t      GasAnalysis;

void Mod_GasAnalysisInit(GasAnalysis_t* pst_Proc);

void Mod_GasAnalysisGoAnalysis(GasAnalysis_t* pst_Proc);

void Mod_GasAnalysisGoAdjZero(GasAnalysis_t* pst_Proc);

void Mod_GasAnalysisGoCalibration(GasAnalysis_t* pst_Proc);

void Mod_GasAnalysisPoll(GasAnalysis_t* pst_Proc);

void Mod_GasAnalysisAddPoint(GasAnalysis_t* pst_Proc, CaliPoint_t * pst_Point);

void Mod_GasAnalysisMarkWorkLine(GasAnalysis_t* pst_Proc);

/*
void App_WaveProcInit(WaveProc_t* pst_Proc);

void App_WaveOpsGoAnalysis(WaveProc_t* pst_Proc);

void App_WaveOpsGoZero(WaveProc_t* pst_Proc);

int8_t App_WaveOpsGoCalibration(WaveProc_t* pst_Proc);

void App_WaveOpsClarCaliPointList(WaveProc_t* pst_Proc);

void App_WaveOpsAdjZeroPoint(WaveProc_t* pst_Proc);

void App_WaveOpsZero(WaveProc_t* pst_Proc);

void App_WaveOpsCalibrationOnePoint(WaveProc_t* pst_Proc);

void App_WaveOpsAnalysis(WaveProc_t* pst_Proc);

void App_WaveProcessPoll(WaveProc_t* pst_Proc);
*/


#endif //__MOD_GASANALYSIS_H__
