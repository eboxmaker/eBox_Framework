/******************** (C) COPYRIGHT 2012 WildFire Team ***************************
 * 文件名  ：httpd.c
 * 描述    ：网页服务器相关函数         
 * 实验平台：野火STM32开发板
 
 * 库版本  ：ST3.5.0
 * 作者    ：wildfire team 
 * 论坛    ：www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
 * 淘宝    ：http://firestm32.taobao.com
**********************************************************************************/
#include "httpd.h"



const static uint8_t http_html_hdr[] =  HTTP_HEAD;   	//http文件头
const static uint8_t login[]= LOGIN;	 		//要显示的网页
const static uint8_t led_ctrl_on[] =LED_CTRL_ON;
const static uint8_t led_ctrl_off[] =LED_CTRL_OFF;



/*
 * 函数名：http_recv
 * 描述  ：http接收到数据后的回调函数
 * 输入  ：tcp_arg设置的参数、tcp_pcb、pcb、err
 * 输出  ：err
 * 调用  ：内部调用 
 */

static err_t http_recv(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
	{  
  	char * data = NULL;
	char *UserName =NULL;
	char *PassWord =NULL;
	char *LED_CMD  =NULL;
	char *ch =NULL;

	data = p->payload;	//把接收到的数据指针交给data

	 if (err == ERR_OK && p != NULL)	//判断是否非空
	 {	    	 	 			
		if(strncmp(data,"GET",3)==0) 
			{	
						
				/*第一次与服务器连接，打开登录页面，未输入登录信息*/
		  			tcp_write(pcb,http_html_hdr,sizeof(http_html_hdr),0);      /* 发送http协议头部信息 */
	  				tcp_write(pcb,login,sizeof(login),0);              			/* 发送登录网页信息 */
			}
				  	   		
		else if(strncmp(data,"POST",4)==0)		
		  {
				LED_CMD	= strstr(data,"LED_CTRL=");					
				UserName = strstr(data,"UserName="); 		//截取UserName=
				PassWord = strstr(UserName,"PassWord=");	//截取PassWord=						

					if(UserName!=NULL && PassWord!=NULL)	 /*输入了用户名和密码*/
					{	
						ch = strchr(UserName,'&'); 				//把&替换为‘\0’
						*ch = '\0';
		
						UserName +=sizeof("UserName=")-1; 		//提取用户名。sizeof字符串包括终止符‘\0’ 
						PassWord += sizeof("PassWord=")-1;		//提取密码
						
						if(strcmp(UserName,"wildfire")==0 && strncmp(PassWord,"123456",6)==0)	/*输入的用户名和密码正确*/
						{	
							//LED1(ON);
						//	printf("\r\n提取出的用户名 =%s\r\n提取出的密码 =%s",UserName,PassWord);				 
						 	tcp_write(pcb,http_html_hdr,sizeof(http_html_hdr),0);      /* 发送http协议头部信息 */
							tcp_write(pcb,led_ctrl_on,sizeof(led_ctrl_on),0);          /* 发送led控制网页信息 */
								
						 }
						 else /*输入的用户名和密码错误*/
						 {
						   	tcp_write(pcb,http_html_hdr,sizeof(http_html_hdr),0);      /* 发送http协议头部信息 */
	  						tcp_write(pcb,login,sizeof(login),0);              			/* 发送登录网页信息 */
						 }
					}
					else if(LED_CMD !=NULL)	  /*接收到LED控制命令*/
					{
						if(strstr(LED_CMD,"LED_CTRL=ON"))	 /*检测是哪个命令：开\关*/
						{
							//LED1(ON);
							tcp_write(pcb,http_html_hdr,sizeof(http_html_hdr),0);      /* 发送http协议头部信息 */
							tcp_write(pcb,led_ctrl_on,sizeof(led_ctrl_on),0);         /* 发送led控制网页信息 */
						}
						else if(strstr(LED_CMD,"LED_CTRL=OFF"))
						{
							//LED1(OFF);
						   	tcp_write(pcb,http_html_hdr,sizeof(http_html_hdr),0);      /* 发送http协议头部信息 */
							tcp_write(pcb,led_ctrl_off,sizeof(led_ctrl_off),0);        /* 发送led控制网页信息 */
						} 								
					}			
		   	 }
		
    		pbuf_free(p);  /* 释放该pbuf段 */
	 }	  
  tcp_close(pcb);                                              			/* 关闭这个连接 */
  err = ERR_OK;
  
  return err;
}



/*
 * 函数名：http_accept
 * 描述  ：http web server的回调函数，建立连接后调用
 * 输入  ：tcp_arg设置的参数、pcb、err
 * 输出  ：err
 * 调用  ：内部调用 
 */
static err_t http_accept(void *arg,struct tcp_pcb *pcb,err_t err)
{
  tcp_setprio(pcb, TCP_PRIO_MIN);     /* 设置回调函数优先级，当存在几个连接时特别重要，此函数必须调用*/
								
  tcp_recv(pcb,http_recv);            /* 设置TCP段到时的回调函数 */
     
  return ERR_OK;
}


/*
 * 函数名：httpd_init
 * 描述  ：初始化web server，初始化后才能显示网页
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用 
 */
void httpd_init(void)
{
  struct tcp_pcb *pcb;
  
  pcb = tcp_new();                           /* 建立通信的TCP控制块(pcb) */
  
  tcp_bind(pcb,IP_ADDR_ANY,80);             /* 绑定本地IP地址和端口号 */
 
  pcb = tcp_listen(pcb);                    /* 进入监听状态 */
  
  tcp_accept(pcb,http_accept);             /* 设置有连接请求时的回调函数 */

} 

