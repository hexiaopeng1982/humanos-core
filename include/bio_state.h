#ifndef BIO_STATE_H
#define BIO_STATE_H

#include <stdint.h>

/*
 * 生物状态向量（精简版）
 * 这就是我们要维护的“内存映射寄存器”
 */
typedef struct {
    uint32_t timestamp;   // 系统时间（秒）

    /* 生命体征 */
    float heart_rate;     // BPM
    float body_temp;      // Celsius

    /* 细胞层面（占位符） */
    float telomere_len;   // 端粒长度 (0.0 - 1.0 归一化)
    float senescence;     // 衰老细胞比例 (0.0 - 1.0)

    /* 代谢层面 */
    float inflammation;  // 炎症水平 (0.0 - 1.0)
    float energy;        // 能量储备 (0.0 - 1.0)

} bio_state_t;

#endif
