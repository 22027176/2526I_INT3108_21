#include "init.h"

int main() {
    init_LEDs();
    init_SWs();
    init_SysTick();

    while (1) {
        system_logic();
    }
}
