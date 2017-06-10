
#include "ebox.h"
#include "os.h"


// TIME STAMP support
CPU_TS_TMR CPU_TS_TmrRd(void){
    return millis();
}

void CPU_TS_TmrInit(void){
}
