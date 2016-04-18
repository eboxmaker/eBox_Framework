#ifndef __CSB_H
#define __CSB_H
#include "ebox.h"

class ULTRA:public IN_CAPTURE
{
    public:
        float wave_time_us;
    public:
        ULTRA(GPIO *echo,GPIO *triger):IN_CAPTURE(echo)
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
        GPIO *triger;
        uint8_t flag;
				uint8_t status;
    
        

};

#endif