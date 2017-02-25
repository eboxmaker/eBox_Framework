#ifndef __CAN_H
#define __CAN_H


#include "ebox_common.h"
#include "FunctionPointer.h"

 /**
 *@brief    CANĿǰֻ֧����CAN1������Ϣʹ�ù�������ʽ�Լ�������
 
 ����ʹ�÷���1������
    -1.��ʼ��    
        can1.begin(BSP_CAN_500KBPS);
    -2.����CanTxMsg֡��Ϣ�� 
        StdId/ExtId(��׼/��չID),RTR(����֡/Զ��֡),
        IDE(��ǰ���Id��Ӧ),DCL(���ݳ���),Data(��������)
    -3.��������        
         can1.write(&TxMessage);
         
����ʹ�÷���2����ѯ����
    -1.��ʼ��    
        can1.begin(BSP_CAN_500KBPS);
    -2.���ù�����
        --1.����ģʽ can1.set_filter_idmask(CAN_ID_STD,0,0x321,0XFFFFFFFF);
        ����
        --2.�б�ģʽ can1.set_filter_idlist(CAN_ID_STD,0,0X321);
    -3.��ѯ�Ƿ������ݽ��յ���������յ���Ȼ���û��������д���
        if(can1.available())
        {
            can1.read(&RxMessage);
            uart1.printf("Rx:%s\n",RxMessage.Data);
        }
����ʹ�÷���3���жϽ���
    -1.��ʼ��    
        can1.begin(BSP_CAN_500KBPS);
    -2.���ù�����
        --1.����ģʽ can1.set_filter_idmask(CAN_ID_STD,0,0x321,0XFFFFFFFF);
        ����
        --2.�б�ģʽ can1.set_filter_idlist(CAN_ID_STD,0,0X321);
    -3.���жϴ������������ж�
        can1.attach(test);
        can1.interrupt(ENABLE);
*/


typedef void (*can_irq_handler)(uint32_t id);

typedef enum
{
    BSP_CAN_33KBPS = 0,              // ������ 33K ģʽ
    BSP_CAN_50KBPS,                  // ������ 50K ģʽ
    BSP_CAN_625KBPS,                 // ������ 62.5K ģʽ
    BSP_CAN_96KBPS,                  // ������ 96K ģʽ
    BSP_CAN_100KBPS,                 // ������ 100K ģʽ
    BSP_CAN_125KBPS,                 // ������ 125K ģʽ
    BSP_CAN_250KBPS,                 // ������ 250K ģʽ
    BSP_CAN_500KBPS,                 // ������ 500K ģʽ
    BSP_CAN_800KBPS,                 // ������ 800K ģʽ
    BSP_CAN_1MBPS,                   // ������ 1Mģʽ
}BSP_CAN_BAUD;

class Can
{
    public:
      	Can(Gpio* p_pin_rx, Gpio* p_pin_tx);
    
        void    begin(BSP_CAN_BAUD bps);
    
        void    interrupt(FunctionalState enable, uint8_t preemption_priority = 0, uint8_t sub_priority = 0);
    
        /**
         *@brief    CAN������Ϣʹ�ù�������ʽ�Լ�������
         *@param    nCanType��CAN_ID_STD����CAN_ID_EXT
         *@param    num��0-13  ����������
         *@param    ID�������������IP
         *@param    mask���������������������
         *@retval   NONE
        */
        void    set_filter_idmask(u8 nCanType,u8 num,u32 ID,u32 mask);    
    
        /**
        *@brief     CAN������Ϣʹ���б�ķ�ʽֻ����ָ��ID�����ݣ����Ч��
                    ����������ȫ�����ó�1.
        *@param     nCanType��CAN_ID_STD����CAN_ID_EXT
        *@param     num��0-13  ����������
        *@param     ID����Ϣ֡ͷIDɸѡ����
        *@retval    NONE
        */
        void    set_filter_idlist(u8 nCanType,u8 num,u32 ID);

        u8      write(CanTxMsg *pCanMsg);
        
        void    read(CanRxMsg *pCanMsg);
        
        /**
         *@brief    ����CAN����FIFO�е���Ϣ��Ŀ�����Ϊ3���������3��ᵼ�����ݶ�ʧ
         *@retval   FIFO�е���Ϣ����
        */
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
