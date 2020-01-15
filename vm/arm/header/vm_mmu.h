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

/*
    [VM_Model](https://developer.arm.com/docs/ddi0595/e/aarch64-system-registers/id_aa64mmfr0_el1)
 */
typedef union ID_AA64MMFR0_EL1_Register {
    uint64_t reister_value;
    struct {
        uint64_t pa_range:4;        // 3 ~ 0
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


void vm_mmu_init(void);

void vm_mmu_context_switch(vm_pt_le1_entry_t *new_proc_pte1, vm_pt_le1_entry_t **old_proc_pte1);
