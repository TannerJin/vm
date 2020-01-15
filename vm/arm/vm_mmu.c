//
//  vm_mmu.c
//  vm
//
//  Created by jintao on 2020/1/14.
//  Copyright © 2020 jintao. All rights reserved.
//

#include "vm_mmu.h"

void vm_mmu_disableStage2(void);

void vm_mmu_init() {
    
    // set kernel translation table addr
    __asm__ volatile(
        "msr TTBR1_EL1, %0;"
        "ibs;"
        :
        :"r"(kernel_ptb_addr)
    );
    
    // *
    // *
    // *
    
}

// don't use stage2 for now
void vm_mmu_disableStage2() {
    uint64_t SCTLR_EL2_Value = 0;
    __asm__ volatile(
        "mrs %0, SCTLR_EL2;"
        "orr %0, %0 , #0;"          // set [EE] bit
        "msr SCTLR_EL2, %0;"
        :"=r"(SCTLR_EL2_Value)
        :"r"(SCTLR_EL2_Value)
    );
}


void vm_mmu_memoryModel_get() {
    ID_AA64MMFR0_EL1_Register_t register_t;
    register_t.reister_value = 0;
    
    __asm__ volatile(
        "mrs %0, ID_AA64MMFR0_EL1;"
        :"=r"(register_t)
        :
    );
}


void vm_mmu_context_switch(vm_pt_le1_entry_t *new_proc_pte1, vm_pt_le1_entry_t **old_proc_pte1) {
    vm_pt_le1_entry_t* old_pte1;
    __asm__ volatile(
        "mrs %0, TTBR0_EL1;"    // save old TTBR
        "msr TTBR0_EL1, %1;"    // set new TTBR
                     
        :"=r"(old_pte1)
        :"r"(new_proc_pte1)
    );
    *old_proc_pte1 = old_pte1;
}
