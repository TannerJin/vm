//
//  vm_kernel.h
//  vm
//
//  Created by jintao on 2020/1/14.
//  Copyright © 2020 jintao. All rights reserved.
//

#ifndef vm_kernel_h
#define vm_kernel_h

#include "vm_page_table.h"

#define VM_Kernel_Mask 0xFFFF_0000_0000_0000

// pm_kernel_page_table_base_addr
static vm_pte_le1_t* kernel_ptb_pm_addr;

void* kern_malloc(uint64_t size);

#endif
