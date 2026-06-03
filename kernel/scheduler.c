#include <stdio.h>
#include <unistd.h>
#include "../include/bio_state.h"
#include "../include/driver.h"

/* ========= 全局人体状态 ========= */
static bio_state_t g_human = {0};

/* ========= 驱动表（手动注册，不依赖 linker magic） =========
 * 新驱动想加入，只需在 drivers/某目录里定义自己的 _drv_xxx 结构体，
 * 然后把它的指针加到这个表里即可。
 */
extern driver_ops_t _drv_anti_inflammation;   /* 来自 drivers/example/anti_inflammation.c */
/* extern driver_ops_t _drv_xxx;  ← 以后新驱动在这里声明 extern */

static driver_ops_t* g_drv_table[] = {
    &_drv_anti_inflammation,
    /* &_drv_xxx, */
    NULL
};

/* ========= 模拟环境：自然漂移（熵增） ========= */
static void sim_tick(bio_state_t* s)
{
    s->timestamp++;
    s->inflammation += 0.01f;
    s->energy       -= 0.005f;
    if (s->inflammation > 1.0f) s->inflammation = 1.0f;
    if (s->energy       < 0.0f) s->energy       = 0.0f;
}

/* ========= 内核主循环 ========= */
int main(void)
{
    printf("[HumanOS] Core v0.1 booting...\n");

    g_human.inflammation = 0.10f;
    g_human.energy       = 1.00f;
    g_human.heart_rate   = 72.0f;
    g_human.body_temp    = 36.7f;

    /* init 阶段 */
    for (driver_ops_t** pp = g_drv_table; *pp; ++pp)
        (*pp)->init();

    /* super-loop */
    while (1) {
        sim_tick(&g_human);

        printf("[State] t=%u  inflam=%.3f  energy=%.3f  HR=%.0f\n",
               g_human.timestamp,
               g_human.inflammation,
               g_human.energy,
               g_human.heart_rate);

        for (driver_ops_t** pp = g_drv_table; *pp; ++pp) {
            driver_ops_t* d = *pp;
            if (d->diagnose(&g_human) == 1) {
                printf("  -> [%s] triggered, repairing...\n", d->name);
                d->repair(&g_human);
            }
        }

        sleep(1);
    }
    return 0;
}

