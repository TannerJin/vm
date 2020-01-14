//
//  vm_mmu.c
//  vm
//
//  Created by jintao on 2020/1/14.
//  Copyright © 2020 jintao. All rights reserved.
//

#include "vm_mmu.h"
#include "vm_kernel.h"

void vm_mmu_level_init(void);

void vm_mmu_init() {
    
    // set kernel translation table addr
    __asm__ volatile(
        "msr TTBR1_EL1, %0;"
        "ibs;"
        :
        :"r"(kernel_ptb_paddr)
    );
    
    // *
    // *
    // *
    
    vm_mmu_level_init();
    return;
}


void vm_mmu_level_init() {
    uint64_t level = 0;
    
    __asm__ volatile(
        // level1
        "mrs %0, SCTLR_EL1;"
        "orr %0, %0, #1;"
        "msr SCTLR_EL1, %0;"
        // level2
        "mrs %0, SCTLR_EL2;"
        "orr %0, %0, #1;"
        "msr SCTLR_EL2, %0;"
        // level3
        "mrs %0, SCTLR_EL3;"
        "orr %0, %0, #1;"
        "msr SCTLR_EL3, %0;"
        :
        :"r"(level)
    );
}
