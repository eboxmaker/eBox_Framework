/**
  ******************************************************************************
  * @file    DateTiemTest.cpp
  * @author  shentq
  * @version V2.0
  * @date    2021/07/23
  * @brief   ebox application example .
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form
  * or means, without the prior written consent of shentq. This specification is
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/


#include "ebox.h"
#include "att7022/att7022.h"

Att7022 at(&PA3,&PA4,&spi1);
PhaseData_t APhaseData;
void readAll(void);

void setup()
{
    ebox_init();
    uart1.begin(115200);
    at.begin();
    uart1.printf("0x%08x\n",at.read_id());

}
int main(void)
{

    setup();
    while(1)
    {
        readAll();
        uart1.printf("=======A�����=======\n");
        uart1.printf("A��Ч��ѹ:%0.1f\n",APhaseData.urms.value);
        uart1.printf("A��Ч����:%0.1f\n",APhaseData.irms.value);
        uart1.printf("A��ѹ������λ��:%0.1f\n",APhaseData.pg.value);
        uart1.printf("�й�����:%0.1f\n",APhaseData.p.value);
        uart1.printf("�޹�����:%0.1f\n",APhaseData.pq.value);
        uart1.printf("���ڹ���:%0.1f\n",APhaseData.ps.value);
        uart1.printf("��������:%0.1f\n",APhaseData.pf.value);
        uart1.printf("����:%0.1f\n",APhaseData.p.value);
        
        delay(1000);
    }
}
void readAll(void)
{
	u8 x,i,j;
    //���²���
	APhaseData.urms.value = at.ReadURms(APhase);;
	APhaseData.irms.value = at.ReadIRms(APhase);
	APhaseData.pg.value = at.ReadPG(APhase);//��ѹ������λ��
	APhaseData.p.value = at.ReadP(APhase);//�й�����
	APhaseData.pq.value = at.ReadQ(APhase);//�޹�����
	APhaseData.pf.value = at.ReadPF(APhase);//��������
	APhaseData.ps.value = at.ReadS(APhase);//���ڹ���
	APhaseData.ep.value = at.ReadEP(APhase);//����
	APhaseData.epKwh.value = at.ReadEPKwh(APhase);//����

    
}






