
#include "ebox_can.h"

static uint32_t can_irq_id;
static can_irq_handler irq_handler;

#ifdef __cplusplus
extern "C" {
#endif

void USB_LP_CAN1_RX0_IRQHandler(void)
{
    irq_handler(can_irq_id);
    CAN_FIFORelease(CAN1, CAN_FIFO0);	
}

void can_irq_init(can_irq_handler handler, uint32_t id)
{
    irq_handler = handler;
    can_irq_id = id;
}

#ifdef __cplusplus
}
#endif


void Can::interrupt(FunctionalState enable, uint8_t preemption_priority, uint8_t sub_priority)
{
    if(preemption_priority > 4)preemption_priority = 4;
    if(sub_priority > 4)sub_priority = 4;
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;	   //CAN1 RX0??
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		   //?????0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			   //?????0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    if(enable == ENABLE)
    {
        CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
    }
    else
    {
        CAN_ITConfig(CAN1, CAN_IT_FMP0, DISABLE);
    }
}

Can::Can(Gpio *p_pin_rx, Gpio *p_pin_tx)
{
    this->pin_rx = p_pin_rx;
    this->pin_tx = p_pin_tx;
}

const uint16_t CANBAUD[][4] =
{
    { CAN_SJW_1tq, CAN_BS1_6tq, CAN_BS2_3tq, 108 },     //33k
    { CAN_SJW_1tq, CAN_BS1_6tq, CAN_BS2_3tq, 72 },      //50k
    { CAN_SJW_1tq, CAN_BS1_3tq, CAN_BS2_5tq, 64 },      //62.5k
    { CAN_SJW_1tq, CAN_BS1_6tq, CAN_BS2_8tq, 25 },      //96k
    { CAN_SJW_1tq, CAN_BS1_6tq, CAN_BS2_3tq, 36 },      //100k
    { CAN_SJW_1tq, CAN_BS1_3tq, CAN_BS2_5tq, 32 },      //125k
    { CAN_SJW_1tq, CAN_BS1_3tq, CAN_BS2_5tq, 16 },      //250k
    { CAN_SJW_1tq, CAN_BS1_3tq, CAN_BS2_5tq, 8 },       //500k
    { CAN_SJW_1tq, CAN_BS1_9tq, CAN_BS2_5tq, 3 },       //800k
    { CAN_SJW_1tq, CAN_BS1_3tq, CAN_BS2_5tq, 4 },       //1M
};

void Can::set_bps(BSP_CAN_BAUD bps)
{
    CAN_InitTypeDef CAN_InitStructure;

    CAN_InitStructure.CAN_SJW = CANBAUD[bps][0];//CAN_SJW_1tq;
    CAN_InitStructure.CAN_BS1 = CANBAUD[bps][1];//CAN_BS1_6tq;
    CAN_InitStructure.CAN_BS2 = CANBAUD[bps][2];//CAN_BS2_8tq;
    CAN_InitStructure.CAN_Prescaler = CANBAUD[bps][3];//60;

    CAN_InitStructure.CAN_TTCM = DISABLE;   //禁用时间触发
    CAN_InitStructure.CAN_ABOM = ENABLE;    //自动离线管理
    CAN_InitStructure.CAN_AWUM = ENABLE;    //自动唤醒
    CAN_InitStructure.CAN_NART = ENABLE;    //错误不自动重传
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;

    CAN_Init(CAN1, &CAN_InitStructure);
}


 /**
 *@brief    CAN接收消息使用过滤器方式以及其设置
 *@param    nCanType：CAN_ID_STD或者CAN_ID_EXT
 *@param    num：0-13  过滤器组编号
 *@param    ID：类似于网络的IP
 *@param    mask：类似于网络的子网掩码
 *@retval   NONE
*/
void Can::set_filter_idmask(u8 nCanType,u8 num,u32 ID,u32 mask)
{
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;

    CAN_FilterInitStructure.CAN_FilterNumber=num;						
    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;		
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;

    if(nCanType == CAN_ID_STD)
    {/* std id  */
        CAN_FilterInitStructure.CAN_FilterIdHigh= ID<<5;
        CAN_FilterInitStructure.CAN_FilterIdLow=0|CAN_ID_STD;
    }
    else
    {
    /* ext id  */
        CAN_FilterInitStructure.CAN_FilterIdHigh= ((ID<<3)>>16) & 0xffff;
        CAN_FilterInitStructure.CAN_FilterIdLow= ((ID<<3)& 0xffff) | CAN_ID_EXT;
    }

    CAN_FilterInitStructure.CAN_FilterMaskIdHigh=(mask>>16)&0xffff;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow=mask&0xffff;

    CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0 ;
    CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);
}

 /**
 *@brief    CAN接收消息使用列表的方式只接收指定ID的数据，其等效于
            将子网掩码全部设置成1.
 *@param    nCanType：CAN_ID_STD或者CAN_ID_EXT
 *@param    num：0-13  过滤器组编号
 *@param    ID：消息帧头ID筛选设置
 *@retval   NONE
*/
void Can::set_filter_idlist(u8 nCanType,u8 num,u32 ID)
{
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;

    CAN_FilterInitStructure.CAN_FilterNumber=num;						
    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdList;	
    CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
    
    if(nCanType == CAN_ID_STD)
    {/* std id  */
        CAN_FilterInitStructure.CAN_FilterIdHigh= ID<<5;	
        CAN_FilterInitStructure.CAN_FilterIdLow= 0|CAN_ID_STD;
    }
    else
    {
        /* ext id  */
        CAN_FilterInitStructure.CAN_FilterIdHigh= ((ID<<3)>>16) & 0xffff;
        CAN_FilterInitStructure.CAN_FilterIdLow= ((ID<<3)& 0xffff) | CAN_ID_EXT;				
    }
    
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0;			
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0;			

    CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0 ;
    CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);
}

void Can::begin(BSP_CAN_BAUD bps)
{

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1 | RCC_APB1Periph_CAN2, ENABLE);

    /*to do */
    if(pin_rx->id == PB8_ID && pin_tx->id == PB9_ID )
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
        GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);
    }
    pin_rx->mode(INPUT_PU);
    pin_tx->mode(AF_PP);

    set_bps(bps);
    for(u8 i=0;i<3;i++)
        CAN_CancelTransmit(CAN1,i);
    can_irq_init(Can::_irq_handler, (uint32_t)this);
}

u8 Can::write(CanTxMsg *pCanMsg)
{
    u32 nTime = 0x3fff;
    u8 TMailbox;

    TMailbox = CAN_Transmit(CAN1, pCanMsg);

    while((CAN_TransmitStatus(CAN1, TMailbox) != CANTXOK) && (--nTime));

    if(nTime == 0)
    {
        CAN_CancelTransmit(CAN1, TMailbox);//发送错误时取消发送
        return false;
    }
    else
    {
        return true;
    }
}

void Can::read(CanRxMsg *pCanMsg)
{
    CAN_Receive(CAN1, CAN_FIFO0, pCanMsg );
}

u8 Can::available(void)
{
    return CAN_MessagePending(CAN1, CAN_FIFO0);
}

void Can::attach(void (*fptr)(void)) {
    if (fptr) {
        _irq.attach(fptr);
    }
}

void Can::_irq_handler(uint32_t id) {
    Can *handler = (Can*)id;
    handler->_irq.call();
}
