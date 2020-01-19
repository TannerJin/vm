//
//  pm_page.c
//  vm
//
//  Created by jintao on 2020/1/19.
//  Copyright © 2020 jintao. All rights reserved.
//

#include "pm_page.h"
#include "vm_kernel.h"
#include "vm_page_table.h"

pm_address_t pm_alloc_page() {
    // 1. pm_free_list
    if (pm_free_list_header_vm_addr) {
        pm_free_list_t *current = pm_free_list_remove_node();
        pm_address_t pm_addr = current->pm_addr;
        kern_free(current);
        return pm_addr;
    }
        
    // 2. pm_addr
    if (pm_current_use_pm_addr <= PM_MAX_ADDRESS) {
        pm_address_t pm_addr = pm_current_use_pm_addr;
        
        // TODO: 原子性
        pm_current_use_pm_addr += 1;
        return (pm_addr << VM_GRANULE_PAGE_SHIFT);
    }
    
    // TODO: Kern Fault
    return 0;
}


void pm_free_page(pm_address_t page_pm_addr) {
    pm_free_list_t *free_node = kern_malloc(sizeof(pm_free_list_t));
    free_node->pm_addr = page_pm_addr;
    free_node->next = 0;
    pm_free_list_add_node(free_node);
}
