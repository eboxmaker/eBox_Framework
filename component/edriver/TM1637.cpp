//  Author:Fred.Chu
//  Date:9 April,2013
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
//  Modified record:
//
/*******************************************************************************/
#include "TM1637.h"
static int8_t TubeTab[] = {0x3f, 0x06, 0x5b, 0x4f,
                           0x66, 0x6d, 0x7d, 0x07,
                           0x7f, 0x6f, 0x77, 0x7c,
                           0x39, 0x5e, 0x79, 0x71
                          };//0~9,A,b,C,d,E,F
TM1637::TM1637(Gpio *clk, Gpio *data)
{
    this->clk = clk;
    this->data = data;

}
void TM1637::begin()
{
    clk->mode(OUTPUT_PP);
    data->mode(OUTPUT_PP);
    clearDisplay();

}


void TM1637::writeByte(int8_t wr_data)
{
    uint8_t i, count1;
    for(i = 0; i < 8; i++)  //sent 8bit data
    {
        clk->reset();
        if(wr_data & 0x01)data->set();//(Datapin,HIGH);//LSB first
        else data->reset();
        wr_data >>= 1;
        clk->set();

    }
    clk->reset();//(Clkpin,LOW); //wait for the ACK
    data->set();//(Datapin,HIGH);
    clk->set();//(Clkpin,HIGH);
    data->mode(INPUT);//(Datapin,INPUT);
    while(data->read())
    {
        count1 += 1;
        if(count1 == 200)//
        {
            data->mode(OUTPUT_PP);
            data->reset();
            count1 = 0;
        }
        data->mode(INPUT);//(Datapin,INPUT);
    }
    data->mode(OUTPUT_PP);

}
//send start signal to TM1637
void TM1637::start(void)
{
    clk->set();//(Clkpin,HIGH);//send start signal to TM1637
    data->set();//(Datapin,HIGH);
    data->reset();//(Datapin,LOW);
    clk->reset();//(Clkpin,LOW);
}
//End of transmission
void TM1637::stop(void)
{
    clk->reset();//(Clkpin,LOW);
    data->reset();//(Datapin,LOW);
    clk->set();//(Clkpin,HIGH);
    data->set();//(Datapin,HIGH);
}
//display function.Write to full-screen.
void TM1637::display(int8_t DispData[])
{
    int8_t SegData[4];
    uint8_t i;
    for(i = 0; i < 4; i ++)
    {
        SegData[i] = DispData[i];
    }
    coding(SegData);
    start();          //start signal sent to TM1637 from MCU
    writeByte(ADDR_AUTO);//
    stop();           //
    start();          //
    writeByte(Cmd_SetAddr);//
    for(i = 0; i < 4; i ++)
    {
        writeByte(SegData[i]);        //
    }
    stop();           //
    start();          //
    writeByte(Cmd_DispCtrl);//
    stop();           //
}
//******************************************
void TM1637::display(uint8_t BitAddr, int8_t DispData)
{
    int8_t SegData;
    SegData = coding(DispData);
    start();          //start signal sent to TM1637 from MCU
    writeByte(ADDR_FIXED);//
    stop();           //
    start();          //
    writeByte(BitAddr | 0xc0); //
    writeByte(SegData);//
    stop();            //
    start();          //
    writeByte(Cmd_DispCtrl);//
    stop();           //
}

void TM1637::clearDisplay(void)
{
    display(0x00, 0x7f);
    display(0x01, 0x7f);
    display(0x02, 0x7f);
    display(0x03, 0x7f);
}
//To take effect the next time it displays.
void TM1637::set(uint8_t brightness, uint8_t SetData, uint8_t SetAddr)
{
    Cmd_SetData = SetData;
    Cmd_SetAddr = SetAddr;
    Cmd_DispCtrl = 0x88 + brightness;//Set the brightness and it takes effect the next time it displays.
}

//Whether to light the clock point ":".
//To take effect the next time it displays.
void TM1637::point(bool PointFlag)
{
    _PointFlag = PointFlag;
}
void TM1637::coding(int8_t DispData[])
{
    uint8_t PointData;
    if(_PointFlag == POINT_ON)PointData = 0x80;
    else PointData = 0;
    for(uint8_t i = 0; i < 4; i ++)
    {
        if(DispData[i] == 0x7f)DispData[i] = 0x00;
        else DispData[i] = TubeTab[DispData[i]] + PointData;
    }
}
int8_t TM1637::coding(int8_t DispData)
{
    uint8_t PointData;
    if(_PointFlag == POINT_ON)PointData = 0x80;
    else PointData = 0;
    if(DispData == 0x7f) DispData = 0x00 + PointData;//The bit digital tube off
    else DispData = TubeTab[DispData] + PointData;
    return DispData;
}
