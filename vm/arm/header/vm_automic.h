//
//  vm_automic.h
//  vm
//
//  Created by jintao on 2020/1/31.
//  Copyright © 2020 jintao. All rights reserved.
//

#ifndef vm_automic_h
#define vm_automic_h

#include <stdio.h>
#include <int32_t.h>

_Bool VMAtomicCompareAndSwap32(int32_t oldValue, int32_t newValue, int32_t* theValue);

int32_t VMAtomicAdd32(int32_t theCount, int32_t* theValue);

#endif /* vm_automic_h */
