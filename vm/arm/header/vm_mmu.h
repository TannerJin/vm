//
//  vm_mmu.h
//  vm
//
//  Created by jintao on 2020/1/14.
//  Copyright © 2020 jintao. All rights reserved.
//

/*
   Documents:
   [ARMv8-A Address Translation](https://static.docs.arm.com/100940/0100/armv8_a_address%20translation_100940_0100_en.pdf)
   [SMMU Architecture](https://static.docs.arm.com/ihi0062/dc/IHI0062D_c_system_mmu_architecture_specification.pdf)
*/


/*
   [VM Architecture] -> level 3 and 4KB for ARM64(ARMv8-A)

   63       48 47                             11      0
   +--------------------------------------------------+
   |  kernel  |        user addr              |  page |
   +--------------------------------------------------+
                   vm_address

   translation table base address registers:
       TTBR1_EL(1)     -> kernel
       TTBR0_EL(2/3)   -> user

   translation control register(TCR_ELI):
       T0SZ and T1SZ are all 0 or all 1
       Translation Granule(TG) -> 4KB(00)
*/

void vm_mmu_init(void);
