#ifndef DRIVER_H
#define DRIVER_H

#include "bio_state.h"

/* 驱动操作集 */
typedef struct {
    const char* name;
    
    /* 初始化 */
    int (*init)(void);
    
    /* 诊断：读取状态，返回是否需要修复（0=正常，1=警告，-1=错误） */
    int (*diagnose)(const bio_state_t* state);
    
    /* 修复：执行动作 */
    int (*repair)(bio_state_t* state);
    
    /* 卸载 */
    void (*deinit)(void);
} driver_ops_t;

/* 驱动注册宏（极客最爱） */
#define REGISTER_DRIVER(drv) \
    __attribute__((used, section(".drivers"))) static driver_ops_t* _##drv##_ptr = &drv

#endif
