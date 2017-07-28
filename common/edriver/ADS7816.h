/**
  ******************************************************************************
  * @file    incapture.h
  * @author  shentq
  * @version V2.0
  * @date    2017/07/23
  * @brief   
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


/* Define to prevent recursive inclusion -------------------------------------*/


#ifndef __ADS7816_H
#define __ADS7816_H

#include "ebox.h"

#define v_ref 2500
class Ads7816
{
    public:
        Ads7816(Gpio *cs,Gpio *clk,Gpio *din)
		{
			this->cs    = cs;
			this->clk   = clk;
			this->din   = din;
		}
        void        begin(void);
        uint16_t    read(void);
        float       read_voltage(void);
        
    
    private:
		Gpio    *cs;
		Gpio    *clk;
		Gpio    *din;

};

#endif
