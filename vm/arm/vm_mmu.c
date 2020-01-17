//
//  vm_mmu.c
//  vm
//
//  Created by jintao on 2020/1/14.
//  Copyright © 2020 jintao. All rights reserved.
//

#include "vm_mmu.h"

void vm_mmu_enable(void);
void vm_mmu_configure(void);
void vm_mmu_get_memory_model(ID_AA64MMFR0_EL1_Register_t *register_t);

/*
    Non-secure:
        kernel: TTBR1_EL1, TCR_EL1
        user:   TTBR0_EL1, TCR_EL1
 */
void vm_mmu_init() {
    
    // set kernel translation table addr
    __asm__ volatile(
        "msr TTBR1_EL1, %0;"
        "ibs;"
        :
        :"r"(kernel_ptb_pm_addr)
    );
    
    // *
    // *
    // *
}

/*
    [tanslate states](https://armv8-ref.codingbelief.com/zh/chapter_d4/d42_2_controlling_address_translation_stages.html)
    
 */
void vm_mmu_configure() {
    // stage2 disable
    uint64_t HCR_EL2 = 0;
    __asm__ volatile(// HCR_EL2.VM = 0
                     "mrs %0, HCR_EL2;"
                     "orr %0, %0, #0;"              // set bit[0]
                     "msr HCR_EL2, %0;"
                     
                     :"=r"(HCR_EL2)
                     :
                     );
    
    // TCR_EL1
    TCR_EL1_Register_t TCR_EL1;
    TCR_EL1.value = 0;
    __asm__ volatile("mrs %0, TCR_EL1;"
                     
                     :"=r"(TCR_EL1)
                     :
                     );
    TCR_EL1.t1sz = 48;    // 2^(64-48), 内核区域大小
    TCR_EL1.t0sz = 16;    // 2^(64-16), 用户区域大小
    
    TCR_EL1.tg1 = 1;      // 01表示16KB, 内核(要不设计为4KB??)
    TCR_EL1.tg0 = 1;      // 01表示16KB，用户(设计为16KB)
    
    ID_AA64MMFR0_EL1_Register_t memory_model_register;
    vm_mmu_get_memory_model(&memory_model_register);
    
    TCR_EL1.ips = (memory_model_register.pa_range & 7);   // 设置地址翻译输出的物理地址大小 = 支持的物理地址大小
    __asm__ volatile("msr TCR_EL1, %0;"
                     
                     :
                     :"r"(TCR_EL1)
                    );
    
    // MMU enable
    vm_mmu_enable();
}


void vm_mmu_get_memory_model(ID_AA64MMFR0_EL1_Register_t *register_t) {
    ID_AA64MMFR0_EL1_Register_t memory_model;
    memory_model.reister_value = 0;
    
    __asm__ volatile("mrs %0, ID_AA64MMFR0_EL1;"
                     
                     :"=r"(memory_model)
                     :
                     );
    *register_t = memory_model;
}

__attribute__((always_inline))
void vm_mmu_disable() {
    uint64_t SCTLR_EL1_Value = 0;
    __asm__ volatile("mrs %0, SCTLR_EL1;"
                     "orr %0, %0 , #0;"          // set bit[0] bit
                     "msr SCTLR_EL2, %0;"
                     :"=r"(SCTLR_EL1_Value)
                     :
                     );
}

__attribute__((always_inline))
void vm_mmu_enable() {
    uint64_t SCTLR_EL1_Value = 0;
    uint64_t SCTLR_EL2_Value = 0;
    uint64_t SCTLR_EL3_Value = 0;   // Secure EL3
    __asm__ volatile(// EL1.M = 1
                     "mrs %0, SCTLR_EL1;"
                     "orr %0, %0 , #1;"          // set bit[0] bit
                     "msr SCTLR_EL2, %0;"
                     // EL2.M = 0
                     "mrs %1, SCTLR_EL2;"
                     "orr %1, %1 , #0;"          // set bit[0] bit
                     "msr SCTLR_EL2, %1;"
                     // EL3.M = 0
                     "mrs %1, SCTLR_EL3;"
                     "orr %1, %1 , #0;"          // set bit[0] bit
                     "msr SCTLR_EL3, %1;"
                                
                     :"=r"(SCTLR_EL1_Value), "=r"(SCTLR_EL2_Value), "=r"(SCTLR_EL3_Value)
                     :
                    );
}

void vm_mmu_context_switch(vm_pte_le1_t *new_proc_pte1, vm_pte_le1_t **old_proc_pte1) {
    // TODO: 原子性 or 关调度 ?
    vm_pte_le1_t* old_pte1;
    __asm__ volatile("mrs %0, TTBR0_EL1;"    // save old TTBR
                     "msr TTBR0_EL1, %1;"    // set new TTBR
                     
                     :"=r"(old_pte1)
                     :"r"(new_proc_pte1)
                     );
    *old_proc_pte1 = old_pte1;
}
