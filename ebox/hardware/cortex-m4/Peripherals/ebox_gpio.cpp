#include "ebox_gpio.h"

/**
 *@name     Gpio(GPIO_TypeDef *port,uint16_t pin)
 *@brief    Gpio构造函数
 *@param    port:   GPIOA,B,C,D,E,F,G
 *          pin:    GPIO_Pin_0...15
 *@retval   None
*/
mcuGpio::mcuGpio(GPIO_TypeDef *port, uint16_t pin)
{
    uint8_t temp1,temp2;
    this->port = port;
    this->pin = pin;
    temp1 = ((uint32_t)port - AHB1PERIPH_BASE)>>10;
    for(int i = 0; i <= 15; i ++)
    {
        if(((this->pin >> i) & 0xfffe) == 0)
        {
            temp2 = i ;
            break;
        }
    }
    this->id = (PIN_ID_t)(temp1*16 + temp2);
}
/**
 *@name     void Gpio::mode(PIN_MODE mode)
 *@brief    GPIO模式设置
 *@param    mode:   PIN_MODE枚举变量类型
 *@retval   None
*/
void mcuGpio::mode(PIN_MODE mode)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    rcc_clock_cmd((uint32_t)port,ENABLE);
    GPIO_InitStructure.GPIO_Pin = this->pin;
    //GPIO_InitStructure.GPIO_Mode = (GPIOMode_TypeDef)mode;
    switch((uint8_t)mode)
    {
        /*analog input mode
        */
        case AIN:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;

            GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        break;
        
        /* digital input mode
        */
        case INPUT:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
            GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        break;
        
        case INPUT_PD:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
            GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
        break;
            
        case INPUT_PU:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
            GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
        break;
        
        /*digital output mode
        */
        case OUTPUT_OD:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
            GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
            GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        break;
            
        case OUTPUT_OD_PU:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
            GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
            GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
        break;
            
        case OUTPUT_OD_PD:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
            GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
            GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
        break;
            
        case OUTPUT:
        case OUTPUT_PP:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
            GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
            GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        break;
        
        case OUTPUT_PP_PU:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
            GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
            GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
        break;
        
        case OUTPUT_PP_PD:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
            GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
            GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
        break;
        
        
        /*af mode
        */
        case AF_OD:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
            GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
            GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        break;
            
        case AF_OD_PU:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
            GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
            GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
        break;
            
        case AF_OD_PD:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
            GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
            GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
        break;
            
        case AF_PP:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
            GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
            GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
       break;
        
        case AF_PP_PU:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
            GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
            GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
       break;
        
        case AF_PP_PD:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
            GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
            GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
       break;
        /* if parament is other mode,set as INPUT mode
        */
       default:
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
            GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
            break;
    }
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(this->port, &GPIO_InitStructure);   //初始化GPIOC端口
}
void mcuGpio::mode(PIN_MODE mode,uint8_t af_configration)
{
    mcuGpio::mode(mode);
    af_config(af_configration);
}

void mcuGpio::af_config(uint8_t af_configration)
{  
  	GPIO_PinAFConfig(port, (id % 16), af_configration);  
}

/**
 *@name     void Gpio::set()
 *@brief    GPIO输出高电平
 *@param    NONE
 *@retval   NONE
*/
void mcuGpio::set()
{
    this->port->BSRRL = this->pin;
}

/**
 *@name     void Gpio::reset()
 *@brief    GPIO输出低电平
 *@param    NONE
 *@retval   NONE
*/
void mcuGpio::reset()
{
    this->port->BSRRH = this->pin;
}


/**
 *@name     void Gpio::write(uint8_t val)
 *@brief    GPIO输出设置
 *@param    val：1：输出高电平；0：输出低电平
 *@retval   NONE
*/
void mcuGpio::write(uint8_t val)
{
    if(val == 0)
        this->port->BSRRH = this->pin;
    else
        this->port->BSRRL = this->pin;
}


/**
 *@name     void Gpio::set()
 *@brief    GPIO输出高电平
 *@param    NONE
 *@retval   NONE
*/
void mcuGpio::read(uint8_t *val)
{
    *val = this->port->IDR & this->pin;
}


/**
 *@name     uint8_t Gpio::read(void)
 *@brief    读取GPIO引脚状态
 *@param    NONE
 *@retval   返回引脚当前的电平状态
*/
uint8_t mcuGpio::read(void)
{
    if(this->port->IDR & this->pin)
        return 1;
    return  0;
}

/**
 *@name     void Gpio::toggle()
 *@brief    GPIO输出电平翻转
 *@param    NONE
 *@retval   NONE
*/
void mcuGpio::toggle()
{
    port->ODR ^= this->pin;
}
/**
 *@name     uint8_t shift_in(Gpio *data_pin, Gpio *clock_pin, uint8_t bit_order)
 *@brief    Gpio按照时钟连续输出一个u8类型的数据，用于串行通信。
 *@param    data_pin：  数据输入引脚
            clock_pin:  时钟信号的引脚
            bit_order:  数据大小端控制LSB_FIRST：低位先发送；MSB_FIRST高位先发送
 *@retval   接收到的数据
*/
int mcuGpio::operator =(mcuGpio&) 
{
    return read();
}

mcuGpio::operator int()
{
    return read();
}

mcuGpio mcuGpio::operator= ( int value)
{
    write(value);
    return *this;
}

