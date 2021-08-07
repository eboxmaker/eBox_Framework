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
#include "datetime.h"
#include "Linear.h"
#include "Vector.h"
#include "eboxList.h"

stl::Vector<String> vectStr;
stl::List<String>  listStr;
void setup()
{
    ebox_init();
    uart1.begin(115200);
    listStr.init();
    listStr.push_back("LIST 123");
    listStr.push_back("LIST ABC");
    listStr.push_back("LIST ebox");
    uart1.printf("=======List test========\n");
    for(stl::List<String>::iterator it = listStr.begin(); it != listStr.end(); it++)
    {
        uart1.printf("str:%s\n",it.current->data.c_str());
    }
    for(stl::List<String>::iterator it = listStr.begin(); it != listStr.end(); it++)
    {
        if(it.current->data == "LIST ebox")
        {
            listStr.erase(it);
        }
    }
    for(stl::List<String>::iterator it = listStr.begin(); it != listStr.end(); it++)
    {
        uart1.printf("str:%s\n",it.current->data.c_str());
    }
    
    
    uart1.printf("=======vector test========\n");
    vectStr.push_back("123");
    vectStr.push_back("ABC");
    vectStr.push_back("ebox");
    for(stl::Vector<String>::iterator it = vectStr.begin(); it != vectStr.end(); it++)
    {
        uart1.printf("str:%s\n",it->c_str());
    }
    for(int i = 0; i < vectStr.size(); i++)
    {
        uart1.printf("str:%s\n",vectStr[i].c_str());
    }
    //存在风险，删除最后一个元素会出现野指针
//    for(stl::Vector<String>::iterator it = vectStr.begin(); it != vectStr.end(); it++)
//    {
//        if(*it == "ebox")
//        {
//            uart1.printf("删除:%s\n",it->c_str());
//            uart1.flush();
//            vectStr.erase(it);
//        }
//    }
    for (int i=0; i<vectStr.size(); i++)
    {
        if (vectStr[i] == "ebox")
        {
            uart1.printf("删除:%s\n",vectStr[i].c_str());
            uart1.flush();
            vectStr.erase(vectStr.begin()+i);
            i--;//重要！！！
        }
    }
    for(int i = 0; i < vectStr.size(); i++)
    {
        uart1.printf("str:%s\n",vectStr[i].c_str());
    }
    
    for (int i=0; i<vectStr.size(); i++)
    {
        uart1.printf("删除:%s\n",vectStr[i].c_str());
        uart1.flush();
        vectStr.erase(vectStr.begin()+i);
        i--;//重要！！
    }
    for(int i = 0; i < vectStr.size(); i++)
    {
        uart1.printf("str:%s\n",vectStr[i].c_str());
    }
    
}
int main(void)
{

    setup();
    while(1)
    {

        delay_ms(1000);
    }
}




