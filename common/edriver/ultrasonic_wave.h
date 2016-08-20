#ifndef __CSB_H
#define __CSB_H
#include "ebox.h"

class ULTRA:public InCapture
{
    public:
        float wave_time_us;
    public:
        ULTRA(Gpio *echo,Gpio *triger):InCapture(echo)
        {
            this->triger = triger;
        };
        void	begin();
        void	start();
				int		avaliable();
				float	read_cm();
				
				void	attch_mesuer_event(void(*callback)(void));
        void	mesure_event();
        
    private:
        Gpio *triger;
        uint8_t flag;
				uint8_t status;
    
        

};

#endif
