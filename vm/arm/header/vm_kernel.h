//
//  vm_kernel.h
//  vm
//
//  Created by jintao on 2020/1/14.
//  Copyright © 2020 jintao. All rights reserved.
//

#include "vm_page_table.h"

#define VM_Kernel_Mask 0xFFFF_0000_0000_0000

// pm_kernel_page_table_base_addr
static union vm_pt_le1_entry_t* kernel_ptb_addr;
