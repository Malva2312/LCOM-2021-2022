#include <lcom/lcf.h>
#include <stdint.h>
#include <lcom/utils.h>
#ifdef LAB3
int cnt = 0;
#endif

int (util_sys_inb)(int port, uint8_t *val){
    uint32_t stub;
    
    bool not_valid = sys_inb(port, &stub);
    #ifdef LAB3
    cnt++;
    #endif
    if(not_valid) return 1;
    *val = (uint8_t) stub;
    return 0;
}
