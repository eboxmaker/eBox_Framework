#ifndef __RCC_H
#define __RCC_H


#ifdef __cplusplus
extern "C" {
#endif

void rcc_clock_cmd(uint32_t dev, FunctionalState state);

#ifdef __cplusplus
}
#endif

#endif
