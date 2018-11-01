#include "mod_gasanalysis.h"
#include "Pub_Nihe.h"

#pragma location = (0x68020000)
FP64        alf_BkSpectrum[DEF_WAVE_BUFF_SIZE] = {0};
#pragma location = (0x68028000)
FP64        alf_Spectrum[DEF_WAVE_BUFF_SIZE] = {0};
#pragma location = (0x68030000)
FP64        alf_AbsSpectrum[DEF_WAVE_BUFF_SIZE] = {0};

FP32            alf_WorkLine[3][4]  = { 0.0 };
CaliPoint_t     ast_CPortBuff[DEF_MAX_POINT_NUM] = {0};

GasAnalysis_t      GasAnalysis;

void App_WaveProcInit(GasAnalysis_t* pst_Proc);
void Mod_GasAnalysisGoAnalysis(GasAnalysis_t* pst_Proc);
void Mod_GasAnalysisGoAdjZero(GasAnalysis_t* pst_Proc);
void Mod_GasAnalysisGoCalibration(GasAnalysis_t* pst_Proc);

//==================================================================================================
//| �������� | Mod_SpectrumRangeAvg
//|----------|--------------------------------------------------------------------------------------
//| �������� | �������������м���Щֵ��ƽ��
//|----------|--------------------------------------------------------------------------------------
//| ������� | pst_Proc:���,uin_Left:������� ,uin_Right:�ұ�����
//|----------|--------------------------------------------------------------------------------------       
//| ���ز��� | ��
//|----------|-------------------------------------------------------------------------------------- 
//| ������� | wjb
//==================================================================================================
FP64 Mod_SpectrumRangeAvg(GasAnalysis_t* pst_Proc, INT16U uin_Left, INT16U uin_Right)
{
    INT16U i;
    FP64 sum = 0;
    for(i = uin_Left; i<uin_Right; i++)
    {
        sum += pst_Proc->plf_Spectrum[i];
    }
    return (FP64)(sum/(uin_Left-uin_Right));
}
//==================================================================================================
//| �������� | Mod_GetWaveLenthIndex
//|----------|--------------------------------------------------------------------------------------
//| �������� | ���ݲ����ҳ���Ӧ������ֵ
//|----------|--------------------------------------------------------------------------------------
//| ������� | pst_Proc:���
//|----------|--------------------------------------------------------------------------------------       
//| ���ز��� | 0-3840����ֵ -1û��������
//|----------|-------------------------------------------------------------------------------------- 
//| ������� | wjb
//==================================================================================================
INT16S Mod_GetWaveLenthIndex(GasAnalysis_t * pst_Proc, double lf_WaveLenth)
{
    if( pst_Proc == NULL || pst_Proc->plf_Wavelength == NULL )
        return -1;
  
    for(int i = 0; i < pst_Proc->uin_Pixels; i++ )
    {
        if( pst_Proc->plf_Wavelength[i] >= lf_WaveLenth )
            return i;
    }
    return -1;
}

//==================================================================================================
//| �������� | Mod_GasAnalysisInit
//|----------|--------------------------------------------------------------------------------------
//| �������� | ��ʼ���������ģ��
//|----------|--------------------------------------------------------------------------------------
//| ������� | pst_Proc:���
//|----------|--------------------------------------------------------------------------------------       
//| ���ز��� | ��
//|----------|-------------------------------------------------------------------------------------- 
//| ������� | wjb
//==================================================================================================
void Mod_GasAnalysisInit(GasAnalysis_t* pst_Proc)
{
    pst_Proc->plf_BkSpectrum = alf_BkSpectrum;
    pst_Proc->plf_Spectrum = alf_Spectrum;
    pst_Proc->plf_AbsSpectrum = alf_AbsSpectrum;
    
    pst_Proc->pst_CaliPointList = ast_CPortBuff;
    pst_Proc->uc_CaliPointLenth = 0;
    pst_Proc->uc_CaliPointSize = DEF_MAX_POINT_NUM;
    pst_Proc->f_RefConcentration = 0.0;
    pst_Proc->uc_WorkLineOrder = 2;
    pst_Proc->pf_a204 = alf_WorkLine[0];
    pst_Proc->pf_a214 = alf_WorkLine[1];
    pst_Proc->pf_a226 = alf_WorkLine[2];
    
    //Mod_GasAnalysisGoAdjZero(pst_Proc);
    Mod_GasAnalysisGoAnalysis(pst_Proc);
}

//==================================================================================================
//| �������� | Mod_GasAnalysisGoAnalysis
//|----------|--------------------------------------------------------------------------------------
//| �������� | ת�����ģʽ
//|----------|--------------------------------------------------------------------------------------
//| ������� | pst_Proc:���
//|----------|--------------------------------------------------------------------------------------       
//| ���ز��� | ��
//|----------|-------------------------------------------------------------------------------------- 
//| ������� | wjb
//==================================================================================================
void Mod_GasAnalysisGoAnalysis(GasAnalysis_t* pst_Proc)
{
    TRACE_DBG(">>DBG:       ת�����ģʽ\n\r");
    pst_Proc->e_Ops = eOps_Analysis;
}

//==================================================================================================
//| �������� | Mod_GasAnalysisGoAdjZero
//|----------|--------------------------------------------------------------------------------------
//| �������� | ת�����ģʽ
//|----------|--------------------------------------------------------------------------------------
//| ������� | pst_Proc:���
//|----------|--------------------------------------------------------------------------------------       
//| ���ز��� | ��
//|----------|-------------------------------------------------------------------------------------- 
//| ������� | wjb
//==================================================================================================
void Mod_GasAnalysisGoAdjZero(GasAnalysis_t* pst_Proc)
{
    TRACE_DBG(">>DBG:       ת�����ģʽ\n\r");
    pst_Proc->e_Ops = eOps_AdjZero;
}

//==================================================================================================
//| �������� | App_WaveOpsGoCalibration
//|----------|--------------------------------------------------------------------------------------
//| �������� | ת��У׼ģʽ
//|----------|--------------------------------------------------------------------------------------
//| ������� | pst_Proc:���
//|----------|--------------------------------------------------------------------------------------       
//| ���ز��� | ��
//|----------|-------------------------------------------------------------------------------------- 
//| ������� | wjb
//==================================================================================================
void Mod_GasAnalysisGoCalibration(GasAnalysis_t* pst_Proc)
{
    TRACE_DBG(">>DBG:       ת��궨ģʽ\n\r");
    pst_Proc->e_Ops = eOps_Calibration;
}

//==================================================================================================
//| �������� | Mod_GasAnalysissClarCaliPointList
//|----------|--------------------------------------------------------------------------------------
//| �������� | ������б궨��
//|----------|--------------------------------------------------------------------------------------
//| ������� | pst_Proc:���
//|----------|--------------------------------------------------------------------------------------       
//| ���ز��� | ��
//|----------|-------------------------------------------------------------------------------------- 
//| ������� | wjb
//==================================================================================================
void Mod_GasAnalysissClarCaliPointList(GasAnalysis_t* pst_Proc)
{
    
    CaliPoint_t* p = pst_Proc->pst_CaliPointList;
    uint8_t i =0;
    /* ȡ��ʹ�õı궨������� */
    for(i = 0; i < DEF_MAX_POINT_NUM; i ++)
    {
        p[i].b_Use = FALSE;
    }
    
}

//==================================================================================================
//| �������� | Mod_GasAnalysisMarkWorkLine
//|----------|--------------------------------------------------------------------------------------
//| �������� | ����������
//|----------|--------------------------------------------------------------------------------------
//| ������� | pst_Proc:���
//|----------|--------------------------------------------------------------------------------------       
//| ���ز��� | ��
//|----------|-------------------------------------------------------------------------------------- 
//| ������� | wjb
//==================================================================================================
void Mod_GasAnalysisMarkWorkLine(GasAnalysis_t* pst_Proc)
{
    uint16_t i,j;
    for(i = 0; i < pst_Proc->uc_WorkLineOrder; i++)
    {
        pst_Proc->pf_a204[0] = 0.0;
        pst_Proc->pf_a214[1] = 0.0;
        pst_Proc->pf_a226[2] = 0.0;
    }
    
    //������Ϻ��� ����������߲���¼
    float x204[DEF_MAX_POINT_NUM];     //��ֵ
    float x214[DEF_MAX_POINT_NUM];     //��ֵ
    float x226[DEF_MAX_POINT_NUM];     //��ֵ
    float y[DEF_MAX_POINT_NUM];        //Ũ��
    
    CaliPoint_t* p = pst_Proc->pst_CaliPointList;
    uint8_t uc_PointNum =0;
    
    /* ȡ��ʹ�õı궨������� */
    for(i = 0; i < DEF_MAX_POINT_NUM; i ++)
    {
        if(p[i].b_Use == TRUE)
        {
            y[uc_PointNum] = p[i].f_Concentration;
            x204[uc_PointNum] = p[i].f_Hi204_4;
            x214[uc_PointNum] = p[i].f_Hi214_8;
            x226[uc_PointNum] = p[i].f_Hi226_0;
            uc_PointNum++;
        }
    }
    
    /* �����ݰ�Ũ�ȴ�С�������� */
    for (j = 0; j < uc_PointNum - 1; j++)
    {
        for(i = j; i < uc_PointNum - 1 ; i++)
        {
            if(y[i] > y[i+1])
            {
                float t;
                t =  y[i+1];
                y[i+1] = y[i];
                y[i] = t;
                
                t =  x204[i+1];
                x204[i+1] = x204[i];
                x204[i] = t;
                
                t =  x214[i+1];
                x214[i+1] = x214[i];
                x214[i] = t;
                
                t =  x226[i+1];
                x226[i+1] = x226[i];
                x226[i] = t;
            }
        }    
    }
    
    /* �����������ֵ��Ӧ��ϵ�� */
    NiHe1(x204,y,uc_PointNum, pst_Proc->pf_a204,pst_Proc->uc_WorkLineOrder);
    NiHe1(x214,y,uc_PointNum, pst_Proc->pf_a214,pst_Proc->uc_WorkLineOrder);
    NiHe1(x226,y,uc_PointNum, pst_Proc->pf_a226,pst_Proc->uc_WorkLineOrder);
    
    /* �洢�������ϵ����EEPROM */
    for(i = 0; i < 3 ; i++)
    {
        SaveToEeprom((INT32U)(&pst_Proc->pf_a204[i]));
        SaveToEeprom((INT32U)(&pst_Proc->pf_a214[i])); 
        SaveToEeprom((INT32U)(&pst_Proc->pf_a226[i])); 
    }
}

//==================================================================================================
//| �������� | Mod_GasAnalysisAddPoint
//|----------|--------------------------------------------------------------------------------------
//| �������� | ���һ���궨��
//|----------|--------------------------------------------------------------------------------------
//| ������� | pst_Proc:���,pst_Point:��ӵı궨�������Ӧ��
//|----------|--------------------------------------------------------------------------------------       
//| ���ز��� | ��
//|----------|-------------------------------------------------------------------------------------- 
//| ������� | wjb
//==================================================================================================
void Mod_GasAnalysisAddPoint(GasAnalysis_t* pst_Proc, CaliPoint_t * pst_Point)
{
    CaliPoint_t* p = pst_Proc->pst_CaliPointList;
    uint8_t size = pst_Proc->uc_CaliPointSize;
    uint8_t i = 0;
    for(i = 0; i< size; i++)
    {
        if(p[i].b_Use == FALSE)
        {
            TRACE_DBG(">>DBG:       ���һ���궨�� ��д��EEPROM \n\r");
            p[i].b_Use = TRUE;
            p[i].f_Hi204_4 = pst_Point->f_Hi204_4;
            p[i].f_Hi214_8 = pst_Point->f_Hi214_8;
            p[i].f_Hi226_0 = pst_Point->f_Hi226_0;
            p[i].f_Concentration = pst_Point->f_Concentration;
            
            SaveToEepromExt((uint32_t)(&p[i]),sizeof(CaliPoint_t));
            break;
        }
    }
}

//==================================================================================================
//| �������� | Mod_GasAnalysissDeletePoint
//|----------|--------------------------------------------------------------------------------------
//| �������� | ɾ��һ���궨��
//|----------|--------------------------------------------------------------------------------------
//| ������� | pst_Proc:���,uch_Index:��Ҫɾ���ı궨�������
//|----------|--------------------------------------------------------------------------------------       
//| ���ز��� | ��
//|----------|-------------------------------------------------------------------------------------- 
//| ������� | wjb
//==================================================================================================
void App_WaveDeletePoint(GasAnalysis_t* pst_Proc,  uint8_t uch_Index)
{
    CaliPoint_t* p = pst_Proc->pst_CaliPointList;
    uint8_t i = 0;
    p[i].b_Use = FALSE;
    SaveToEepromExt((uint32_t)(&p[i]),sizeof(CaliPoint_t));
}

//==================================================================================================
//| �������� | Mod_GasAnalysissReadPoint
//|----------|--------------------------------------------------------------------------------------
//| �������� | ��ȡһ���궨��
//|----------|--------------------------------------------------------------------------------------
//| ������� | pst_Proc:���,uch_Index:��Ҫ��ȡ�ı궨������� ,pst_Point:���ر궨�������
//|----------|--------------------------------------------------------------------------------------       
//| ���ز��� | ��
//|----------|-------------------------------------------------------------------------------------- 
//| ������� | wjb
//==================================================================================================
void App_WaveReadPoint(GasAnalysis_t* pst_Proc,  uint8_t uch_Index ,CaliPoint_t * pst_Point)
{
    CaliPoint_t* p = pst_Proc->pst_CaliPointList;
    //uint8_t size = pst_Proc->uc_CaliPointSize;
    uint8_t i = uch_Index;
    
    pst_Point->b_Use =  p[i].b_Use;
    pst_Point->f_Hi204_4 = p[i].f_Hi204_4;
    pst_Point->f_Hi214_8 = p[i].f_Hi214_8;
    pst_Point->f_Hi226_0 = p[i].f_Hi226_0;
    pst_Point->f_Concentration = p[i].f_Concentration;
}


//==================================================================================================
//| �������� | Mod_GetPeakHigh
//|----------|--------------------------------------------------------------------------------------
//| �������� | ���������շ�ĸ߶�
//|----------|--------------------------------------------------------------------------------------
//| ������� | pst_Proc:���,lf_CenterWaveLenth: ���Ĳ���,lf_Width:������Χ
//|----------|--------------------------------------------------------------------------------------       
//| ���ز��� | ��
//|----------|-------------------------------------------------------------------------------------- 
//| ������� | wjb
//==================================================================================================
FP64 Mod_GetPeakHigh(GasAnalysis_t* pst_Proc, FP64 lf_CenterWaveLenth, FP64 lf_Width)
{
#if 0
    /* ������͵� �����շ�߶� */
    INT16S i_InedexCenter = 0;
    INT16S i_InedexLeft = 0;
    INT16S i_InedexRight = 0;
    INT16S i_Width = 0;
    INT16U i = 0;
    FP64   lf_min = 0;
    
    FP64   lf_HiLeft = 0;
    FP64   lf_HiRight = 0;
    FP64   lf_HiCenter = 0;
    
    /* ��ȡ���Ĳ�����+-1um������ */
    i_InedexLeft = Mod_GetWaveLenthIndex(pst_Proc,lf_CenterWaveLenth - lf_Width); 
    i_InedexRight = Mod_GetWaveLenthIndex(pst_Proc,lf_CenterWaveLenth + lf_Width); 
    i_Width = (i_InedexRight - i_InedexLeft)/2;
    
    /* ��+-1um��Χ��������͵�*/
    for(i = i_InedexLeft; i<=i_InedexRight; i++)
    {
        if(lf_min > pst_Proc->plf_AbsSpectrum[i])
        {
            i_InedexCenter = i;
            lf_min = pst_Proc->plf_AbsSpectrum[i];
        }
    }
    
    i_InedexRight = i_InedexCenter - i_Width/2;
    i_InedexLeft = i_InedexRight - i_Width;
    
    lf_HiLeft = Mod_SpectrumRangeAvg(pst_Proc, i_InedexLeft,i_InedexRight);
    
    i_InedexLeft = i_InedexRight;
    i_InedexRight = i_InedexLeft + i_Width;
    lf_HiCenter = Mod_SpectrumRangeAvg(pst_Proc, i_InedexLeft,i_InedexRight);
    
    
    i_InedexLeft = i_InedexRight;
    i_InedexRight = i_InedexLeft + i_Width;
    lf_HiRight = Mod_SpectrumRangeAvg(pst_Proc, i_InedexLeft,i_InedexRight);
    
    return (lf_HiCenter - ((lf_HiLeft+lf_HiRight)/2));
    
#else
    /* ֱ��ͨ�������Ҳ���*/
    uint16_t i = 0; 
    int16_t i_InedexLeft = 0;
    int16_t i_InedexRight = 0;
    double lf_BkgLeft = 0;
    double lf_BkgRight = 0;
    double lf_Center = 0;
    double lf_WaveLenthWidth = 2.0;
      
    //������  
    i_InedexLeft = Mod_GetWaveLenthIndex(pst_Proc,lf_CenterWaveLenth - 1.5*lf_WaveLenthWidth);  
    i_InedexRight = Mod_GetWaveLenthIndex(pst_Proc,lf_CenterWaveLenth - 0.5*lf_WaveLenthWidth);
    for(i = i_InedexLeft; i < i_InedexRight; i++)
    {
        lf_BkgLeft += (pst_Proc->plf_AbsSpectrum[i]);
    }
    lf_BkgLeft /= (i_InedexLeft - i_InedexRight);
    
    //�����
    i_InedexLeft = Mod_GetWaveLenthIndex(pst_Proc,lf_CenterWaveLenth - 0.5*lf_WaveLenthWidth);
    i_InedexRight = Mod_GetWaveLenthIndex(pst_Proc,lf_CenterWaveLenth + 0.5*lf_WaveLenthWidth);
    for(i = i_InedexLeft; i < i_InedexRight; i++)
    {
        lf_Center += (double)(pst_Proc->plf_AbsSpectrum[i]);
    }
    lf_Center /= (i_InedexLeft - i_InedexRight);
    
    //������
    i_InedexLeft = Mod_GetWaveLenthIndex(pst_Proc,lf_CenterWaveLenth + 0.5*lf_WaveLenthWidth);
    i_InedexRight = Mod_GetWaveLenthIndex(pst_Proc,lf_CenterWaveLenth + 1.5*lf_WaveLenthWidth);
    for(i = i_InedexLeft; i < i_InedexRight; i++)
    {
        lf_BkgRight += (double)(pst_Proc->plf_AbsSpectrum[i]);
    }
    lf_BkgRight /= (i_InedexLeft - i_InedexRight);
    
    
    return (lf_Center - (lf_BkgLeft+lf_BkgRight)/2);
#endif
}
//==================================================================================================
//| �������� | Mod_GasAnalysisAdjZero
//|----------|--------------------------------------------------------------------------------------
//| �������� | ���㹤��ģʽ ���±�����
//|----------|--------------------------------------------------------------------------------------
//| ������� | pst_Proc:���
//|----------|--------------------------------------------------------------------------------------       
//| ���ز��� | ��
//|----------|-------------------------------------------------------------------------------------- 
//| ������� | wjb
//==================================================================================================
static void Mod_GasAnalysisAdjZero(GasAnalysis_t* pst_Proc)
{
    USB4000_HandleTypeDef* USB4000_Handle =  (USB4000_HandleTypeDef *) pst_Proc->pst_Dev;

    for(int i = 0; i < pst_Proc->uin_Pixels; i++)
    {
        pst_Proc->plf_BkSpectrum[i] = USB4000_Handle->plf_ProcessSpectrum[i];
        pst_Proc->plf_Spectrum[i] = USB4000_Handle->plf_ProcessSpectrum[i];
        pst_Proc->plf_AbsSpectrum[i] = 0;
    }
    
    SaveToEepromExt((uint32_t)pst_Proc->plf_BkSpectrum,pst_Proc->uin_Pixels);      
    Mod_GasAnalysisGoAnalysis(pst_Proc);
    
    TRACE_DBG(">>DBG:       �����������\n\r");
}

//==================================================================================================
//| �������� | Mod_GasAnalysisCalibrationOnePoint
//|----------|--------------------------------------------------------------------------------------
//| �������� | �궨ģʽ���� �궨��ɺ��˳�������ģʽ
//|----------|--------------------------------------------------------------------------------------
//| ������� | pst_Proc:���
//|----------|--------------------------------------------------------------------------------------       
//| ���ز��� | ��
//|----------|-------------------------------------------------------------------------------------- 
//| ������� | wjb
//==================================================================================================
static void Mod_GasAnalysisCalibrationOnePoint(GasAnalysis_t* pst_Proc)
{
    USB4000_HandleTypeDef* USB4000_Handle =  (USB4000_HandleTypeDef *) pst_Proc->pst_Dev;
    
    for(int i = 0; i < pst_Proc->uin_Pixels; i++)
    {
        pst_Proc->plf_Spectrum[i] = USB4000_Handle->plf_ProcessSpectrum[i];
        pst_Proc->plf_AbsSpectrum[i] = pst_Proc->plf_Spectrum[i] - pst_Proc->plf_BkSpectrum[i];
    }
    
    if( pst_Proc->uc_CaliPointLenth < pst_Proc->uc_CaliPointSize )
    {
        //��¼һ���ο��� 
        CaliPoint_t CaliPoint;
        
        TRACE_DBG(">>DBG:       �궨��� д��EEPROM \n\r");
        
        CaliPoint.b_Use = TRUE;
        CaliPoint.f_Hi204_4 = Mod_GetPeakHigh(pst_Proc, 204.4,2.0);
        CaliPoint.f_Hi214_8 = Mod_GetPeakHigh(pst_Proc, 214.8,2.0);
        CaliPoint.f_Hi226_0 = Mod_GetPeakHigh(pst_Proc, 226.0,2.0);
        CaliPoint.f_Concentration = pst_Proc->f_RefConcentration;
        
        pst_Proc->f_Hi204_4 = CaliPoint.f_Hi204_4;
        pst_Proc->f_Hi214_8 = CaliPoint.f_Hi214_8;
        pst_Proc->f_Hi226_0 = CaliPoint.f_Hi226_0;
        
        Mod_GasAnalysisAddPoint(pst_Proc,&CaliPoint);
        
        Mod_GasAnalysisGoAnalysis(pst_Proc);
    }
}

//==================================================================================================
//| �������� | Mod_GasAnalysisAnalysis
//|----------|--------------------------------------------------------------------------------------
//| �������� | ����ģʽ
//|----------|--------------------------------------------------------------------------------------
//| ������� | pst_Proc:���
//|----------|--------------------------------------------------------------------------------------       
//| ���ز��� | ��
//|----------|-------------------------------------------------------------------------------------- 
//| ������� | wjb
//==================================================================================================
static void Mod_GasAnalysisAnalysis(GasAnalysis_t* pst_Proc)
{
    USB4000_HandleTypeDef* USB4000_Handle =  (USB4000_HandleTypeDef *) pst_Proc->pst_Dev;
    for(int i = 0; i < pst_Proc->uin_Pixels; i++)
    {
        pst_Proc->plf_Spectrum[i] = USB4000_Handle->plf_ProcessSpectrum[i];
        pst_Proc->plf_AbsSpectrum[i] = pst_Proc->plf_Spectrum[i] - pst_Proc->plf_BkSpectrum[i];
    }
        
    double f_Hi204_4 = Mod_GetPeakHigh(pst_Proc,204.4,2.0);
    double f_Hi214_8 = Mod_GetPeakHigh(pst_Proc,214.8,2.0);
    double f_Hi226_0 = Mod_GetPeakHigh(pst_Proc,226.0,2.0);
    
    pst_Proc->f_Hi204_4 = f_Hi204_4;
    pst_Proc->f_Hi214_8 = f_Hi214_8;
    pst_Proc->f_Hi226_0 = f_Hi226_0;
    
    
    //������Ϲ�ʽ����Ũ��
    pst_Proc->f_Concentration_204 = s_fx(pst_Proc->pf_a204,pst_Proc->uc_WorkLineOrder,(FP32)f_Hi204_4);
    pst_Proc->f_Concentration_214 = s_fx(pst_Proc->pf_a214,pst_Proc->uc_WorkLineOrder,(FP32)f_Hi214_8);
    pst_Proc->f_Concentration_226 = s_fx(pst_Proc->pf_a226,pst_Proc->uc_WorkLineOrder,(FP32)f_Hi226_0);
    
    TRACE_DBG(">>DBG:       ����Ũ�����\n\r");
}

//==================================================================================================
//| �������� | Mod_GasAnalysisPoll
//|----------|--------------------------------------------------------------------------------------
//| �������� | �������������ѯ
//|----------|--------------------------------------------------------------------------------------
//| ������� | ��
//|----------|--------------------------------------------------------------------------------------       
//| ���ز��� | ��
//|----------|-------------------------------------------------------------------------------------- 
//| ������� | wjb
//==================================================================================================
void Mod_GasAnalysisPoll(GasAnalysis_t* pst_Proc)
{
    USB4000_HandleTypeDef* USB4000_Handle =  (USB4000_HandleTypeDef *) pst_Proc->pst_Dev;
    
    pst_Proc->plf_Wavelength = USB4000_Handle->plf_WaveLenth;
    pst_Proc->uin_Pixels = USB4000_Handle->uin_Pixels;
    
    switch(pst_Proc->e_Ops)
    {
    case eOps_AdjZero:
        Mod_GasAnalysisAdjZero(pst_Proc);
        break;
            
    case eOps_Calibration:
        Mod_GasAnalysisCalibrationOnePoint(pst_Proc);
        break;
        
    case eOps_Analysis:
        Mod_GasAnalysisAnalysis(pst_Proc);
        break;
    default:
        Mod_GasAnalysisAnalysis(pst_Proc);
        break;
    }

}
