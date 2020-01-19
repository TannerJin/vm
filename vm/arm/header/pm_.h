//
//  pm_.h
//  vm
//
//  Created by jintao on 2020/1/19.
//  Copyright © 2020 jintao. All rights reserved.
//

#ifndef pm__h
#define pm__h

#include "uint64_t.h"

typedef uint64_t pm_address_t;

typedef struct free_list {
    pm_address_t pm_addr;
    void* next;
} pm_free_list_t;


// 空闲(释放了的)物理地址(表示16KB)
static pm_free_list_t *pm_free_list_header_vm_addr;  
static pm_free_list_t *pm_free_list_totle_vm_addr;

void pm_free_list_add_node(pm_free_list_t* node);
pm_free_list_t* pm_free_list_remove_node(void);


// 当前使用的物理地址页帧  0 ~ (2^(64-14)-1)
#define PM_MIN_ADDRESS 0
#define PM_MAX_ADDRESS 0x3ffffffffffff
static pm_address_t pm_current_use_pm_addr = 0;


void pm_configure(void);

#endif /* pm__h */
