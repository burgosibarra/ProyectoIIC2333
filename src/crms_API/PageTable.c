#include "PageTable.h"

const int ADDRESSES_AMOUNT = 32;

PageTable* page_table_init()
{
    PageTable* page_table = malloc(sizeof(PageTable));
    page_table -> entries = malloc(ADDRESSES_AMOUNT * sizeof(uint8_t));
    return page_table;
}


void page_table_destroy(PageTable* page_table)
{
    free(page_table -> entries);
    free(page_table);
}