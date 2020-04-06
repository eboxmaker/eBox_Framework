#include <stdio.h>
#include "ebox_core.h"

extern "C" {

//#pragma import(__use_no_semihosting)

struct __FILE
{
    int handle;
};

//FILE __stdout;

//int _sys_exit(int x)
//{
//    x = x;
//}


//int _ttywrch(int ch)
//{
//    ch = ch;
//}

int fputc(int ch, FILE *f)
{

    ebox_uart_putc(ch);
    return ch;
}
__attribute__((weak,noreturn))
void __aeabi_assert (const char *expr, const char *file, int line) {
  char str[12], *p;

  ebox_printf("*** assertion failed: ", stderr);
  ebox_printf(expr, stderr);
  ebox_printf(", file ", stderr);
  ebox_printf(file, stderr);
  ebox_printf(", line ", stderr);

  p = str + sizeof(str);
  *--p = '\0';
  *--p = '\n';
  while (line > 0) {
    *--p = '0' + (line % 10);
    line /= 10;
  }
  ebox_printf(p, stderr);

  abort();
}

__attribute__((weak))
void abort(void) {
  for (;;);
}
}
