#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <byteswap.h>
#include "PageTable.h"


struct crmsfile;
typedef struct crmsfile CrmsFile;

struct crmsfile
{
    uint8_t validity;
    char* name;
    uint32_t file_size;
    uint32_t start_address;
    uint32_t finished_address;
};

struct pcb;
typedef struct pcb PCB;
struct pcb
{
    uint8_t state;
    uint8_t id;
    char* name;
    CrmsFile** files;
    PageTable* page_table;
};

PCB* pcb_table[16];
char* file_direction;

PCB* pcb_init(uint8_t state, uint8_t id, char* name);
void pcb_destroy(PCB* pcb);

CrmsFile* crmsfile_init();
void crmsfile_create(CrmsFile* crmsfile, char* name, uint32_t file_size, uint32_t start_address, uint32_t finished_address);
void crmsfile_destroy(CrmsFile* crmsfile);

void cr_mount(char* memory_path);
void cr_ls_process();
int cr_exists(int process_id, char* file_name);
void cr_ls_files(int process_id);
void cr_start_process(int process_id, char* process_name);
void cr_finish_process(int process_id);