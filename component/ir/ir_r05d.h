#ifndef __IR_R05D_H
#define __IR_R05D_H

#include "ebox_core.h"
#include "ebox_pwm.h"

#define CODE_L_L 4400
#define CODE_L_H 4400

#define CODE_S_L 540
#define CODE_S_H 5220

#define CODE_0_L 540
#define CODE_0_H 540

#define CODE_1_L 540
#define CODE_1_H 1620

#define CODE_E_L 540
#define CODE_E_H 2000


class IrR05D
{

    public:
        IrR05D(Gpio &io);
        void begin();
    public:
        void init(void);
        void send_byte(uint8_t data);
        void send_l();
        void send_s();
        void send_e();
        void send_ld(uint8_t data);
        Pwm *pwm;
};



#endif

