//
//  vm_automic.c
//  vm
//
//  Created by jintao on 2020/1/31.
//  Copyright © 2020 jintao. All rights reserved.
//

#include "vm_automic.h"

_Bool VMAtomicCompareAndSwap32(int32_t oldValue, int32_t newValue, int32_t* theValue) {
    __asm__ volatile(
                     "ldxr x0, [%0]\n"
//                     "cmp x0, [%1]\n"
//                     "strexeq x1;"
                     :
                     :"r"(theValue), "o"(oldValue)
                     :"memory"
                     );
    return 1;
}


__attribute__((target("lse")))
int32_t VMAtomicAdd32(int32_t theCount, int32_t* theValue) {
    int32_t result = 0;
    __asm__ volatile(
                     "mov w0, %w2\n"
                     "mov x2, %1\n"
                     "ldadd w0, w1, [x2]\n"    // w0: theCount值; x2: theValue地址; w1: 加之前theValue值
                     "add w1, w1, w0\n"
                     "mov %w0, w1\n"
                  
                     :"=&r"(result)
                     :"r"(theValue), "r"(theCount)
                     :"memory", "w0", "w1", "x2"
                    );
    return result;
}
