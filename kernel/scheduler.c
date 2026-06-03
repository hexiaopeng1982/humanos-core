#include <stdio.h>
#include <unistd.h> // for sleep
#include "../include/bio_state.h"
#include "../include/driver.h"

/* 引入驱动段（Linker Magic） */
extern driver_ops_t __start_drivers;
extern driver_ops_t __stop_drivers;

/* 全局状态 */
static bio_state_t g_human_state;

void update_simulation(bio_state_t* state) {
    // 模拟自然衰老：炎症增加，能量减少
    state->inflammation += 0.01f;
    state->energy -= 0.005f;
    if (state->inflammation > 1.0f) state->inflammation = 1.0f;
    if (state->energy < 0.0f) state->energy = 0.0f;
}

int main(void) {
    printf("[Kernel] HumanOS Core v0.1 Booting...\n");

    /* 初始化状态 */
    g_human_state.inflammation = 0.1f;
    g_human_state.energy = 1.0f;

    /* 遍历所有注册的驱动 */
    driver_ops_t** drv_ptr;
    for (drv_ptr = &__start_drivers; drv_ptr < &__stop_drivers; drv_ptr++) {
        driver_ops_t* drv = *drv_ptr;
        printf("[Kernel] Loading driver: %s\n", drv->name);
        drv->init();
    }

    /* 主循环 */
    while (1) {
        update_simulation(&g_human_state);
        
        printf("[State] Inflam: %.2f | Energy: %.2f\n", 
               g_human_state.inflammation, g_human_state.energy);

        // 调度驱动
        for (drv_ptr = &__start_drivers; drv_ptr < &__stop_drivers; drv_ptr++) {
            driver_ops_t* drv = *drv_ptr;
            if (drv->diagnose(&g_human_state) == 1) {
                printf("[Kernel] Driver %s triggered!\n", drv->name);
                drv->repair(&g_human_state);
            }
        }

        sleep(1); // 1Hz 心跳
    }
    return 0;
}
