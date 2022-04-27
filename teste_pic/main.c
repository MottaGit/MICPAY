
#include <xc.h>
#include "header.h"

void main(void) {
    
    while(1)
    {
        switch(ESTADO)
        {
            case 0:
                if(keyboard_input() == '#')
                    maquina_on_off(1);
                break;
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
        }
    }
    
    return;
}
