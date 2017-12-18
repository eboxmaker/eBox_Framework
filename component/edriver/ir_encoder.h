#ifndef  __IR_ENCODER_H
#define  __IR_ENCODER_H
/*
 * ir_encoder.h
 *
 * Version 1.0
 *
 * NEC IR encoder library for eBOX.
 *
 * See
 *
 * License:   GPL v3 (http://www.gnu.org/licenses/gpl.html)
 * Datasheet:
 *
 * Modification History:
 * -Link                                  - Version 1.0 (2016/01/06)
 *
 *
 *
 *
 */


#include "ebox.h"

class IR_ENCODER
{
public:
    void begin(Pwm *pwm);
    void send(uint32_t value);
private:
    void init(void);
    void send_byte(uint8_t data);
    Pwm *p_pwm;

};


#endif


