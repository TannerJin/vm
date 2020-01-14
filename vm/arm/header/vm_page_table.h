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
    [ARMv8 block and table formats](https://armv8-ref.codingbelief.com/en/chapter_d4/d43_1_vmsav8-64_translation_table_descriptor_formats.html)
 */

// MARK: Level 1(table)
typedef struct vm_page_el1 {
    uint64_t nstable:1;                     // 63
    uint64_t aptable:2;                     // 62 ~ 61
    uint64_t xntable:1;                     // 60
    uint64_t pxntable:1;                    // 59
    uint64_t ignored0:7;                    // 58 ~ 52
    uint64_t res0:4;                        // 51 ~ 48
    uint64_t next_level_addr:36;            // 47 ~ 12
    uint64_t ignored1:10;                   // 11 ~ 2/Users/jintao/Documents/T-OS/vm
    uint64_t format:2;                      //  1 ~ 0    *value: 11*
} vm_page_el1_t;


// MARK: Level 2(table)
typedef struct vm_page_el1_t vm_page_el2_t;


// MARK: Level 3(block)
typedef struct vm_page_pte {
    uint64_t upper_block_attr:12;           // 63 ~ 52
    uint64_t res0:4;                        // 51 ~ 48
    uint64_t physics_addr:26;               // 47 ~ 22
    uint64_t res1:10;                       // 21 ~ 12
    uint64_t lower_block_attr:10;           // 11 ~ 2
    uint64_t format:2;                      //  1 ~ 0    *value: 01*
} vm_page_pte_t;






