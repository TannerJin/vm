//
//  pm_.c
//  vm
//
//  Created by jintao on 2020/1/19.
//  Copyright © 2020 jintao. All rights reserved.
//

#include "pm_.h"

void pm_free_list_add_node(pm_free_list_t* node) {
    if (!pm_free_list_header_vm_addr) {
        pm_free_list_header_vm_addr = node;
    }
    
    if (pm_free_list_totle_vm_addr) {
        pm_free_list_t* last_totle_node = pm_free_list_totle_vm_addr;
        last_totle_node->next = node;
    }
    pm_free_list_totle_vm_addr = node;
}

pm_free_list_t* pm_free_list_remove_node(void) {
    if (pm_free_list_header_vm_addr) {
        pm_free_list_t *current = pm_free_list_header_vm_addr;
        pm_free_list_header_vm_addr = current->next;
        if (pm_free_list_totle_vm_addr == pm_free_list_header_vm_addr) {
            pm_free_list_totle_vm_addr = 0;
        }
        return current;
    }
    return 0;
}

void pm_configure() {
    // pm 大小
    
}
