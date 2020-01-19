//
//  vm_page_table.c
//  vm
//
//  Created by jintao on 2020/1/13.
//  Copyright © 2020 jintao. All rights reserved.
//

#include "vm_page_table.h"
#include "vm_kernel.h"
#include "vm_mmu.h"
#include "int64_t.h"

pm_address_t vm_addr_tanslate_to_physics_addr(vm_pte_le1_t* vm_ttbr0_le0, vm_address_t vm_addr) {
    // level 0
    int64_t pte0_index = VM_PTE_LE0_INDEX(vm_addr);
    vm_pte_le1_t* pte1_addr = (vm_pte_le1_t *)vm_ttbr0_le0[pte0_index].next_level_addr;
    
    // level 1
    int64_t pte1_index = VM_PTE_LE1_INDEX(vm_addr);
    vm_pte_le2_t* pte2_addr = (vm_pte_le2_t*)pte1_addr[pte1_index].next_level_addr;
    
    // level 2
    int64_t pte2_index = VM_PTE_LE2_INDEX(vm_addr);
    uint64_t pte2_format = pte2_addr[pte2_index].format;
    
    if (pte2_format == 1) {
        // block, no physics_addr
        return 0;       // 0 or -1 ?
    }
    vm_pte_le3_t* pte3_addr = (vm_pte_le3_t*)pte2_addr[pte2_index].table.next_level_addr;
    
    // level 3
    int64_t pte3_index = VM_PTE_LE3_INDEX(vm_addr);
    vm_pte_le3_t *pte3 = pte3_addr + pte3_index;
    
    return pte3->physics_addr_base + VM_GRANULE_PAGE_OFFSET(vm_addr);
}


void physics_addr_map_to_vm_addr(vm_pte_le1_t* vm_ttbr0_le0, vm_address_t vm_addr, pm_address_t pm_addr) {
    // level 0
    int64_t pte0_index = VM_PTE_LE0_INDEX(vm_addr);
    vm_pte_le1_t* pte1_addr = (vm_pte_le1_t *)vm_ttbr0_le0[pte0_index].next_level_addr;     // TODO: 这里应该手动转换成虚拟地址??
   
    // level 1
    int64_t pte1_index = VM_PTE_LE1_INDEX(vm_addr);
    vm_pte_le2_t* pte2_addr = (vm_pte_le2_t*)pte1_addr[pte1_index].next_level_addr;
   
    // level 2
    int64_t pte2_index = VM_PTE_LE2_INDEX(vm_addr);
    uint64_t pte2_format = pte2_addr[pte2_index].format;
   
    if (pte2_format == 1) {
        // now is block, creat Table
        vm_pte_le3_t* pte_le3_addr = kern_malloc((1<<11)*sizeof(uint64_t));
        if (pte_le3_addr != 0) {
            (pte2_addr + pte2_index)->table.next_level_addr = (uint64_t)pte_le3_addr;       // TODO: 这里应该手动转换成物理地址??
            (pte2_addr + pte2_index)->format = 3;
        } else {
            // TODO: kernel fault
        }
    }
    vm_pte_le3_t* pte3_addr = (vm_pte_le3_t*)pte2_addr[pte2_index].table.next_level_addr;
   
    // level 3
    int64_t pte3_index = VM_PTE_LE3_INDEX(vm_addr);
    vm_pte_le3_t* pte3 = pte3_addr + pte3_index;
    
    // pm map vm
    pm_address_t pm_addr_base = (pm_addr & (~VM_GRANULE_PAGE_MASK)) >> 14;
    pte3->physics_addr_base = pm_addr_base;
}
