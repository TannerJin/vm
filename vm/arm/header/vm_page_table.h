//
//  vm_page_table.h
//  vm
//
//  Created by jintao on 2020/1/13.
//  Copyright © 2020 jintao. All rights reserved.
//

#ifndef vm_page_table_h
#define vm_page_table_h

#include "vm_.h"
#include "pm_.h"

/*
    Document
    [translation lookup bits](https://armv8-ref.codingbelief.com/zh/chapter_d4/d42_3_memory_translation_granule_size.html)
    [ARMv8 block and table formats](https://armv8-ref.codingbelief.com/en/chapter_d4/d43_1_vmsav8-64_translation_table_descriptor_formats.html)
 */


/*
   [ARM64(ARMv8-A) VM Architecture]
   level lookup: 4 level
   granule size: 16 KB  (TCR_ELI.tg)

   63       48     47    46         36 35         25 24          14 13        0
   +--------------------------------------------------------------------------+
   |  kernel  | level 0 |   level 1   |   level 2   |    level 3   |   page   |
   +--------------------------------------------------------------------------+
                               vm_address
 
   translation table base address registers:
       TTBR1_EL(1)    -> kernel
       TTBR0_EL(n)    -> user
*/



/* Page Table Entry (Table)
 
 */
typedef union vm_lt_table {
    uint64_t value;
    struct {
        uint64_t format:2;                      //  1 ~ 0    *value: 11*
        uint64_t ignored1:12;                   // 13 ~ 2
        uint64_t next_level_addr:34;            // 47 ~ 14
        uint64_t res0:4;                        // 51 ~ 48
        uint64_t ignored0:7;                    // 58 ~ 52
        uint64_t pxntable:1;                    // 59
        uint64_t xntable:1;                     // 60
        uint64_t aptable:2;                     // 62 ~ 61
        uint64_t nstable:1;                     // 63
    };
} vm_lt_table_t;


/* Page Table Entry (Block)
 
 */
typedef union vm_lt_block {
    uint64_t value;
    struct {
        uint64_t format:2;                      //  1 ~ 0    *value: 01*
        uint64_t lower_block_attr:10;           // 11 ~ 2
        uint64_t lower_res0:13;                 // 24 ~ 12
        uint64_t output_addr:23;                // 47 ~ 25
        uint64_t upper_res0:4;                  // 51 ~ 48
        uint64_t upper_block_attr:12;           // 63 ~ 52
    };
} vm_lt_block_t;


/*  Level 0 and 1
    name: page table level1
 */
typedef vm_lt_table_t vm_pte_le1_t;

#define VM_PTE_LE0_MASK 0x0000800000000000
#define VM_PTE_LE0_INDEX(vm_addr) ((vm_addr & VM_PTE_LE0_MASK) >> 47)

#define VM_PTE_LE1_MASK 0x00007FF000000000
#define VM_PTE_LE1_INDEX(vm_addr) ((vm_addr & VM_PTE_LE1_MASK) >> 36)


/*  Level 2
    name: page table level2
 */
typedef union vm_pte_le2 {
    uint64_t format:2;              // 1: block; 3: table
    vm_lt_table_t table;
    vm_lt_block_t block;
} vm_pte_le2_t;

#define VM_PTE_LE2_MASK 0x0000000FFE000000
#define VM_PTE_LE2_INDEX(vm_addr) ((vm_addr & VM_PTE_LE2_MASK) >> 25)


/*  Level 3  (16KB Pages Descriptor)
    name: page table level3
 */
typedef union vm_pte_le3 {
    uint64_t value;
    struct {
        uint64_t format:2;                      //  1 ~ 0       *value: 1 1*
        uint64_t lower_attr:10;                 // 11 ~ 2
        uint64_t unknow:2;                      // 13 ~ 12
        uint64_t physics_addr_base:34;          // 47 ~ 14
        uint64_t rest0:4;                       // 51 ~ 48
        uint64_t upper_attr:12;                 // 63 ~ 52
    };
} vm_pte_le3_t;

#define VM_PTE_LE3_MASK 0x0000000001FFC000
#define VM_PTE_LE3_INDEX(vm_addr) ((vm_addr & VM_PTE_LE3_MASK) >> 14)


/*  VM_GRANULE
    
 */
#define VM_GRANULE_PAGE_SHIFT 14
#define VM_GRANULE_PAGE_MASK 0x0000000000003FFF
#define VM_GRANULE_PAGE_OFFSET(vm_addr) (vm_addr & VM_GRANULE_PAGE_MASK)


/// vm_addr  -> pm_addr
/// @param vm_ttbr0_le0 level0 translate page table base addr (vm)
/// @param vm_addr  vm_addr
pm_address_t vm_addr_tanslate_to_physics_addr(vm_pte_le1_t* vm_ttbr0_le0, vm_address_t vm_addr);

void physics_addr_map_to_vm_addr(vm_pte_le1_t* vm_ttbr0_le0, vm_address_t vm_addr, pm_address_t pm_addr);

#endif
