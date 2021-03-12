#ifndef __MIDEA_H
#define __MIDEA_H
#include "../ir_r05d.h"
#include "ebox_uart.h"

#define MIDEA_CODE_A   B10110010



class MideaNormal :public IrR05D
{
public:
    typedef enum {ModeAuto,ModeCool,ModeDehumidification,ModeHeat,ModeWind}Mode_t;
    typedef enum {WindAuto,WindLow,WindMid,WindHigh,WindConst}Wind_t;
    public:
        MideaNormal(Gpio &io):IrR05D(io)
        {
            A = MIDEA_CODE_A;
            B = B10111111;//·çËÙAuto
            C = B11010000;//26¡ã£¬Auto
            settings.temp = 26;
            settings.modeIndex = ModeCool;
            settings.windIndex = WindAuto;
        }
        typedef struct settings
        {
            Wind_t windIndex;
            uint8_t temp;
            Mode_t modeIndex;

        }settings_t;

        
        void cool_26();
        
        void wind(Wind_t value);
        void windNext();
        
        void temp(uint8_t value);
        void tempAdd();
        void tempSub();  
        
        void mode(Mode_t value);
        void modeNext();
        
        void send();
        void print(Uart &uart);
        void powerOff();
                
        settings_t settings;
        
        uint8_t A;
        uint8_t B;
        uint8_t C;
        
        private:
            String modetostr(Mode_t mode);
            String windtostr(Wind_t mode);
        
};



#endif

