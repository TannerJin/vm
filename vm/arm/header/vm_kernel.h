//
//  vm_kernel.h
//  vm
//
//  Created by jintao on 2020/1/14.
//  Copyright © 2020 jintao. All rights reserved.
//

#include "uint64_t.h"

#define VM_Kernel_Mask 0xFFFF_0000_0000_0000

// pm_kernel_page_table_base_addr
static uint64_t* kernel_ptb_paddr;
