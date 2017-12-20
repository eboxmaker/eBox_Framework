
/* 本文件用于存储网页，html文件
*由于本文件是c语言文件，
*html是文本文件，使用html语言时，
*注意对符号 "  //  加上 \  ，以表示其ascii码。
*/

/*http文件头 */
#define HTTP_HEAD "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n"


/*网页，以数组形式存储。登录页面*/
#define LOGIN "\
<html>\
<head>\
<meta http-equiv=\"Content-Type\" content=\"text/html; charset=gb2312\">\
<title>LWIP移植实验 </title>\
</head>\
</body>\
<br><center><img src=\"http:/\/my.csdn.net/uploads/201208/05/1344174874_1299.jpg\">\
<br><center><h2>野火STM32开发平台</h2>\
<br><center><b>***********野火STM32开发板STM32F103-ENC28J60-移植LWIP，web服务器测试***********</b>\
<form action=\"login\" method=\"post\">\
<p>  用户名:\
<input type=\"text\" name=\"UserName\">\
<p>	 密码:  \
<input type=\"password\" name=\"PassWord\">\
<p><input type=\"submit\" value=\"登录\"> \
</form>\
<br><br><center><a href=\"http:/\/firestm32.taobao.com/\">野火官方旗舰店</a>\
      <a href=\"http:/\/www.amobbs.com/forum-1008-1.html\">amoBBS论坛野火专区</a>\
<br><br><center><img src=\"http:/\/my.csdn.net/uploads/201208/05/1344174982_8577.jpg\">\
</body> \
</html>\
" 
//form_action.asp
/*网页，以数组形式存储。LED控制页面*/
#define LED_CTRL_ON "\
<html>\
<head>\
<meta http-equiv=\"Content-Type\" content=\"text/html; charset=gb2312\">\
<title>LWIP移植实验 </title>\
</head>\
</body>\
<br><center><img src=\"http:/\/my.csdn.net/uploads/201208/05/1344174874_1299.jpg\">\
<br><center><h2>野火STM32开发平台</h2>\
<br><center><b>***********野火STM32开发板STM32F103-ENC28J60-移植LWIP，web服务器测试***********</b>\
<br><br><br><center><b>***********！！！登录成功！！！***********</b>\
<br><br><br>LED状态:  <input type=\"button\" id=\"b1\" style=\"background:#0000FF\" value=\"亮\" > \
<form name=\"input\" action=\"/html/html_form_action.asp\" method=\"post\">\
<br>点亮LED1<input type=\"radio\" name=\"LED_CTRL\" value=\"ON\" checked=\"checked\">\
<br/>\
<br>关闭LED1<input type=\"radio\" name=\"LED_CTRL\" value=\"OFF\">\
<br><br><center><input type =\"submit\" value =\"控制\">\
<br><br><br><center><a href=\"http:/\/firestm32.taobao.com/\">野火官方旗舰店</a>\
      <a href=\"http:/\/www.amobbs.com/forum-1008-1.html\">amoBBS论坛野火专区</a>\
</body> \
</html>\
"

#define LED_CTRL_OFF "\
<html>\
<head>\
<meta http-equiv=\"Content-Type\" content=\"text/html; charset=gb2312\">\
<title>LWIP移植实验 </title>\
</head>\
</body>\
<br><center><img src=\"http:/\/my.csdn.net/uploads/201208/05/1344174874_1299.jpg\">\
<br><center><h2>野火STM32开发平台</h2>\
<br><center><b>***********野火STM32开发板STM32F103-ENC28J60-移植LWIP，web服务器测试***********</b>\
<br><br><br><center><b>***********！！！登录成功！！！***********</b>\
<br><br><br>LED状态:  <input type=\"button\" id=\"b1\" style=\"background:#DCDCDC\" value=\"灭\" > \
<form name=\"input\" action=\"/html/html_form_action.asp\" method=\"post\">\
<br>点亮LED1<input type=\"radio\" name=\"LED_CTRL\" value=\"ON\" checked=\"checked\">\
<br/>\
<br>关闭LED1<input type=\"radio\" name=\"LED_CTRL\" value=\"OFF\" checked=\"checked\">\
<br><br><center><input type =\"submit\" value =\"控制\">\
<br><br><br><center><a href=\"http:/\/firestm32.taobao.com/\">野火官方旗舰店</a>\
      <a href=\"http:/\/www.amobbs.com/forum-1008-1.html\">amoBBS论坛野火专区</a>\
</body> \
</html>\
"

