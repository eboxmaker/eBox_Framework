#ifndef __CAN_H
#define __CAN_H


#include "ebox_common.h"
#include "FunctionPointer.h"

 /**
 *@brief    CAN接收消息使用过滤器方式以及其设置
 基本使用方法1：发送
    -1.初始化    
        can1.begin(BSP_CAN_500KBPS);
    -2.设置CanTxMsg帧信息： 
        StdId/ExtId(标准/拓展ID),RTR(数据帧/远程帧),
        IDE(和前面的Id对应),DCL(数据长度),Data(数据内容)
    -3.发送数据        
         can1.write(&TxMessage);
基本使用方法2：接收
    -1.初始化    
        can1.begin(BSP_CAN_500KBPS);
    -2.设置过滤器
        --1.掩码模式 can1.set_filter_idmask(CAN_ID_STD,0,0x321,0XFFFFFFFF);
        或者
        --2.列表模式 can1.set_filter_idlist(CAN_ID_STD,0,0X321);
    -3.查询是否有数据接收到，如果接收到，然后用户可以自行处理
        if(can1.available())
        {
            can1.read(&RxMessage);
            uart1.printf("Rx:%s\n",RxMessage.Data);
        }

*/


typedef void (*can_irq_handler)(uint32_t id);

typedef enum
{
    BSP_CAN_33KBPS = 0,              // 波特率 33K 模式
    BSP_CAN_50KBPS,                  // 波特率 50K 模式
    BSP_CAN_625KBPS,                 // 波特率 62.5K 模式
    BSP_CAN_96KBPS,                  // 波特率 96K 模式
    BSP_CAN_100KBPS,                 // 波特率 100K 模式
    BSP_CAN_125KBPS,                 // 波特率 125K 模式
    BSP_CAN_250KBPS,                 // 波特率 250K 模式
    BSP_CAN_500KBPS,                 // 波特率 500K 模式
    BSP_CAN_800KBPS,                 // 波特率 800K 模式
    BSP_CAN_1MBPS,                   // 波特率 1M模式
}BSP_CAN_BAUD;

class Can
{
    public:
      	Can(Gpio* p_pin_rx, Gpio* p_pin_tx);
        void    begin(BSP_CAN_BAUD bps);
        void    set_filter(u8 Fifo,u8 nCanType,u8 num,u32 ID,u32 Mask);
        void    interrupt(FunctionalState enable, uint8_t preemption_priority = 0, uint8_t sub_priority = 0);
        void    attach_interrupt(void (*callback_fun)(void));
        void    set_filter_idlist(u8 nCanType,u32 ID);
        void    set_filter_idmask(u8 nCanType,u8 num,u32 ID,u32 mask);
        void    set_filter_idlist(u8 nCanType,u8 num,u32 ID);
        u8      write(CanTxMsg *pCanMsg);
        void    read(CanRxMsg *pCanMsg);
        u8      available(void);
        /** Attach a function to call whenever a serial interrupt is generated
         *
         *  @param fptr A pointer to a void function, or 0 to set as none
         *  @param type Which serial interrupt to attach the member function to (Seriall::RxIrq for receive, TxIrq for transmit buffer empty)
         */
        void attach(void (*fptr)(void));

        /** Attach a member function to call whenever a serial interrupt is generated
         *
         *  @param tptr pointer to the object to call the member function on
         *  @param mptr pointer to the member function to be called
         *  @param type Which serial interrupt to attach the member function to (Seriall::RxIrq for receive, TxIrq for transmit buffer empty)
         */
        template<typename T>
        void attach(T* tptr, void (T::*mptr)(void)) {
            if((mptr != NULL) && (tptr != NULL)) {
                _irq.attach(tptr, mptr);
            }
        }
            
        static void _irq_handler(uint32_t id);    
    private:
    	void set_bps(BSP_CAN_BAUD);
    	
    private:
	    CAN_TypeDef* _CANx;
        Gpio* pin_rx;           //arduino pin number
        Gpio* pin_tx;           //arduino pin number    

        BSP_CAN_BAUD _bps;  
protected:
    FunctionPointer _irq;		
};

#endif
