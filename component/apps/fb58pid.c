//VAR_INPUT
#ifndef TRUE
  #define TRUE 0x01
#endif 
#ifndef FALSE
  #define FALSE 0x00
#endif 
float    PV_IN = 0.0 ;   //过程变量输入
//可以在“过程变量输入”输入端设置初始值，或者也可以连接到浮点数格式的外部过程变量上。
int      PV_PER  = 0 ;      //外围设备过程变量 外围设备I/O 格式的过程变量连接到控制器的“外围设备过程变量”输入端。
float    DISV = 0.0 ;   //干扰变量 对于前馈控制，干扰变量连接到“干扰变量”输入端。
unsigned char    INT_HPOS = FALSE ; //积分作用保持在正方向	积分作用的输出可以锁定在正方向。为此，INT_HPOS 输入端必须设置成TRUE。在级联控制中，主控制器的INT_HPOS 互连到次级控制器的QLMN_HLM。
unsigned char    INT_HNEG = FALSE ; //积分作用保持在负方向
//积分作用的输出可以锁定在负方向。为此，INT_HNEG 输入端必须设置成TRUE。在级联控制中，主控制器的INT_HNEG 互连到次级控制器的QLMN_LLM。
int    SELECT = 0 ;      //调用PID 和脉冲发生器的选项
//如果激活了脉冲发生器，则有几种方法可用于调用PID 算法和脉冲发生//器：
//. SELECT =0：在一个快速周期性中断级中调用控制器，并处理PID算法和脉冲发生器。
//. SELECT =1：在OB1 中调用控制器，并且只处理PID 算法。
//. SELECT =2：在一个快速周期性中断级中调用控制器，并只处理脉冲发生器。
//. SELECT =3：在一个慢速周期性中断级中调用控制器，并只处理PID 算法。
//END_VAR
//VAR_OUTPUT
float    PV = 0.0 ;   //过程变量   有效的过程变量在“过程变量”输出端输出。
float    LMN = 0.0 ;   //可调节变量	可调节变量的有效值以浮点数格式， 在“可调节变量”输出端输出。
int    LMN_PER = 0 ;      //外围设备操作可调节变量 外围设备格式的可调节变量值连接到控制器的“外围设备操作可调节变量”输出端。
unsigned char    QPULSE = FALSE ; //输出脉冲信号 可调节变量的值是在QPULSE 输出端调制的输出脉冲宽度。
unsigned char    QLMN_HLM = FALSE ; //达到可调节变量的上限 可调节变量的值始终限制在上限和下限范围之内。当超过了上限时，通过QLMN_HLM 输出端进行指示。
unsigned char    QLMN_LLM = FALSE ; //达到可调节变量的下限 可调节变量的值始终限制在上限和下限范围之内。当超过了下限时，通过QLMN_LLM 输出端进行指示。
unsigned char    QC_ACT = TRUE ;  //下一个循环周期，连续控制器继续工作
//该参数指示在下一个块调用时是否执行连续控制器环节(只有当SELECT 具有数值0 或1 时才相关)。
//END_VAR
//VAR_IN_OUT
float    CYCLE = 0.1 ;   //连续控制器的采样时间[秒]
//在此设置PID 算法的采样时间。调谐器在阶段1 中计算采样时间，然后将其输入到CYCLE 中。
float    CYCLE_P = 0.02 ;  //脉冲发生器的采样时间[秒]
//在该输入端，输入脉冲发生器环节的采样时间。FB 58“TCONT_CP”在阶段1 中计算采样时间，然后将其输入到CYCLE_P。
float    SP_INT = 0.0 ;   //内部设定值 “内部设定值”输入端用于指定一个设定值。
float    MAN = 0.0 ;   //手动值	“手动值”输入端用于指定一个手动值。在自动模式中，它被改成可调节变量。
unsigned char    COM_RST = FALSE ; //完全重启动	该块有一个初始化例行程序，当COM_RST 输入端被置位时执行该例行程序。
unsigned char    MAN_ON = TRUE;   //手动操作打开 如果“手动操作打开”输入被置位，则中断控制回路。MAN 手动值被设置为可调节变量的值。
//END_VAR
//VAR
float    DEADB_W = 0.0 ;   //死区宽度 偏差通过死区。“死区宽度”输入决定死区大小。
float    I_ITLVAL = 0.0 ;   //积分作用的初始化值
//积分作用的输出可以在I_ITL_ON 输入端上设置。该初始化值用于“积分作用的初始化值”输入。在重启动期间，
//COM_RST = TRUE，积分作用被设置成初始值。
float    LMN_HLM = 100.0;  //可调节变量上限
//可调节变量的值始终限制在上限和下限范围之内。“可调节变量上限”输入指定了上限。
float    LMN_LLM = 0.0 ;   //可调节变量下限
//可调节变量的值始终限制在上限和下限范围之内。“可调节变量下限”输入指定了下限。
float    PV_FAC = 1.0 ;   //过程变量因子
//“过程变量因子”输入和“外围设备过程值”相乘。该输入用于调整过程变量的范围。
float    PV_OFFS = 0.0 ;   //过程变量偏移量
//“过程变量偏移量”输入被加到“外围设备过程变量”上。该输入用于调整过程变量的范围。
float    LMN_FAC = 1.0 ;   //可调节变量因子
//“可调节变量因子”输入和可调节变量相乘。该输入用于调整可调节变量的范围。
float    LMN_OFFS = 0.0 ;   //可调节变量偏移量
//“可调节变量偏移量”输入加到可调节变量的数值上。该输入用于调整可调节变量的范围。
float    PER_TM = 1.0 ;   //周期[秒]
//脉宽调制的脉冲重复周期在PER_TM 参数上输入。脉冲发生器的脉冲重复周期和采样时间之间的关系决定脉宽调制的精度。
float    P_B_TM = 0.0 ;   //最小脉冲/断开时间[秒]
//最小脉冲或最小断开时间可以在“最小脉冲/断开时间”参数上设置。P_B_TM 在内部被限制为大于CYCLE_P。
float    TUN_DLMN = 20.0 ;  //用于过程激励的可调节变量增量
//用于控制器整定的过程激励来自于TUN_DLMN 上的设定值阶跃变化。
int    PER_MODE = 0 ;      //外围设备模式可以在这个开关上输入I/O 模块的类型。输入PV_PER 上的过程变量在PV 输出上被规格化成°C。
//. PER_MODE =0：标准
//. PER_MODE =1：气候
//. PER_MODE =2：电流/电压
unsigned char    PVPER_ON = FALSE ; //外围设备过程变量打开
//如果想从I/O 中读取过程变量，则PV_PER 输入必须连接到I/O，并且“外围设备过程变量”输入必须置位。
unsigned char    I_ITL_ON = FALSE ; //积分作用的初始化打开
//积分作用的输出可以设置为I_ITLVAL 输入。“设置积分作用”输入必须置位。
unsigned char    PULSE_ON = FALSE ; //脉冲发生器打开
//如果设置了PULSE_ON = TRUE，则脉冲发生器被激活。
unsigned char    TUN_KEEP = FALSE ; //保持整定打开
//只有当TUN_KEEP 变成FALSE时，工作模式才切换成自动。
float    ER = 0.0 ;   //偏差信号  有效的偏差在“偏差信号”输出端输出。
float    LMN_P = 0.0 ;   //比例分量	“比例分量”包含了可调节变量的比例作用。
float    LMN_I = 0.0 ;   //积分分量	“积分分量”包含了可调节变量的积分作用。
float    LMN_D = 0.0 ;   //微分分量	“微分分量”包含了可调节变量的微分作用。
int    PHASE = 0 ;      //自整定的阶段 控制器整定的当前阶段在PHASE 输出端指示(0..7)。
int    STATUS_H = 0 ;      //自整定的状态加热 在加热时，STATUS_H 指示搜索拐点的诊断值。
int    STATUS_D = 0 ;      //自整定的状态控制器设计	当加热时，STATUS_D 指示控制器设计的诊断值。
unsigned char    QTUN_RUN = FALSE ; //整定已激活(PHASE 2)
//已经应用了整定可调节变量，整定已经开始，并仍然处于第2 阶段(定位拐点)。
struct 
{
         //PI 控制器参数
	float        GAIN; //= 0.0 ;   //PI 比例增益
	float        TI; //= 0.0 ;   //PI 复位时间[s]
}PI_CON; 
struct 
{  
          //PID 控制器参数
	float        GAIN; //= 0.0 ;   //PID 比例增益
	float        TI; //= 0.0 ;   //PID 复位时间[s]
	float        TD; //= 0.0 ;   //PID 微分时间[s]
} PID_CON; 
struct 
{
         //保存的控制器参数	PID 参数保存在此结构中。
	float        PFAC_SP; //= 1.0 ;   //用于设定值变化的比例因子[0..1]
	float        GAIN; //= 0.0 ;   //比例增益
	float        TI; //= 0.0 ;   //复位时间[s]
	float        TD; //= 0.0 ;   //微分时间[s]
	float        D_F; //= 5.0 ;   //微分因子TD/TM_LAG (5..10)
	float        CON_ZONE; //= 100.0 ; //控制带打开
	unsigned char        CONZ_ON; //= FALSE ; //控制带
} PAR_SAVE;  
float    PFAC_SP = 1.0 ;   //用于设定值变化的比例因子
//当有设定值变化时，PFAC_SP 指定有效的比例P 作用。它的设置范围是0 到1。
//. 1：如果设定值发生变化，则比例P 作用具有全部效果。
//. 0：如果设定值发生变化，比例P 作用没有任何效果。
float    GAIN = 2.0 ;   //比例增益
//“比例增益”输入用于指定控制器增益。可以通过给GAIN 一个负号来反转控制方向。
float    TI = 40.0 ;  //复位时间[秒]
//“复位时间”输入(积分时间)决定了积分作用响应。
float    TD = 10.0 ;  //微分时间[秒]
//“微分时间”输入决定了微分作用响应。
float    D_F = 5.0 ;   //微分因子
//微分因子描述了D 作用的滞后。D_F = 微分时间/“D 作用的滞后”
float    CON_ZONE = 100.0 ; //控制带
//如果偏差大于控制带宽度，将输出可调节变量上限作为实际的可调节变量。
//如果偏差小于控制带宽度，将输出可调节变量下限作为实际的可调节变量。
unsigned char    CONZ_ON = FALSE ; //控制带打开	CONZ_ON =TRUE 激活控制带。
unsigned char    TUN_ON = FALSE ; //自整定打开
//如果设置了TUN_ON=TRUE，则可调节变量被取平均，直到设定值发生阶跃变化或TUN_ST=TRUE 将可调节变量激励TUN_DLMN 激活。
unsigned char    TUN_ST = FALSE ; //开始自整定
//如果要使设定值在控制器整定期间在工作点上保持不变，则通过TUN_ST=1 来激活可调节变量发生大小为TUN_DLMN 的阶跃变化。
unsigned char    UNDO_PAR = FALSE ; //撤消对控制器参数的更改
//从数据结构PAR_SAVE 中装载控制器参数PFAC_SP、GAIN、TI、  TD、D_F CONZ_ON 和CON_ZONE (只在手动模式中)。
unsigned char    SAVE_PAR = FALSE ; //保存当前控制器参数
//将控制器参数PFAC_SP、GAIN、TI、TD、D_F CONZ_ON 和CON_ZONE 保存到数据结构PAR_SAVE 中。
unsigned char    LOAD_PID = FALSE ; //装载优化的PI/PID 参数
//根据PID_ON，从数据结构PI_CON 或PID_CON 中装载控制器参数GAIN、TI、TD (仅在手动模式中)
unsigned char    PID_ON = TRUE ;  //PID 模式打开
//在PID_ON 输入上，可以指定整定过的控制器是否作为PI 控制器或PID 控制器运行。
//. PID 控制器：PID_ON = TRUE
//. PI 控制器：PID_ON = FALSE对于某些过程类型来说，尽管PID_ON = TRUE，但仍然可以只设计PI 控制器。
float    GAIN_P = 0.0 ;   //过程比例增益
//识别出的过程增益。对于过程类型I来说，GAIN_P 趋向于被估计得低一些。
float    TU = 0.0 ;   //延迟时间[秒] 识别出的过程延迟。
float    TA = 0.0 ;   //恢复时间[秒]
//识别出的过程的系统时间常数。对于过程类型I 来说，TA 趋向于被估计得低一些。
float    KIG = 0.0 ;   //PV WITH 100 % LMN 变化的最大斜率GAIN_P = 0.01 * KIG * TA
float    N_PTN = 0.0 ;   //处理顺序 该参数指定处理的执行顺序。也可以指定“非整数值”。
float    TM_LAG_P = 0.0 ;   //PTN 模型的时间延迟[秒] PTN 模型的时间延迟(有效值只用于N_PTN >= 2)。
float    T_P_INF = 0.0 ;   //到拐点的时间[秒] 从过程激发到拐点所经历的时间。
float    P_INF = 0.0 ;   //拐点处的PV - PV0 从过程激发到拐点处，过程变量发生的变化。
float    LMN0 = 0.0 ;   //整定开始时的可调节变量   在阶段1 中检测(平均值)。
float    PV0 = 0.0 ;   //整定开始时的过程值
float    PVDT0 = 0.0 ;   //整定开始时PV 的变化率[1/s]  采用有符号数。
float    PVDT = 0.0 ;   //PV 的当前变化率[1/s] 采用有符号数。
float    PVDT_MAX = 0.0 ;   //每秒钟PV 的最大变化率[1/s]
//拐点处过程变量的最大变化率(采用有符号数，始终大于0)，用于计算TU 和KIG。
float    NOI_PVDT = 0.0 ;   //PVDT_MAX 中的噪声比率，采用%形式 噪声的比例越高，控制参数的精度就越低(越不积极)。
float    NOISE_PV = 0.0 ;   //PV 中的绝对噪声  阶段1 中最大和最小过程变量的差值。
int    FIL_CYC = 1 ;      //均值过滤器的周期数	过程变量取FIL_CYC 个周期的平均值。如果需要，FIL_CYC 可以自动从1 增加到最大值1024。
int    POI_CMAX = 0 ;      //拐点后的最大周期数
//该时间用于在测量噪声存在的情况下，找到更进一步的(或者说，更好的)拐点。只有在此时间过后，整定才结束。
int    POI_CYCL = 0 ;      //拐点后的周期数
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
float    sInvAlt = 0.0 ;   //上周期比例偏差值 
float    sIanteilAlt = 0.0 ;  //上周期积分值  
float    sRestInt = 0.0 ;   //上周期积分偏差量（浮点数计算偏差） 
float    sRestDif = 0.0 ;   //上周期微分偏差量（浮点数计算偏差） 
float    sRueck = 0.0 ;    
float    sLmn = 0.0 ;     //上周期调节值
float   spassPTm = 0.0 ;    
float    sCycTmPass = 0.0 ;    
float    sPTm = 0.0 ;    
float    srOptLmn = 0.0 ;    
unsigned char    sbOptLmn = FALSE ;    
unsigned char    sbConzOn = FALSE ;    
unsigned char   sbSpChange = FALSE ;  //设定值是否变化  
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
//float    rErKp  ; //偏差/比例偏差
float    rDiff  ;
float    rDiff_int  ; //偏差临时量，用于判断是否超出控制带
float    Verstaerk  ;
float    RueckDiff  ;
float    RueckAlt  ;  //上周期积分量
float    dLmn  ;	  //调节量
//unsigned char    bUpp ;
//unsigned char    bDownp ;
//unsigned char    bUpn ;
//unsigned char    bDownn ;
//unsigned char    bHvar ;
unsigned char    bIZv ;
unsigned char    bLMNnew ;
unsigned char    bPulse ;
unsigned char    bConzOn ;	 //控制带被执行
float    rSeek  ;
float    rPvDelt  ;
float   rPvFil  ;
float   rPV0  ;
float    rPvdtOri  ;
float    rPVsigned  ;
float    rSpDelt  ;	 //设定点的变化
float   tw_durch_tu  ;
float    gf  ;
float    koeff  ;
float    rCycle  ;	//步进控制器的采样时间（s）
float    rTmLag  ;	//微分作用时间延时浮点值
float    rDeltaI  ;	//积分值的变化
float    rVal  ;
float    rPvNew2  ;
float    rNoiPvdt  ;
float    rPvdtMax0  ;
float    rPVin  ;
float    rP_B_TM  ;
float    rPvdt  ;
float    rPvdt2  ;
float    rPvdt3  ;	 // srPvdt2 初始值
//float    rLmnPmod  ; //相称组件CONZONE修改
int    iFilCyc  ;	 //内部：周期号为中值滤波
int    iVal  ;    
float    rT1sim  ;	 //模拟时间常数
float    rT2sim  ;	 //模拟时间常数
float    rKsim  ;
float    rSumHp  ;
float    rSumLp  ;
float    rHp  ;
float    rLp  ;
float    rNptn  ;	//通过模拟顺序确定
float    rNptnHl  ;	//通过仿真上限决定的顺序
float    rNptnLl  ;	//通过仿真下限确定的顺序

//END_VAR






void pid_tune(void);





void pid(void)
{
	if( COM_RST )//复位
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
		if( PVPER_ON )	//过程变量输入选择
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
			if( LOAD_PID && MAN_ON )  //判断是否是PID和手动控制
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
			if( UNDO_PAR && MAN_ON && PAR_SAVE.GAIN != 0.0 )//重新装载参数
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
			if( SAVE_PAR )	//存储参数
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
			iFilCyc = FIL_CYC ;//均值过滤器的周期数
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
			if( ! TUN_ON )	//如果不需要整定
			{
				TUN_ST = FALSE ;
				sbSpChange = FALSE ;
				sbOptLmn = FALSE ;
				srOptLmn = 0.0 ;
				srEffDlmn = 0.0 ;
				PHASE = 0 ;
				QTUN_RUN = FALSE ;
			}
			if( sctFil == 0 && TUN_ON )	 //判断是否需要整定
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
			rDiff = SP_INT - PV ;  //偏差
			rSpDelt = SP_INT - sSpOld ;
			if( rDiff < - DEADB_W )	  //检查偏差是否低于死区宽度的负值
			{
				rDiff = rDiff + DEADB_W ;
			}
			else if( rDiff > DEADB_W )//检查偏差是否高于死区宽度
			{
				rDiff = rDiff - DEADB_W ;
			}
			else
			{
				rDiff = 0.0 ;
			}
			ER = rDiff ;//偏差
			rTmLag = TD / D_F ;
			LMN_P = GAIN * ER ;	  //比例分量
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
				dLmn = LMN_P + LMN_I + DISV ;  //PI输出
				if( ! MAN_ON )	//当PI控制时，将PID输出送到手动MAN
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
				dLmn = LMN_P + LMN_I + DISV + LMN_D ;	//PID输出
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
	switch(PHASE)  //自整定过程
	{
		case 0:	 //无整定,自动或手动模式
		{
	//					tTime = TIME_TCK() ;
			siTime =  tTime  ;
			siCycle = 0 ;
			siCycleP = 0 ;
			FIL_CYC = 1 ;  //均值过滤器的周期数
			LMN0 = sLmn ;
			srPvMin = PV ;
			PV0 = rPvFil ;
			NOISE_PV = 0.0 ;
			siStatus = 1 ;
			siSpDir = 1 ;
			TUN_ST = FALSE ;
			SP_INT = sSpOld ;
			STATUS_H = 0 ;//缺省或(还)没有新的控制器参数。
			PHASE = 1 ;
		}
		break;
		case 1:		 //准备整定,检查参数,等待激励,测量采样时间.
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
					STATUS_H = 30002 ; //有效可调节变量差值 < 5 % ,修正可调节变量差值TUN_DLMN。
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
						STATUS_H = 30005 ;//采样时间CYCLE 和CYCLE_P 之间的差值超过了测量值的5%。
						//将CYCLE 和CYCLE_P 与周期性中断级的周期相比较，注意可能存在调用分配器。
						//检查CPU 的使用情况。如果CPU 处于高使用状态，则需要延长采样时间，该时间可以不和CYCLE 或CYCLE_P 匹配。
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
		case 2:		 //实际整定,等待在稳定的控制器输出值上检测到拐点.
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
				FIL_CYC = FIL_CYC * 2 ;		//均值过滤器的周期数
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
		case 3:	 //过程参数的计算.整定前有效的控制器参数被保存.
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
				STATUS_H = 10000 ; //整定已结束，并且已经找到合适的控制器参数
			}
			else
			{
				STATUS_H = STATUS_H + 20000 ;
			}
			STATUS_D = 0 ; //没有计算任何控制器参数
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
		case 4:	 //控制器的设计
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
					STATUS_D = 310 ;//N_PTN >= 2.1，过程类型III，快速
				}
				else
				{
					PID_CON.GAIN = koeff / ( N_PTN - 2.0 ) / 16.0 / GAIN_P ;
					STATUS_D = 320 ; //N_PTN > 2.6，过程类型III
				}
				PID_CON.TI = koeff * TM_LAG_P / 15.0 ;
				PID_CON.TD = ( 1.0 + N_PTN ) * ( 3.0 + N_PTN ) * TM_LAG_P / koeff ;
				CONZ_ON = FALSE ;
			}
			else
			{
				STATUS_D = 110 ;//N_PTN <= 1.5，过程类型I，快速
				CONZ_ON = TRUE ;
				PI_CON.TI = 6.0 * TU ;
				PI_CON.GAIN = 44.0 / TU / KIG ;
				PID_CON.TI = 2.0 * TU ;
				PID_CON.GAIN = 2.727272 * PI_CON.GAIN ;
				gf = 2.0 ;
				if( N_PTN > 1.5 )
				{
					gf = gf - ( N_PTN - 1.5 ) * 2.5 ;
					STATUS_D = 121 ;//N_PTN > 1.5，过程类型I
				}
				if( N_PTN > 1.9 )
				{
					PI_CON.GAIN = PI_CON.GAIN * gf ;
					PID_CON.TI = ( ( 1.0 - gf ) * 4.0 + 1.0 ) * PID_CON.TI ;
					STATUS_D = 200 ;//N_PTN > 1.9，过程类型II (中间范围)
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
				STATUS_D = 0 ;//没有计算任何控制器参数
			}
			
		}
		break;
		case 5:		//使控制器处理新的可调节变量
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
				STATUS_D = STATUS_D + 1 ; //111、122、201、311、321在阶段7 修正参数。
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
		case 7:	 //检查过程类型
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