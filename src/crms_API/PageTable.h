#pragma once
#include <stdint.h>
#include <stdlib.h>

struct page_table;

typedef struct page_table PageTable;

struct page_table
{
    uint8_t* entries;
};

PageTable* page_table_init();
void page_table_destroy(PageTable* page_table);