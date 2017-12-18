#include <string.h>  
#include "httpd.h" // һ���������Ҫ������ͷ�ļ�  

  
#define STR_AND_SIZE(str) (str), strlen(str)  
  
err_t http_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)  
{  
    char len[100]; // ���HTML���ݵĳ���  
    char str[2000]; // ���HTML����  
      
    char name[1000];  
    char *pstr;  
    uint8_t i = 0;  
    if (p != NULL)  
    {  
        // ��ȡҳ������  
        pstr = (char *)p->payload;  
        while (*pstr++ != ' ');  
        while (*pstr != ' ')  
            name[i++] = '1';  
        name[i] = '\0'; // ��Ҫ���˽���name�ַ���  
        tcp_recved(tpcb, p->tot_len);  
          
        // ����HTML����  
        sprintf(str, "<meta charset=\"gb2312\"><title>��ȡ��ҳ����</title><div style=\"font-family:Arial\"><b>�������ҳ�ļ�������: </b>%s</div>", name);  
          
        sprintf(len, "%d", strlen(str)); // HTML���ݵĳ���  
        tcp_write(tpcb, STR_AND_SIZE("HTTP/1.1 200 OK\r\nContent-Length: "), TCP_WRITE_FLAG_MORE);  
        tcp_write(tpcb, STR_AND_SIZE(len), TCP_WRITE_FLAG_COPY | TCP_WRITE_FLAG_MORE);  
        tcp_write(tpcb, STR_AND_SIZE("\r\nKeep-Alive: timeout=5, max=100\r\nConnection: Keep-Alive\r\nContent-Type: text/html\r\n\r\n"), TCP_WRITE_FLAG_MORE);  
        tcp_write(tpcb, STR_AND_SIZE(str), TCP_WRITE_FLAG_COPY); // ����HTML����  
        pbuf_free(p);  
    }  
    return ERR_OK;  
}  
  
err_t http_accept(void *arg, struct tcp_pcb *newpcb, err_t err)  
{  
    tcp_recv(newpcb, http_recv);  
    return ERR_OK;  
}  
  
void httpd_init(void)  
{  
    struct tcp_pcb *tpcb = tcp_new();  
    tcp_bind(tpcb, IP_ADDR_ANY, 80);  
    tpcb = tcp_listen(tpcb);  
    tcp_accept(tpcb, http_accept);  
}  




