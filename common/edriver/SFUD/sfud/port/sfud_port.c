/*
 * This file is part of the Serial Flash Universal Driver Library.
 *
 * Copyright (c) 2016, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (theSoftware'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: Portable interface for each platform.
 * Created on: 2016-04-23
 */

#include <sfud.h>
#include <stdarg.h>

//char log_buf[256];

//void sfud_log_debug(const char *file, const long line, const char *format, ...);


//sfud_err sfud_spi_port_init(sfud_flash *flash) {
//    sfud_err result = SFUD_SUCCESS;
//    
//    _sfud_spi_port_init(&flash);

//    return result;
//}



///**
// * This function is print debug info.
// *
// * @param file the file which has call this function
// * @param line the line number which has call this function
// * @param format output format
// * @param ... args
// */
//void sfud_log_debug(const char *file, const long line, const char *format, ...) {
//    va_list args;

//    /* args point to the first variable parameter */
//    va_start(args, format);
//    ebox_printf("[SFUD](%s:%ld) ", file, line);
//    /* must use vprintf to print */
//    vsnprintf(log_buf, sizeof(log_buf), format, args);
//    ebox_printf("%s\n", log_buf);
//    va_end(args);
//}

///**
// * This function is print routine info.
// *
// * @param format output format
// * @param ... args
// */
//void sfud_log_info(const char *format, ...) {
//    va_list args;

//    /* args point to the first variable parameter */
//    va_start(args, format);
//    ebox_printf("[SFUD]");
//    /* must use vprintf to print */
//    vsnprintf(log_buf, sizeof(log_buf), format, args);
//    ebox_printf("%s\n", log_buf);
//    va_end(args);
//}
