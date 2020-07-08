#ifndef __RD_PGMAP_H
#define __RD_PGMAP_H
#include <stdint.h>
#include <stdbool.h>

int read_pagemap(unsigned long virt_addr);
int calc_num_pages(uint64_t region_size,uint64_t page_size);

#endif // __RD_PGMAP_H