//VAR_INPUT
#ifndef TRUE
  #define TRUE 0x01
#endif 
#ifndef FALSE
  #define FALSE 0x00
#endif 
float    PV_IN = 0.0 ;   //���̱�������
//�����ڡ����̱������롱��������ó�ʼֵ������Ҳ�������ӵ���������ʽ���ⲿ���̱����ϡ�
int      PV_PER  = 0 ;      //��Χ�豸���̱��� ��Χ�豸I/O ��ʽ�Ĺ��̱������ӵ��������ġ���Χ�豸���̱���������ˡ�
float    DISV = 0.0 ;   //���ű��� ����ǰ�����ƣ����ű������ӵ������ű���������ˡ�
unsigned char    INT_HPOS = FALSE ; //�������ñ�����������	�������õ��������������������Ϊ�ˣ�INT_HPOS ����˱������ó�TRUE���ڼ��������У�����������INT_HPOS �������μ���������QLMN_HLM��
unsigned char    INT_HNEG = FALSE ; //�������ñ����ڸ�����
//�������õ�������������ڸ�����Ϊ�ˣ�INT_HNEG ����˱������ó�TRUE���ڼ��������У�����������INT_HNEG �������μ���������QLMN_LLM��
int    SELECT = 0 ;      //����PID �����巢������ѡ��
//������������巢���������м��ַ��������ڵ���PID �㷨�����巢��//����
//. SELECT =0����һ�������������жϼ��е��ÿ�������������PID�㷨�����巢������
//. SELECT =1����OB1 �е��ÿ�����������ֻ����PID �㷨��
//. SELECT =2����һ�������������жϼ��е��ÿ���������ֻ�������巢������
//. SELECT =3����һ�������������жϼ��е��ÿ���������ֻ����PID �㷨��
//END_VAR
//VAR_OUTPUT
float    PV = 0.0 ;   //���̱���   ��Ч�Ĺ��̱����ڡ����̱���������������
float    LMN = 0.0 ;   //�ɵ��ڱ���	�ɵ��ڱ�������Чֵ�Ը�������ʽ�� �ڡ��ɵ��ڱ���������������
int    LMN_PER = 0 ;      //��Χ�豸�����ɵ��ڱ��� ��Χ�豸��ʽ�Ŀɵ��ڱ���ֵ���ӵ��������ġ���Χ�豸�����ɵ��ڱ���������ˡ�
unsigned char    QPULSE = FALSE ; //��������ź� �ɵ��ڱ�����ֵ����QPULSE ����˵��Ƶ���������ȡ�
unsigned char    QLMN_HLM = FALSE ; //�ﵽ�ɵ��ڱ��������� �ɵ��ڱ�����ֵʼ�����������޺����޷�Χ֮�ڡ�������������ʱ��ͨ��QLMN_HLM ����˽���ָʾ��
unsigned char    QLMN_LLM = FALSE ; //�ﵽ�ɵ��ڱ��������� �ɵ��ڱ�����ֵʼ�����������޺����޷�Χ֮�ڡ�������������ʱ��ͨ��QLMN_LLM ����˽���ָʾ��
unsigned char    QC_ACT = TRUE ;  //��һ��ѭ�����ڣ�������������������
//�ò���ָʾ����һ�������ʱ�Ƿ�ִ����������������(ֻ�е�SELECT ������ֵ0 ��1 ʱ�����)��
//END_VAR
//VAR_IN_OUT
float    CYCLE = 0.1 ;   //�����������Ĳ���ʱ��[��]
//�ڴ�����PID �㷨�Ĳ���ʱ�䡣��г���ڽ׶�1 �м������ʱ�䣬Ȼ�������뵽CYCLE �С�
float    CYCLE_P = 0.02 ;  //���巢�����Ĳ���ʱ��[��]
//�ڸ�����ˣ��������巢�������ڵĲ���ʱ�䡣FB 58��TCONT_CP���ڽ׶�1 �м������ʱ�䣬Ȼ�������뵽CYCLE_P��
float    SP_INT = 0.0 ;   //�ڲ��趨ֵ ���ڲ��趨ֵ�����������ָ��һ���趨ֵ��
float    MAN = 0.0 ;   //�ֶ�ֵ	���ֶ�ֵ�����������ָ��һ���ֶ�ֵ�����Զ�ģʽ�У������ĳɿɵ��ڱ�����
unsigned char    COM_RST = FALSE ; //��ȫ������	�ÿ���һ����ʼ�����г��򣬵�COM_RST ����˱���λʱִ�и����г���
unsigned char    MAN_ON = TRUE;   //�ֶ������� ������ֶ������򿪡����뱻��λ�����жϿ��ƻ�·��MAN �ֶ�ֵ������Ϊ�ɵ��ڱ�����ֵ��
//END_VAR
//VAR
float    DEADB_W = 0.0 ;   //������� ƫ��ͨ����������������ȡ��������������С��
float    I_ITLVAL = 0.0 ;   //�������õĳ�ʼ��ֵ
//�������õ����������I_ITL_ON ����������á��ó�ʼ��ֵ���ڡ��������õĳ�ʼ��ֵ�����롣���������ڼ䣬
//COM_RST = TRUE���������ñ����óɳ�ʼֵ��
float    LMN_HLM = 100.0;  //�ɵ��ڱ�������
//�ɵ��ڱ�����ֵʼ�����������޺����޷�Χ֮�ڡ����ɵ��ڱ������ޡ�����ָ�������ޡ�
float    LMN_LLM = 0.0 ;   //�ɵ��ڱ�������
//�ɵ��ڱ�����ֵʼ�����������޺����޷�Χ֮�ڡ����ɵ��ڱ������ޡ�����ָ�������ޡ�
float    PV_FAC = 1.0 ;   //���̱�������
//�����̱������ӡ�����͡���Χ�豸����ֵ����ˡ����������ڵ������̱����ķ�Χ��
float    PV_OFFS = 0.0 ;   //���̱���ƫ����
//�����̱���ƫ���������뱻�ӵ�����Χ�豸���̱������ϡ����������ڵ������̱����ķ�Χ��
float    LMN_FAC = 1.0 ;   //�ɵ��ڱ�������
//���ɵ��ڱ������ӡ�����Ϳɵ��ڱ�����ˡ����������ڵ����ɵ��ڱ����ķ�Χ��
float    LMN_OFFS = 0.0 ;   //�ɵ��ڱ���ƫ����
//���ɵ��ڱ���ƫ����������ӵ��ɵ��ڱ�������ֵ�ϡ����������ڵ����ɵ��ڱ����ķ�Χ��
float    PER_TM = 1.0 ;   //����[��]
//������Ƶ������ظ�������PER_TM ���������롣���巢�����������ظ����ںͲ���ʱ��֮��Ĺ�ϵ����������Ƶľ��ȡ�
float    P_B_TM = 0.0 ;   //��С����/�Ͽ�ʱ��[��]
//��С�������С�Ͽ�ʱ������ڡ���С����/�Ͽ�ʱ�䡱���������á�P_B_TM ���ڲ�������Ϊ����CYCLE_P��
float    TUN_DLMN = 20.0 ;  //���ڹ��̼����Ŀɵ��ڱ�������
//���ڿ����������Ĺ��̼���������TUN_DLMN �ϵ��趨ֵ��Ծ�仯��
int    PER_MODE = 0 ;      //��Χ�豸ģʽ�������������������I/O ģ������͡�����PV_PER �ϵĹ��̱�����PV ����ϱ���񻯳ɡ�C��
//. PER_MODE =0����׼
//. PER_MODE =1������
//. PER_MODE =2������/��ѹ
unsigned char    PVPER_ON = FALSE ; //��Χ�豸���̱�����
//������I/O �ж�ȡ���̱�������PV_PER ����������ӵ�I/O�����ҡ���Χ�豸���̱��������������λ��
unsigned char    I_ITL_ON = FALSE ; //�������õĳ�ʼ����
//�������õ������������ΪI_ITLVAL ���롣�����û������á����������λ��
unsigned char    PULSE_ON = FALSE ; //���巢������
//���������PULSE_ON = TRUE�������巢���������
unsigned char    TUN_KEEP = FALSE ; //����������
//ֻ�е�TUN_KEEP ���FALSEʱ������ģʽ���л����Զ���
float    ER = 0.0 ;   //ƫ���ź�  ��Ч��ƫ���ڡ�ƫ���źš�����������
float    LMN_P = 0.0 ;   //��������	�����������������˿ɵ��ڱ����ı������á�
float    LMN_I = 0.0 ;   //���ַ���	�����ַ����������˿ɵ��ڱ����Ļ������á�
float    LMN_D = 0.0 ;   //΢�ַ���	��΢�ַ����������˿ɵ��ڱ�����΢�����á�
int    PHASE = 0 ;      //�������Ľ׶� �����������ĵ�ǰ�׶���PHASE �����ָʾ(0..7)��
int    STATUS_H = 0 ;      //��������״̬���� �ڼ���ʱ��STATUS_H ָʾ�����յ�����ֵ��
int    STATUS_D = 0 ;      //��������״̬���������	������ʱ��STATUS_D ָʾ��������Ƶ����ֵ��
unsigned char    QTUN_RUN = FALSE ; //�����Ѽ���(PHASE 2)
//�Ѿ�Ӧ���������ɵ��ڱ����������Ѿ���ʼ������Ȼ���ڵ�2 �׶�(��λ�յ�)��
struct 
{
         //PI ����������
	float        GAIN; //= 0.0 ;   //PI ��������
	float        TI; //= 0.0 ;   //PI ��λʱ��[s]
}PI_CON; 
struct 
{  
          //PID ����������
	float        GAIN; //= 0.0 ;   //PID ��������
	float        TI; //= 0.0 ;   //PID ��λʱ��[s]
	float        TD; //= 0.0 ;   //PID ΢��ʱ��[s]
} PID_CON; 
struct 
{
         //����Ŀ���������	PID ���������ڴ˽ṹ�С�
	float        PFAC_SP; //= 1.0 ;   //�����趨ֵ�仯�ı�������[0..1]
	float        GAIN; //= 0.0 ;   //��������
	float        TI; //= 0.0 ;   //��λʱ��[s]
	float        TD; //= 0.0 ;   //΢��ʱ��[s]
	float        D_F; //= 5.0 ;   //΢������TD/TM_LAG (5..10)
	float        CON_ZONE; //= 100.0 ; //���ƴ���
	unsigned char        CONZ_ON; //= FALSE ; //���ƴ�
} PAR_SAVE;  
float    PFAC_SP = 1.0 ;   //�����趨ֵ�仯�ı�������
//�����趨ֵ�仯ʱ��PFAC_SP ָ����Ч�ı���P ���á��������÷�Χ��0 ��1��
//. 1������趨ֵ�����仯�������P ���þ���ȫ��Ч����
//. 0������趨ֵ�����仯������P ����û���κ�Ч����
float    GAIN = 2.0 ;   //��������
//���������桱��������ָ�����������档����ͨ����GAIN һ����������ת���Ʒ���
float    TI = 40.0 ;  //��λʱ��[��]
//����λʱ�䡱����(����ʱ��)�����˻���������Ӧ��
float    TD = 10.0 ;  //΢��ʱ��[��]
//��΢��ʱ�䡱���������΢��������Ӧ��
float    D_F = 5.0 ;   //΢������
//΢������������D ���õ��ͺ�D_F = ΢��ʱ��/��D ���õ��ͺ�
float    CON_ZONE = 100.0 ; //���ƴ�
//���ƫ����ڿ��ƴ���ȣ�������ɵ��ڱ���������Ϊʵ�ʵĿɵ��ڱ�����
//���ƫ��С�ڿ��ƴ���ȣ�������ɵ��ڱ���������Ϊʵ�ʵĿɵ��ڱ�����
unsigned char    CONZ_ON = FALSE ; //���ƴ���	CONZ_ON =TRUE ������ƴ���
unsigned char    TUN_ON = FALSE ; //��������
//���������TUN_ON=TRUE����ɵ��ڱ�����ȡƽ����ֱ���趨ֵ������Ծ�仯��TUN_ST=TRUE ���ɵ��ڱ�������TUN_DLMN ���
unsigned char    TUN_ST = FALSE ; //��ʼ������
//���Ҫʹ�趨ֵ�ڿ����������ڼ��ڹ������ϱ��ֲ��䣬��ͨ��TUN_ST=1 ������ɵ��ڱ���������СΪTUN_DLMN �Ľ�Ծ�仯��
unsigned char    UNDO_PAR = FALSE ; //�����Կ����������ĸ���
//�����ݽṹPAR_SAVE ��װ�ؿ���������PFAC_SP��GAIN��TI��  TD��D_F CONZ_ON ��CON_ZONE (ֻ���ֶ�ģʽ��)��
unsigned char    SAVE_PAR = FALSE ; //���浱ǰ����������
//������������PFAC_SP��GAIN��TI��TD��D_F CONZ_ON ��CON_ZONE ���浽���ݽṹPAR_SAVE �С�
unsigned char    LOAD_PID = FALSE ; //װ���Ż���PI/PID ����
//����PID_ON�������ݽṹPI_CON ��PID_CON ��װ�ؿ���������GAIN��TI��TD (�����ֶ�ģʽ��)
unsigned char    PID_ON = TRUE ;  //PID ģʽ��
//��PID_ON �����ϣ�����ָ���������Ŀ������Ƿ���ΪPI ��������PID ���������С�
//. PID ��������PID_ON = TRUE
//. PI ��������PID_ON = FALSE����ĳЩ����������˵������PID_ON = TRUE������Ȼ����ֻ���PI ��������
float    GAIN_P = 0.0 ;   //���̱�������
//ʶ����Ĺ������档���ڹ�������I��˵��GAIN_P �����ڱ����Ƶõ�һЩ��
float    TU = 0.0 ;   //�ӳ�ʱ��[��] ʶ����Ĺ����ӳ١�
float    TA = 0.0 ;   //�ָ�ʱ��[��]
//ʶ����Ĺ��̵�ϵͳʱ�䳣�������ڹ�������I ��˵��TA �����ڱ����Ƶõ�һЩ��
float    KIG = 0.0 ;   //PV WITH 100 % LMN �仯�����б��GAIN_P = 0.01 * KIG * TA
float    N_PTN = 0.0 ;   //����˳�� �ò���ָ�������ִ��˳��Ҳ����ָ����������ֵ����
float    TM_LAG_P = 0.0 ;   //PTN ģ�͵�ʱ���ӳ�[��] PTN ģ�͵�ʱ���ӳ�(��Чֵֻ����N_PTN >= 2)��
float    T_P_INF = 0.0 ;   //���յ��ʱ��[��] �ӹ��̼������յ���������ʱ�䡣
float    P_INF = 0.0 ;   //�յ㴦��PV - PV0 �ӹ��̼������յ㴦�����̱��������ı仯��
float    LMN0 = 0.0 ;   //������ʼʱ�Ŀɵ��ڱ���   �ڽ׶�1 �м��(ƽ��ֵ)��
float    PV0 = 0.0 ;   //������ʼʱ�Ĺ���ֵ
float    PVDT0 = 0.0 ;   //������ʼʱPV �ı仯��[1/s]  �����з�������
float    PVDT = 0.0 ;   //PV �ĵ�ǰ�仯��[1/s] �����з�������
float    PVDT_MAX = 0.0 ;   //ÿ����PV �����仯��[1/s]
//�յ㴦���̱��������仯��(�����з�������ʼ�մ���0)�����ڼ���TU ��KIG��
float    NOI_PVDT = 0.0 ;   //PVDT_MAX �е��������ʣ�����%��ʽ �����ı���Խ�ߣ����Ʋ����ľ��Ⱦ�Խ��(Խ������)��
float    NOISE_PV = 0.0 ;   //PV �еľ�������  �׶�1 ��������С���̱����Ĳ�ֵ��
int    FIL_CYC = 1 ;      //��ֵ��������������	���̱���ȡFIL_CYC �����ڵ�ƽ��ֵ�������Ҫ��FIL_CYC �����Զ���1 ���ӵ����ֵ1024��
int    POI_CMAX = 0 ;      //�յ������������
//��ʱ�������ڲ����������ڵ�����£��ҵ�����һ����(����˵�����õ�)�յ㡣ֻ���ڴ�ʱ����������Ž�����
int    POI_CYCL = 0 ;      //�յ���������
int    sctFil = 0 ;    
float    srPv = 0.0 ;    
float    srNoise = 0.0 ;    
float    srPvdt2 = 0.0 ;    
float    srPvdtMax2 = 0.0 ;    
float    srNoise2 = 0.0 ;    
float    srPvOld2 = 0.0 ;    
int    siStatus = 0 ;    
int    siSpDir = 0 ;    
float    srTime = 0.0 ;    
float    sPvOld = 0.0 ;    
float    sSpOld = 0.0 ;    
float    srPvMin = 0.0 ;    
float    srPVsim = 0.0 ;    
float    srEffDlmn = 0.0 ;    
float    sInvAlt = 0.0 ;   //�����ڱ���ƫ��ֵ 
float    sIanteilAlt = 0.0 ;  //�����ڻ���ֵ  
float    sRestInt = 0.0 ;   //�����ڻ���ƫ����������������ƫ� 
float    sRestDif = 0.0 ;   //������΢��ƫ����������������ƫ� 
float    sRueck = 0.0 ;    
float    sLmn = 0.0 ;     //�����ڵ���ֵ
float   spassPTm = 0.0 ;    
float    sCycTmPass = 0.0 ;    
float    sPTm = 0.0 ;    
float    srOptLmn = 0.0 ;    
unsigned char    sbOptLmn = FALSE ;    
unsigned char    sbConzOn = FALSE ;    
unsigned char   sbSpChange = FALSE ;  //�趨ֵ�Ƿ�仯  
unsigned char    sbReOpt = FALSE ;    
float    sLmnHlmOld = 100.0 ;    
float   sLmnLlmOld = 0.0 ;    
float    srDiffSp = 0.0 ;    
long int    siTime = 0 ;    
long int    siCycle = 0 ;    
long int    siCycleP = 0 ;    
int    siPulsFil = 0 ;    
float    srPulsPv = 0.0 ;    
float    srDPvdt = 0.0 ;    
float    srDPvdt2 = 0.0 ;    
float    sNpInf = 0.0 ;    
float    sCycleM = 0.0 ;    
float    sGf11_30 = 0.0 ;    
float    sGf12_30 = 0.0 ;    
float    sGf21_30 = 0.0 ;    
float    sGf22_30 = 0.0 ;    
float    sPv0Sim_30 = 0.0 ;    
float    srPVsim3_30 = 0.0 ;    
float    srPVsim2_30 = 0.0 ;    
float    srPVsim1_30 = 0.0 ;    
float    srDelPvsim_30 = 0.0 ;    
float    sGf11_20 = 0.0 ;    
float    sGf12_20 = 0.0 ;    
float    sGf21_20 = 0.0 ;    
float    sGf22_20 = 0.0 ;    
float    sPv0Sim_20 = 0.0 ;    
float    srPVsim2_20 = 0.0 ;    
float    srPVsim1_20 = 0.0 ;    
float    srDelPvsim_20 = 0.0 ;    
float    sGf11_15 = 0.0 ;    
float    sGf12_15 = 0.0 ;    
float    sGf21_15 = 0.0 ;    
float    sGf22_15 = 0.0 ;    
float    sPv0Sim_15 = 0.0 ;    
float    srPVsim2_15 = 0.0 ;    
float    srPVsim1_15 = 0.0 ;    
float    srDelPvsim_15 = 0.0 ;    
//END_VAR
//VAR_TEMP
long int    tTime ;//:         TIME ;
long int    iTime  ;
//float    rErKp  ; //ƫ��/����ƫ��
float    rDiff  ;
float    rDiff_int  ; //ƫ����ʱ���������ж��Ƿ񳬳����ƴ�
float    Verstaerk  ;
float    RueckDiff  ;
float    RueckAlt  ;  //�����ڻ�����
float    dLmn  ;	  //������
//unsigned char    bUpp ;
//unsigned char    bDownp ;
//unsigned char    bUpn ;
//unsigned char    bDownn ;
//unsigned char    bHvar ;
unsigned char    bIZv ;
unsigned char    bLMNnew ;
unsigned char    bPulse ;
unsigned char    bConzOn ;	 //���ƴ���ִ��
float    rSeek  ;
float    rPvDelt  ;
float   rPvFil  ;
float   rPV0  ;
float    rPvdtOri  ;
float    rPVsigned  ;
float    rSpDelt  ;	 //�趨��ı仯
float   tw_durch_tu  ;
float    gf  ;
float    koeff  ;
float    rCycle  ;	//�����������Ĳ���ʱ�䣨s��
float    rTmLag  ;	//΢������ʱ����ʱ����ֵ
float    rDeltaI  ;	//����ֵ�ı仯
float    rVal  ;
float    rPvNew2  ;
float    rNoiPvdt  ;
float    rPvdtMax0  ;
float    rPVin  ;
float    rP_B_TM  ;
float    rPvdt  ;
float    rPvdt2  ;
float    rPvdt3  ;	 // srPvdt2 ��ʼֵ
//float    rLmnPmod  ; //������CONZONE�޸�
int    iFilCyc  ;	 //�ڲ������ں�Ϊ��ֵ�˲�
int    iVal  ;    
float    rT1sim  ;	 //ģ��ʱ�䳣��
float    rT2sim  ;	 //ģ��ʱ�䳣��
float    rKsim  ;
float    rSumHp  ;
float    rSumLp  ;
float    rHp  ;
float    rLp  ;
float    rNptn  ;	//ͨ��ģ��˳��ȷ��
float    rNptnHl  ;	//ͨ���������޾�����˳��
float    rNptnLl  ;	//ͨ����������ȷ����˳��

//END_VAR






void pid_tune(void);





void pid(void)
{
	if( COM_RST )//��λ
	{
		PV = 0.0 ;
		ER = 0.0 ;
		LMN_P = 0.0 ;
		LMN_I = 0.0 ;
		LMN_D = 0.0 ;
		LMN = 0.0 ;
		LMN_PER = 0 ;
		PHASE = 0 ;
		QLMN_HLM = FALSE ;
		QLMN_LLM = FALSE ;
		QC_ACT = TRUE ;
		QPULSE = FALSE ;
		TUN_ON = FALSE ;
		UNDO_PAR = FALSE ;
		SAVE_PAR = FALSE ;
		LOAD_PID = FALSE ;
		sInvAlt = 0.0 ;
		sIanteilAlt = I_ITLVAL ;
		sRestInt = 0.0 ;
		sRestDif = 0.0 ;
		sRueck = 0.0 ;
		sLmn = 0.0 ;
		srDiffSp = 0.0 ;
		siPulsFil = 0 ;
		srPulsPv = 0.0 ;
		spassPTm = 0.0 ;
		sCycTmPass = 0.0 ;
		sPTm = 0.0 ;
		sbConzOn = FALSE ;
		sbReOpt = FALSE ;
		sGf11_30 = 0.0 ;
		sGf12_30 = 0.0 ;
		sGf21_30 = 0.0 ;
		sGf22_30 = 0.0 ;
		sPv0Sim_30 = 0.0 ; 
		srPVsim3_30 = 0.0 ; 
		srPVsim2_30 = 0.0 ; 
		srPVsim1_30 = 0.0 ; 
		srDelPvsim_30 = 0.0 ; 
		sGf11_20 = 0.0 ;
		sGf12_20 = 0.0 ;
		sGf21_20 = 0.0 ;
		sGf22_20 = 0.0 ;
		sPv0Sim_20 = 0.0 ;
		srPVsim1_20 = 0.0 ;
		srPVsim2_20 = 0.0 ;
		srDelPvsim_20 = 0.0 ;
		sGf11_15 = 0.0 ;
		sGf12_15 = 0.0 ;
		sGf21_15 = 0.0 ;
		sGf22_15 = 0.0 ;
		sPv0Sim_15 = 0.0 ;
		srPVsim1_15 = 0.0 ;
		srPVsim2_15 = 0.0 ;
		srDelPvsim_15 = 0.0 ;
		COM_RST = FALSE ;
	}
	else
	{
		bLMNnew = (! PULSE_ON || ( SELECT != 2 && QC_ACT ) || SELECT == 3) ;
		bPulse = ( SELECT == 0 || SELECT == 2 ) && PULSE_ON ;
		if( PVPER_ON )	//���̱�������ѡ��
		{
			switch(PER_MODE)
			{
				case 1:
					rPVin =  PV_PER  * 0.01 ;
				break;
				case 2:
					rPVin =  PV_PER  * 3.616898e-003 ;
				break;
				default:
					rPVin =  PV_PER  * 0.1 ;
				break;
			}
			rPVin = rPVin * PV_FAC + PV_OFFS ;
		}
		else
		{
			rPVin = PV_IN ;
		}
		if( bPulse )
		{
			srPulsPv = rPVin - sPvOld + srPulsPv ;
			siPulsFil = siPulsFil + 1 ;
		}
		if( bLMNnew )
		{
			if( siPulsFil > 0 )
			{
				PV = srPulsPv /  siPulsFil  + sPvOld ;
			}
			else
			{
				PV = rPVin ;
			}
			siPulsFil = 0 ;
			srPulsPv = 0.0 ;
			if( LOAD_PID && MAN_ON )  //�ж��Ƿ���PID���ֶ�����
			{
				if( PID_ON )
				{
					if( PID_CON.GAIN != 0.0 )
					{
						GAIN = PID_CON.GAIN ;
						TI = PID_CON.TI ;
						TD = PID_CON.TD ;
						CON_ZONE = 250.0 / GAIN ;
					}
					else 
					{
						PID_ON = FALSE ;
					}
				}
				if( ! PID_ON && PI_CON.GAIN != 0.0 )
				{
					GAIN = PI_CON.GAIN ;
					TI = PI_CON.TI ;
					TD = 0.0 ;
					CONZ_ON = FALSE ;
					CON_ZONE = 250.0 / GAIN ;
				}
				if( CON_ZONE < 0.0 )
				{
					CON_ZONE = -CON_ZONE ;
				}
			}
			LOAD_PID = FALSE ;
			if( UNDO_PAR && MAN_ON && PAR_SAVE.GAIN != 0.0 )//����װ�ز���
			{
				PFAC_SP = PAR_SAVE.PFAC_SP ;
				GAIN = PAR_SAVE.GAIN ;
				TI = PAR_SAVE.TI ;
				TD = PAR_SAVE.TD ;
				D_F = PAR_SAVE.D_F ;
				CONZ_ON = PAR_SAVE.CONZ_ON ;
				CON_ZONE = PAR_SAVE.CON_ZONE ;
			}
			UNDO_PAR = FALSE ;
			if( SAVE_PAR )	//�洢����
			{
				PAR_SAVE.PFAC_SP = PFAC_SP ;
				PAR_SAVE.GAIN = GAIN ;
				PAR_SAVE.TI = TI ;
				PAR_SAVE.TD = TD ;
				PAR_SAVE.D_F = D_F ;
				PAR_SAVE.CONZ_ON = CONZ_ON ;
				PAR_SAVE.CON_ZONE = CON_ZONE ;
				SAVE_PAR = FALSE ;
			}
			iFilCyc = FIL_CYC ;//��ֵ��������������
			if( PHASE != 2 )
			{
				iFilCyc = 1 ;
			}
			rCycle =  iFilCyc  * CYCLE ;
			if( PHASE < 2 )
			{
				if( bPulse )
				{
					rPvFil = PV ;
				}
				else
				{
					rPvFil = ( PV - sPvOld ) * 0.1 + sPvOld ;
				}
				sPvOld = rPvFil ;
				sctFil = 0 ;
			}
			else
			{
				if( sctFil < iFilCyc )
				{
					srPv = PV - sPvOld + srPv ;
					sctFil = sctFil + 1 ;
				}
				if( sctFil >= iFilCyc )
				{
					rPvdt = srPv /   sctFil  ;
					rPvFil = rPvdt + sPvOld ;
					rPvdt = rPvdt / rCycle ;
					sctFil = 0 ;
					srPv = 0.0 ;
					rPvdtOri = rPvdt ;
					gf =  siSpDir  * PVDT ;
					rVal = rPvdt - gf ;
					if( rVal < 0.0 )
					{
						rVal = -rVal ;
					}
					srDPvdt = ( rVal - srDPvdt ) * 0.1 + srDPvdt ;
					if( rPvdt > ( srDPvdt + gf ))
					{
						rPvdt = srDPvdt + gf ;
					}
					else if( rPvdt > ( gf - srDPvdt ) )
					{
						rPvdt = gf - srDPvdt ;
					}
					PVDT = rPvdt ;
				}
			}
			if( ! TUN_ON )	//�������Ҫ����
			{
				TUN_ST = FALSE ;
				sbSpChange = FALSE ;
				sbOptLmn = FALSE ;
				srOptLmn = 0.0 ;
				srEffDlmn = 0.0 ;
				PHASE = 0 ;
				QTUN_RUN = FALSE ;
			}
			if( sctFil == 0 && TUN_ON )	 //�ж��Ƿ���Ҫ����
			{
			 	 pid_tune();
			}
			if( PHASE <= 7 && PHASE >= 2 )
			{
				srPVsim1_30 = sGf11_30 * srPVsim1_30 + ( sLmn - LMN0 ) * sGf12_30 ;
				srPVsim2_30 = sGf21_30 * srPVsim2_30 + sGf22_30 * srPVsim1_30 ;
				srPVsim3_30 = sGf21_30 * srPVsim3_30 + sGf22_30 * srPVsim2_30 ;
				srPVsim1_20 = sGf11_20 * srPVsim1_20 + ( sLmn - LMN0 ) * sGf12_20 ;
				srPVsim2_20 = sGf21_20 * srPVsim2_20 + sGf22_20 * srPVsim1_20 ;
				srPVsim1_15 = sGf11_15 * srPVsim1_15 + ( sLmn - LMN0 ) * sGf12_15 ;
				srPVsim2_15 = sGf21_15 * srPVsim2_15 + sGf22_15 * srPVsim1_15 ;
			}
			rDiff = SP_INT - PV ;  //ƫ��
			rSpDelt = SP_INT - sSpOld ;
			if( rDiff < - DEADB_W )	  //���ƫ���Ƿ����������ȵĸ�ֵ
			{
				rDiff = rDiff + DEADB_W ;
			}
			else if( rDiff > DEADB_W )//���ƫ���Ƿ�����������
			{
				rDiff = rDiff - DEADB_W ;
			}
			else
			{
				rDiff = 0.0 ;
			}
			ER = rDiff ;//ƫ��
			rTmLag = TD / D_F ;
			LMN_P = GAIN * ER ;	  //��������
			if( TI != 0.0 && ! I_ITL_ON )
			{
				if( ! MAN_ON && ! sbOptLmn && ! sbConzOn )
				{
					rDiff = CYCLE / TI * ( LMN_P + sInvAlt ) * 0.5 + sRestInt ;
					if( ( rDiff > 0.0 && ( INT_HPOS || QLMN_HLM ) ) || ( rDiff < 0.0 && ( INT_HNEG || QLMN_LLM ) ))
					{
						rDiff = 0.0;
					}
					LMN_I = sIanteilAlt + rDiff ;
					sRestInt = sIanteilAlt - LMN_I + rDiff ;
				}
			}
			else
			{
				sRestInt = 0.0 ;
				if( I_ITL_ON )
				{
					LMN_I = I_ITLVAL ;
				}
				else
				{
					LMN_I = 0.0 ;
				}
			}
			if( TD != 0.0 && ! MAN_ON && ! sbOptLmn &&  ! sbConzOn )
			{
				Verstaerk = TD / ( CYCLE * 0.5 + rTmLag ) ;
				LMN_D = ( LMN_P - sRueck ) * Verstaerk ;
				RueckAlt = sRueck ;
				RueckDiff = CYCLE / TD * LMN_D + sRestDif ;
				sRueck = RueckDiff + RueckAlt ;
				sRestDif = RueckAlt - sRueck + RueckDiff ;
			}
			else
			{
				LMN_D = 0.0 ;
				sRestDif = 0.0 ;
				sRueck = LMN_P ;
			}
			bConzOn = FALSE ;
			rDeltaI = 0.0 ;
			if( sbOptLmn )
			{
				dLmn = srOptLmn + srEffDlmn ;
			}
			else if( MAN_ON )
			{
				dLmn = MAN ;
			}
			else
			{
				dLmn = LMN_P + LMN_I + DISV ;  //PI���
				if( ! MAN_ON )	//��PI����ʱ����PID����͵��ֶ�MAN
				{
					if( dLmn > LMN_HLM )
					{
						MAN = LMN_HLM ;
					}
					else if( dLmn < LMN_LLM )
					{
						MAN = LMN_LLM ;
					}
					else
					{
						MAN = dLmn ;
					}
				}
				dLmn = LMN_P + LMN_I + DISV + LMN_D ;	//PID���
				if( TI != 0.0 && ! I_ITL_ON )
				{
					if( PFAC_SP > 1.0 )
					{
						PFAC_SP = 1.0 ;
					}
					else if( PFAC_SP < 0.0 )
					{
						PFAC_SP = 0.0 ;
					}
					if( PFAC_SP > 0.5 )
					{
						gf = 1.0 - PFAC_SP ;
					}
					else
					{
						gf = PFAC_SP ;
					}
					rDeltaI = GAIN * rSpDelt * ( PFAC_SP - 1.0 -gf ) ;
					srDiffSp = GAIN * rSpDelt * gf + srDiffSp ;
					srDiffSp = srDiffSp * gf * TI / ( gf * TI + CYCLE ) ;
					dLmn = dLmn + rDeltaI + srDiffSp ;
					if( CONZ_ON )
					{
						gf = CON_ZONE * 0.8 ;
						rDiff_int = ER ;
						if( GAIN < 0.0 )
						{
							rDiff_int = - rDiff_int ;
						}
						if( rDiff_int >= CON_ZONE || ( rDiff_int >= gf && sbConzOn ) )
						{
							dLmn = LMN_HLM ;
							bConzOn = TRUE ;
						}
						else if( rDiff_int <= - CON_ZONE || ( rDiff_int <= - gf && sbConzOn ) )
						{
							dLmn = LMN_LLM ;
							bConzOn = TRUE ;
						}
					}
					if( ! bConzOn )
					{
						if( LMN_HLM < sLmnHlmOld && dLmn > LMN_HLM )
						{
							rVal = dLmn ;
							if( rVal > sLmnHlmOld )
							{
								rVal = sLmnHlmOld ;
							}
							rDeltaI = rDeltaI - rVal + LMN_HLM ;
						}
						else
						{
							if( LMN_LLM > sLmnLlmOld && dLmn < LMN_LLM )
							{
								rVal = dLmn ;
								if( rVal < sLmnLlmOld )
								{
									rVal = sLmnLlmOld ;
								}
								rDeltaI = rDeltaI - rVal + LMN_LLM ;
							}
						}
					}
				}
				else
				{
					srDiffSp = 0.0 ;
				}
			}
			sLmnHlmOld = LMN_HLM ;
			sLmnLlmOld = LMN_LLM ;
			sbConzOn = bConzOn ;
			sLmn = dLmn ;
			if( sLmn > LMN_HLM )
			{
				QLMN_HLM = TRUE ;
				QLMN_LLM = FALSE ;
				sLmn = LMN_HLM ;
			}
			else
			{
				QLMN_HLM = FALSE ;
				if( sLmn <= LMN_LLM )
				{
					QLMN_LLM = TRUE ;
					sLmn = LMN_LLM ;
				}
				else
				{
					QLMN_LLM = FALSE ;
				}
			}
			if( TI != 0.0 )
			{
				if( MAN_ON || sbOptLmn || sbConzOn )
				{
					srDiffSp = 0.0 ;
					sRestInt = 0.0 ;
					LMN_I = sLmn - LMN_P - DISV ;
					if( sbConzOn )
					{
						MAN = sLmn ;
					}
				}
				else
				{
					LMN_I = LMN_I + rDeltaI ;
					if( LMN_I > LMN_HLM - DISV && dLmn > LMN_HLM && dLmn- LMN_D > LMN_HLM )
					{
						rVal = LMN_HLM - DISV ;
						gf = dLmn -LMN_HLM ;
						rVal = LMN_I - rVal ;
						if( rVal > gf )
						{
							rVal = gf ;
						}
						LMN_I = LMN_I - rVal ;
					}
					else if( LMN_I < LMN_LLM - DISV && dLmn < LMN_LLM && dLmn -LMN_D < LMN_LLM )
					{
						rVal = LMN_LLM - DISV ;
						gf = dLmn -LMN_LLM ;
						rVal = LMN_I - rVal ;
						if( rVal < gf )
						{
							rVal = gf ;
						}
						LMN_I = LMN_I - rVal ;
					}
				}
			}
			dLmn = sLmn ;
			sInvAlt = LMN_P ;
			sIanteilAlt = LMN_I ;
			sSpOld = SP_INT ;
			LMN = dLmn * LMN_FAC + LMN_OFFS ;
			dLmn = LMN * 264.8 ;
			if( dLmn >= 32511.0 )
			{
				dLmn = 32511.0 ;
			}
			else if( dLmn <= -32512.0 )
			{
				dLmn = -32512.0 ;
			}
			LMN_PER =  dLmn  ;
			QC_ACT = ! PULSE_ON ;
			sPTm = sLmn * PER_TM * 0.01 ;
			if( sLmn < 0.0 )
			{
				sPTm = 0.0 ;
			}
		}
		if( bPulse )
		{
			rP_B_TM = P_B_TM ;
			if( rP_B_TM < CYCLE_P )
			{
				rP_B_TM = CYCLE_P ;
			}
			bIZv = FALSE ;
			if( QPULSE )
			{
				if( spassPTm < rP_B_TM || sPTm >= spassPTm || sPTm >= PER_TM - rP_B_TM )
				{
					bIZv = TRUE ;
				}
				if( ! bIZv )
				{
					QPULSE = FALSE ;
				}
			}
			else if( spassPTm >= rP_B_TM && spassPTm >= PER_TM - sPTm && sPTm >= rP_B_TM )
			{
				QPULSE = TRUE ;
			}
			else
			{
				bIZv = TRUE ;
			}
			if( bIZv )
			{
				if( spassPTm < PER_TM )
				{
					spassPTm = spassPTm + CYCLE_P ;
				}
			}
			else
			{
				spassPTm = CYCLE_P * 1.5 ;
			}
			sCycTmPass = sCycTmPass + CYCLE_P ;
			siCycleP = siCycleP + 1 ;
			if( sCycTmPass >= CYCLE) 
			{
				QC_ACT = TRUE ;
				sCycTmPass = CYCLE_P * 0.5 ;
			}
		}
	}
}
void pid_tune(void)
{
	rPVsigned = rPvFil ;
	rSpDelt = SP_INT - sSpOld ;
	if( PHASE > 1 )
	{
		if( PHASE < 3 && ! sbSpChange && ( rPvFil - PV0 ) *  siSpDir  < 0.0 )
		{
			siStatus = 999 ;
			siSpDir = - siSpDir;
			srNoise = 0.0 ;
			srNoise2 = 0.0 ;
			PVDT0 = - PVDT0 ;
			srPvdt2 = - srPvdt2 ;
			PVDT_MAX = PVDT0 ;
			srPvdtMax2 = PVDT0 ;
		}
		if( sbSpChange )
		{
			rSeek = ( SP_INT - PV0 ) * 0.75 ;
		}
		else
		{
			rSeek = 0.0 ;
		}
		if( siSpDir < 0 )
		{
			rSeek = - rSeek ;
			rPvFil = - rPvFil ;
			rPvdtOri = - rPvdtOri ;
			PVDT = - PVDT ;
			rPV0 = - PV0 ;
		}
		else
		{
			rPV0 = PV0 ;
		}
	}
	switch(PHASE)  //����������
	{
		case 0:	 //������,�Զ����ֶ�ģʽ
		{
	//					tTime = TIME_TCK() ;
			siTime =  tTime  ;
			siCycle = 0 ;
			siCycleP = 0 ;
			FIL_CYC = 1 ;  //��ֵ��������������
			LMN0 = sLmn ;
			srPvMin = PV ;
			PV0 = rPvFil ;
			NOISE_PV = 0.0 ;
			siStatus = 1 ;
			siSpDir = 1 ;
			TUN_ST = FALSE ;
			SP_INT = sSpOld ;
			STATUS_H = 0 ;//ȱʡ��(��)û���µĿ�����������
			PHASE = 1 ;
		}
		break;
		case 1:		 //׼������,������,�ȴ�����,��������ʱ��.
		{
			siCycle = siCycle + 1 ;
			if( siStatus < 2000 )
			{
				siStatus = siStatus + 1 ;
			}
			LMN0 = ( sLmn - LMN0 ) /  siStatus  + LMN0 ;
			rVal = PV - srPvMin ;
			if( rVal < 0.0 )
			{
				NOISE_PV = NOISE_PV - rVal ;
				srPvMin = PV ;
			}
			else if( NOISE_PV < rVal )
			{
				NOISE_PV = rVal ;
			}
			if( rSpDelt != 0.0 || TUN_ST )
			{
				if( rSpDelt != 0.0 )
				{
					sbSpChange = TRUE ;
				}
				if( SP_INT < PV && sbSpChange )
				{
					siSpDir = -1 ;
				}
				srEffDlmn = TUN_DLMN ;
				gf = srEffDlmn + LMN0 ;
				if( gf > LMN_HLM )
				{
					srEffDlmn = LMN_HLM - LMN0 ;
				}
				else if(gf < LMN_LLM )
				{
					srEffDlmn = LMN_LLM - LMN0 ;
				}
				if( srEffDlmn < 5.0 && srEffDlmn > -5.0 )
				{
					STATUS_H = 30002 ; //��Ч�ɵ��ڱ�����ֵ < 5 % ,�����ɵ��ڱ�����ֵTUN_DLMN��
					SP_INT = sSpOld ;
					TUN_ON = FALSE ;
					PHASE = 0 ;
				}
				else
				{
	//							tTime = TIME_TCK();
					iTime =  tTime ;
					if( iTime > siTime )
					{
						iTime = iTime - siTime ;
					}
					else
					{
						iTime = iTime + 1 ;
						iTime = iTime - siTime + 2147483647 ;
					}
					sCycleM =  iTime  ;
					srTime = sCycleM * 0.001 ;
					sCycleM = srTime /  siCycle  ;
					srTime = ( CYCLE - sCycleM ) / CYCLE ;
					if( srTime > 0.05 || srTime < -0.05 )
					{
						STATUS_H = 30005 ;//����ʱ��CYCLE ��CYCLE_P ֮��Ĳ�ֵ�����˲���ֵ��5%��
						//��CYCLE ��CYCLE_P ���������жϼ���������Ƚϣ�ע����ܴ��ڵ��÷�������
						//���CPU ��ʹ����������CPU ���ڸ�ʹ��״̬������Ҫ�ӳ�����ʱ�䣬��ʱ����Բ���CYCLE ��CYCLE_P ƥ�䡣
						SP_INT = sSpOld ;
						TUN_ON = FALSE ;
						PHASE = 0 ;
					}
					else
					{
						srPv = 0.0 ;
						srNoise = 0.0 ;
						POI_CMAX = 20 ;
						POI_CYCL = 0 ;
						PVDT0 = ( ( rPvFil - PV0 ) *  siSpDir  ) / (  siCycle  * CYCLE ) ;
						PV0 = rPvFil ;
						P_INF = 0.0 ;
						rPVsigned = PV ;
						srTime = 0.0 ;
						PVDT_MAX = PVDT0 ;
						NOI_PVDT = 0.0 ;
						srDPvdt = 0.0 ;
						srDPvdt2 = 0.0 ;
						srPvdt2 = 0.0 ;
						srPvdtMax2 = PVDT0 ;
						srNoise2 = 0.0 ;
						srPvOld2 = rPvFil ;
						siTime = 0 ;
						sbOptLmn = TRUE ;
						srOptLmn = LMN0 ;
						siStatus = 0 ;
						siCycle = 0 ;
						PHASE = 2 ;
						QTUN_RUN = TRUE ;
						sbReOpt = FALSE ;
					}
				}
			}
		}
		break;
		case 2:		 //ʵ������,�ȴ����ȶ��Ŀ��������ֵ�ϼ�⵽�յ�.
		{
			rPvDelt = rPvFil - rPV0 ;
			srTime = srTime + rCycle ;
			siCycle = siCycle + 1 ;
			if( PVDT < PVDT_MAX || PVDT == 0.0 )
			{
				POI_CYCL = POI_CYCL + 1 ;
				if( rPvdtOri >= PVDT_MAX )
				{
					POI_CYCL = 0 ;
				}
			}
			else
			{
				PVDT_MAX = PVDT ;
				if( POI_CYCL > 0 )
				{
					iVal = POI_CYCL * 2 ;
					if( POI_CMAX <= iVal )
					{
						POI_CMAX = iVal ;
					}
					POI_CYCL = 0 ;
				}
			}
			siTime = siTime + 1 ;
			if( siTime == 2 )
			{
				rPvNew2 = ( sPvOld + rPVsigned ) * 0.5 ;
				rPvdt2 = ( rPvNew2 - srPvOld2 ) / ( 2.0 * rCycle ) *  siSpDir  ;
				rVal = rPvdt2 - srPvdt2 ;
				if( rVal < 0.0 )
				{
					rVal = -rVal ;
				}
				srDPvdt2 = ( rVal - srDPvdt2 ) * 0.1 + srDPvdt2 ;
				if( rPvdt2 > srDPvdt2 + srPvdt2 )
				{
					rPvdt2 = srDPvdt2 + srPvdt2 ;
				}
				else if( rPvdt2 < srDPvdt2 - srPvdt2 )
				{
					rPvdt2 = srDPvdt2 - srPvdt2 ;
				}
				rPvdt3 = ( rPvdt2 + srPvdt2 ) * 0.5 ;
				srPvdt2 = rPvdt2 ;
				if( srPvdtMax2 < srPvdt2 )
				{
					srPvdtMax2 = srPvdt2 ;
				}
				gf = srPvdtMax2 - srPvdt2 ;
				if( srNoise2 < gf )
				{
					srNoise2 = gf ;
				}
			}
			if( PVDT_MAX != 0.0 )
			{
				rNoiPvdt = 100.0 * srNoise / PVDT_MAX ;
			}
			else
			{
				rNoiPvdt = 100.0 ;
			}
			rPvdtMax0 = PVDT_MAX - PVDT0 ;
			if((( rPvDelt < 0.8 * rSeek || ! sbSpChange ) && QTUN_RUN && FIL_CYC < 1024 && siTime == 2 )&& ( ( ( siCycle > 6 && PVDT == 0.0 || siStatus == 999 )|| ( ( rNoiPvdt > 60.0 && siCycle > 7 ) && ( POI_CYCL < 2 || srPvdt2 > 0.99 * srPvdtMax2 )))|| ( rNoiPvdt > 5.0 && siCycle > 15 &&  POI_CYCL < 2 )) )
			{
				FIL_CYC = FIL_CYC * 2 ;		//��ֵ��������������
				rPVsigned = ( sPvOld + rPVsigned ) * 0.5 ;
				srDPvdt = srDPvdt2 ;
				srNoise = srNoise2 ;
				PVDT_MAX = srPvdtMax2 ;
				PVDT = srPvdtMax2 ;
				POI_CYCL = 0 ;
				srPvOld2 = ( srPvOld2 + rPvNew2 ) * 0.5 ;
				srPvdtMax2 = PVDT0 ;
				srDPvdt2 = srNoise2 * 0.5 ;
				srNoise2 = 0.0 ;
				srPvdt2 = rPvdt3 ;
				siTime = 0 ;
				siCycle = 0 ;
				siStatus = 0 ;
			}
			if( siTime == 2 )
			{
				srPvOld2 = rPvNew2 ;
				siTime = 0 ;
			}
			if( rPvDelt > rSeek * 0.3 )
			{
				if( POI_CYCL >= POI_CMAX && ( rPvDelt -  siSpDir  * P_INF ) > 2.0 * NOISE_PV && rPvDelt > 1.2 * P_INF  *  siSpDir  )
				{
					QTUN_RUN = FALSE ;
					if( ! TUN_KEEP )
					{
						PHASE = 3 ;
					}
				}
				else if( rPvDelt > rSeek && sbSpChange )
				{
					if( POI_CYCL == 0 )
					{
						POI_CYCL = 1 ;
					}
					STATUS_H = STATUS_H + 20 ;
					QTUN_RUN = FALSE ;
					PHASE = 3 ;
				}
			}
			if( POI_CYCL == 1 )
			{
				if( rPvdtMax0 > 1.000000e-009 )
				{
					rVal = srNoise ;
					gf = rPvdtMax0 * 1.99 ;
					if( rVal > gf )
					{
						rVal = gf ;
					}
					NOI_PVDT = 100.0 * rVal / rPvdtMax0 ;
					rPvdtMax0 = rPvdtMax0 - rVal * 0.45 ;
					TU = ( ( PVDT_MAX - rVal * 0.45 ) * srTime - rPvDelt ) / rPvdtMax0 - ( rCycle - CYCLE ) * 0.5 ;
					KIG = rPvdtMax0 / srEffDlmn *  siSpDir  * 100.0 ;
					P_INF = siSpDir  * rPvDelt ;
					T_P_INF = srTime - 2.0 * rCycle ;
					rT1sim = 1.129 * TU ;
					rT2sim = 105.72 * TU ;
					rKsim = 1.1103 * TU * KIG ;
					sGf11_15 = rT1sim / ( rT1sim + CYCLE ) ;
					sGf12_15 = CYCLE / ( rT1sim + CYCLE ) * rKsim *  siSpDir  ;
					sGf21_15 = rT2sim / ( rT2sim + CYCLE ) ;
					sGf22_15 = CYCLE / ( rT2sim + CYCLE ) ;
					sPv0Sim_15 = 1.29 * PVDT0 * rT2sim + PV0 ;
					srPVsim2_15 = rPvFil - sPv0Sim_15 ;
					srPVsim1_15 = rT2sim * PVDT + srPVsim2_15 ;
					rT1sim = 3.57 * TU ;
					rT2sim = 3.57 * TU ;
					rKsim = 0.097 * KIG * TU ;
					sGf11_20 = rT1sim / ( rT1sim + CYCLE ) ;
					sGf12_20 = CYCLE / ( rT1sim + CYCLE ) * rKsim *  siSpDir  ;
					sGf21_20 = rT2sim / ( rT2sim + CYCLE ) ;
					sGf22_20 = CYCLE / ( rT2sim + CYCLE ) ;
					sPv0Sim_20 = 1.6 * PVDT0 * rT2sim + PV0 ;
					srPVsim2_20 = rPvFil - sPv0Sim_20 ;
					srPVsim1_20 = rT2sim * PVDT + srPVsim2_20 ;
					rT1sim = 1.247 * TU ;
					rT2sim = 1.247 * TU ;
					rKsim = 0.046 * KIG * TU ;
					sGf11_30 = rT1sim / ( rT1sim + CYCLE ) ;
					sGf12_30 = CYCLE / ( rT1sim + CYCLE ) * rKsim *  siSpDir ;
					sGf21_30 = rT2sim / ( rT2sim + CYCLE ) ;
					sGf22_30 = CYCLE / ( rT2sim + CYCLE ) ;
					sPv0Sim_30 = 3.0 * PVDT0 * rT2sim + PV0 ;
					srPVsim3_30 = rPvFil - sPv0Sim_30 ;
					srPVsim2_30 = rT2sim * PVDT + srPVsim3_30 ;
					srPVsim1_30 = rT2sim * PVDT + srPVsim2_30 ;
				}
			}
			if( POI_CYCL > 0 )
			{
				gf = PVDT_MAX - PVDT ;
				if( srNoise < gf )
				{
					srNoise = gf ;
				}
			}
		}
		break;
		case 3:	 //���̲����ļ���.����ǰ��Ч�Ŀ���������������.
		{
			srTime = T_P_INF ;
			if( TU < 3.0 * CYCLE )
			{
				tw_durch_tu = srTime / TU ;
				TU = 3.0 * CYCLE ;
				srTime = TU * tw_durch_tu ;
				STATUS_H = STATUS_H + 100 ;
			}
			else if( TU > 0.7 * srTime )
			{
				TU = 0.7 * srTime ;
				STATUS_H = STATUS_H + 300 ;
			}
			tw_durch_tu = srTime / TU ;
			TA = tw_durch_tu * 0.76 * tw_durch_tu - 0.044 ;
			if( tw_durch_tu < 2.469 )
			{
				TA = ( tw_durch_tu - 2.469 ) * 0.48 + TA ;
			}
			gf = TA - 0.3954 ;
			N_PTN = ( gf * 1.1 + 7.9826 ) / gf ;
			TA = TA * TU ;
			if( N_PTN < 1.01 )
			{
				N_PTN = 1.01 ;
				STATUS_H = STATUS_H + 1000 ;
			}
			else if( N_PTN > 10 )
			{
				N_PTN = 10 ;
				STATUS_H = STATUS_H + 2000 ;
			}
			srNoise = NOI_PVDT * 0.01 * ( PVDT_MAX - PVDT0 ) ;
			GAIN_P = 0.01 * KIG * TA ;
			if( STATUS_H < 10 )
			{
				STATUS_H = 10000 ; //�����ѽ����������Ѿ��ҵ����ʵĿ���������
			}
			else
			{
				STATUS_H = STATUS_H + 20000 ;
			}
			STATUS_D = 0 ; //û�м����κο���������
			PAR_SAVE.PFAC_SP = PFAC_SP ;
			PAR_SAVE.GAIN = GAIN ;
			PAR_SAVE.TI = TI ;
			PAR_SAVE.TD = TD ;
			PAR_SAVE.D_F = D_F ;
			PAR_SAVE.CONZ_ON = CONZ_ON ;
			PAR_SAVE.CON_ZONE = CON_ZONE ;
			PHASE = 4 ;
		}
		break;
		case 4:	 //�����������
		{
			TM_LAG_P = ( srTime + CYCLE ) / ( N_PTN - 1.0 ) ;
			if( N_PTN >= 2.1 )
			{
				PI_CON.GAIN = ( N_PTN + 2.0 ) / ( N_PTN - 1.0 ) / 4.0 / GAIN_P ;
				PI_CON.TI = ( 2.0 + N_PTN ) * TM_LAG_P / 3.0 ;
				koeff = 7.0 * N_PTN + 16.0 ;
				if( N_PTN <= 2.6 )
				{
					PID_CON.GAIN = 3.5625 / GAIN_P ;
					STATUS_D = 310 ;//N_PTN >= 2.1����������III������
				}
				else
				{
					PID_CON.GAIN = koeff / ( N_PTN - 2.0 ) / 16.0 / GAIN_P ;
					STATUS_D = 320 ; //N_PTN > 2.6����������III
				}
				PID_CON.TI = koeff * TM_LAG_P / 15.0 ;
				PID_CON.TD = ( 1.0 + N_PTN ) * ( 3.0 + N_PTN ) * TM_LAG_P / koeff ;
				CONZ_ON = FALSE ;
			}
			else
			{
				STATUS_D = 110 ;//N_PTN <= 1.5����������I������
				CONZ_ON = TRUE ;
				PI_CON.TI = 6.0 * TU ;
				PI_CON.GAIN = 44.0 / TU / KIG ;
				PID_CON.TI = 2.0 * TU ;
				PID_CON.GAIN = 2.727272 * PI_CON.GAIN ;
				gf = 2.0 ;
				if( N_PTN > 1.5 )
				{
					gf = gf - ( N_PTN - 1.5 ) * 2.5 ;
					STATUS_D = 121 ;//N_PTN > 1.5����������I
				}
				if( N_PTN > 1.9 )
				{
					PI_CON.GAIN = PI_CON.GAIN * gf ;
					PID_CON.TI = ( ( 1.0 - gf ) * 4.0 + 1.0 ) * PID_CON.TI ;
					STATUS_D = 200 ;//N_PTN > 1.9����������II (�м䷶Χ)
				};
				PID_CON.GAIN = PID_CON.GAIN * gf ;
				PID_CON.TD = PID_CON.TI / 4.0 ;
			}
			PFAC_SP = 0.8 ;
			D_F = 5.0 ;
			if( PID_ON )
			{
				GAIN = PID_CON.GAIN ;
				TI = PID_CON.TI ;
				TD = PID_CON.TD ;
			}
			else
			{
				GAIN = PI_CON.GAIN ;
				TI = PI_CON.TI ;
				TD = 0.0 ;
				CONZ_ON = FALSE ;
			}
			CON_ZONE = 250.0 / GAIN ;
			if( CON_ZONE < 0.0 )
			{
				CON_ZONE = - CON_ZONE ;
			}
			if( ! TUN_KEEP )
			{
				srOptLmn = 0.75 * srEffDlmn + LMN0 ;
				srEffDlmn = 0.0 ;
				PHASE = 5 ;
			}
			else
			{
				STATUS_D = 0 ;//û�м����κο���������
			}
			
		}
		break;
		case 5:		//ʹ�����������µĿɵ��ڱ���
		{
			sbOptLmn = FALSE ;
			MAN_ON = FALSE ;
			if( sbReOpt )
			{
				PHASE = 0 ;
				TUN_ON = FALSE ;
				if( N_PTN <= 1.9 )
				{
					gf = ( 2.1 - N_PTN ) * 5.0 ;
					TA = gf * TA ;
					GAIN_P = GAIN_P * gf ;
				}
				STATUS_D = STATUS_D + 1 ; //111��122��201��311��321�ڽ׶�7 ����������
			}
			else
			{
				srTime = 0.0 ;
				PHASE = 7 ;
				srDelPvsim_30 = 0.0 ;
				srDelPvsim_20 = 0.0 ;
				srDelPvsim_15 = 0.0 ;
			}
		}
		break;
		case 7:	 //����������
		{
			srTime = srTime + rCycle ;
			if( srTime >= 0.3 * TA )
			{
				srDelPvsim_15 = ( srPVsim2_15 + sPv0Sim_15 ) - PV + srDelPvsim_15 ;
				srDelPvsim_20 = ( srPVsim2_20 + sPv0Sim_20 ) - PV + srDelPvsim_20 ;
				srDelPvsim_30 = ( srPVsim2_30 + sPv0Sim_30 ) - PV + srDelPvsim_30 ;
				if( srTime >= 0.35 * TA )
				{
					sNpInf = N_PTN ;
					if( srDelPvsim_15 < 0.0 )
					{
						if( N_PTN < 1.5 )
						{
							sbReOpt = FALSE ;
						}
						else
						{
							N_PTN = 1.5 ;
							sbReOpt = TRUE ;
						}
					}
					else if( srDelPvsim_30 > 0.0 )
					{
						if( N_PTN > 3.0 )
						{
							sbReOpt =FALSE ;
						}
						else
						{
							N_PTN = 3.0 ;
							sbReOpt = TRUE ;
						}
					}
					else 
					{
						if( srDelPvsim_15 > 0.0 && srDelPvsim_20 < 0.0 )
						{
							rSumHp = - srDelPvsim_20 ;
							rSumLp = srDelPvsim_15 ;
							rHp = 2.0 ;
							rLp = 1.5 ;
						}
						else if ( srDelPvsim_20 > 0.0 && srDelPvsim_30 < 0.0 )
						{
							rSumHp = - srDelPvsim_30 ;
							rSumLp = srDelPvsim_20 ;
							rHp = 3.0 ;
							rLp = 2.0 ;
						}
						rNptn = ( rSumHp * rLp + rSumLp * rHp ) / ( rSumHp + rSumLp ) ;
						rNptnHl = rNptn + 0.1 ;
						rNptnLl = rNptn - 0.1 ;
						if( N_PTN > rNptnHl )
						{
							N_PTN = rNptnHl ;
							sbReOpt = TRUE ;
						}
						else if( N_PTN < rNptnLl )
						{
							N_PTN = rNptnLl ;
							sbReOpt = TRUE ;
						}
						else
						{
							sbReOpt = FALSE ;
						}
					}
					if( sbReOpt )
					{
						srOptLmn = sLmn ;
						sbOptLmn = TRUE ;
						PHASE = 4 ;
					}
					else
					{
						TUN_ON = FALSE ;
						PHASE = 0 ;
						if( N_PTN <= 1.9 )
						{
							gf = ( 2.1 - N_PTN ) * 5.0 ;
							TA = gf * TA ;
							GAIN_P = GAIN_P * gf ;
						}
					}
				}
			}
		}
		break;
		default:
		break;
	}
	sPvOld = rPVsigned ;
}