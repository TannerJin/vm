//
//  vm_mmu.h
//  vm
//
//  Created by jintao on 2020/1/14.
//  Copyright © 2020 jintao. All rights reserved.
//

/*
   Documents:
   [ARMv8-A Address Translation](https://static.docs.arm.com/100940/0100/armv8_a_address%20translation_100940_0100_en.pdf)
   [SMMU Architecture](https://static.docs.arm.com/ihi0062/dc/IHI0062D_c_system_mmu_architecture_specification.pdf)
*/

#include "uint64_t.h"
#include "vm_kernel.h"

/*  ID_AA64MMFR0_ELI
    [VM_Model](https://developer.arm.com/docs/ddi0595/e/aarch64-system-registers/id_aa64mmfr0_el1)
 */
typedef union ID_AA64MMFR0_EL1_Register {
    uint64_t reister_value;
    struct {
        uint64_t pa_range:4;        // 3 ~ 0   CPU支持的物理地址大小; 0101 -> 256TB -> 48 bits
        uint64_t asid_bits:4;
        uint64_t big_end:4;
        uint64_t sns_men:4;         // support a distinction between Secure and Non-secure Memory
        uint64_t big_end_el0:4;
        uint64_t t_gran16:4;
        uint64_t t_gran64:4;
        uint64_t t_gran4:4;
        uint64_t t_gran16_2:4;
        uint64_t t_gran64_2:4;
        uint64_t t_gran4_2:4;
        uint64_t exs:4;
        uint64_t res0:8;
        uint64_t fgt:4;
        uint64_t ecv:4;             // 63 ~ 60
    };
} ID_AA64MMFR0_EL1_Register_t;


/*  TCR_ELI
    https://developer.arm.com/docs/ddi0595/f/aarch64-system-registers/tcr_el1
 */
typedef union TCR_EL1_Register {
    uint64_t value;
    struct {
        uint64_t t0sz:6;        // 用户区域大小  2^(64-T0SZ)
        uint64_t res0:1;
        uint64_t epd0:1;
        uint64_t irgn0:2;
        uint64_t orgn0:2;
        uint64_t sh0:2;
        uint64_t tg0:2;         // user granule size
        
        uint64_t t1sz:6;        // 内核区域大小  2^(64-T1SZ)
        uint64_t a1:1;
        uint64_t epd1:1;
        uint64_t irgn1:2;
        uint64_t orgn1:2;
        uint64_t sh1:2;
        uint64_t tg1:2;         // kernel granule size
        
        uint64_t ips:3;         // 地址翻译后能输出的物理地址大小  (<=ID_AA64MMFR0_EL1_Register.pa_range);  101 -> 256TB -> 48 bits
    };
} TCR_EL1_Register_t;


/*  TTBR
    
 */
typedef union TTBR_Register {
    uint64_t value;
    struct {
        uint64_t res0:1;
        uint64_t address:47;
        uint64_t asid:16;
    };
} TTBR_Register_t;


void vm_mmu_init(void);

void vm_mmu_context_switch(vm_pte_le1_t *new_proc_pte1, vm_pte_le1_t **old_proc_pte1);
