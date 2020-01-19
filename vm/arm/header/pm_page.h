//
//  pm_page.h
//  vm
//
//  Created by jintao on 2020/1/19.
//  Copyright © 2020 jintao. All rights reserved.
//

#ifndef pm_page_h
#define pm_page_h

#include "pm_.h"

pm_address_t pm_alloc_page(void);

void pm_free_page(pm_address_t page_pm_addr);

#endif /* pm_page_h */
