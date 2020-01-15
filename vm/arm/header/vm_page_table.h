//
//  vm_page_table.h
//  vm
//
//  Created by jintao on 2020/1/13.
//  Copyright © 2020 jintao. All rights reserved.
//

#include "uint64_t.h"

/*
    Document
    [translation lookup bits](https://armv8-ref.codingbelief.com/zh/chapter_d4/d42_3_memory_translation_granule_size.html)
    [ARMv8 block and table formats](https://armv8-ref.codingbelief.com/en/chapter_d4/d43_1_vmsav8-64_translation_table_descriptor_formats.html)
 */


/*
   [ARM64(ARMv8-A) VM Architecture]
   level lookup: 3 level
   granule size: 16 KB

   63       48 47         36 35         25 24          14 13        0
   +----------------------------------------------------------------+
   |  kernel  |   level 1   |   level 2   |    level 3   |   page   |
   +----------------------------------------------------------------+
                               vm_address
 
   translation table base address registers:
       TTBR1_EL(1)    -> kernel
       TTBR0_EL(n)    -> user
*/


// MARK: Level Table
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


// MARK: Level Block
typedef union vm_lt_block {
    uint64_t value;
    struct {
        uint64_t format:2;                      //  1 ~ 0    *value: 01*
        uint64_t lower_block_attr:10;           // 11 ~ 2
        uint64_t res1:13;                       // 24 ~ 12
        uint64_t output_addr:23;                // 47 ~ 25
        uint64_t res0:4;                        // 51 ~ 48
        uint64_t upper_block_attr:12;           // 63 ~ 52
    };
} vm_lt_block_t;


/*  Level 0 and 1
    name: page table level1
 */
typedef vm_lt_block_t vm_pt_le1_entry_t;


/*  Level 2
    name: page table level2
 */
typedef union vm_pt_le2_entry {
    vm_lt_table_t table;
    vm_lt_block_t block;
} vm_pt_le2_entry_t;


/*  Level 3  (16KB Pages Descriptor)
    name: page table level3
 */
typedef union vm_pt_le3_entry {
    uint64_t value;
    struct {
        uint64_t format:2;                      //  1 ~ 0       *value: 1 1*
        uint64_t lower_attr:10;                 // 11 ~ 2
        uint64_t unknow:2;                      // 13 ~ 12
        uint64_t physics_addr:34;               // 47 ~ 14
        uint64_t rest0:4;                       // 51 ~ 48
        uint64_t upper_attr:12;                 // 63 ~ 52
    };
} vm_pt_le3_entry_t;

