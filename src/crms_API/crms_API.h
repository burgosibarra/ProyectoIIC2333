#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <byteswap.h>
#include <string.h>
#include "PageTable.h"
#include "mergeSort.h"


struct crmsfile;

typedef struct crmsfile CrmsFile;

struct pcb;

typedef struct pcb PCB;

struct crmsfile
{
    uint8_t validity;
    char* name;
    char mode;
    uint32_t file_size;
    uint32_t start_address;
    uint32_t finished_address;
    PCB* pcb;
};

struct pcb
{
    uint8_t state;
    uint8_t id;
    char* name;
    CrmsFile** files;
    PageTable* page_table;
};

/*
PCB* pcb_table[16];
// https://stackoverflow.com/questions/48538425/c-modify-global-char-array
char file_direction[];
*/

PCB* pcb_init();
void pcb_create(PCB* pcb, int state, int id, char* name);
void pcb_destroy(PCB* pcb);

CrmsFile* crmsfile_init(PCB* pcb);
void crmsfile_create(CrmsFile* crmsfile, char* name, uint32_t file_size, uint32_t start_address, uint32_t finished_address, char mode);
void crmsfile_destroy(CrmsFile* crmsfile);

//Funciones generales
void cr_mount(char* memory_path);
void cr_ls_process();
int cr_exists(int process_id, char* file_name);
void cr_ls_files(int process_id);

//Funciones procesos
void cr_start_process(int process_id, char* process_name);
void cr_finish_process(int process_id);

// Funciones archivos
CrmsFile* cr_open(int process_id, char* file_name, char mode);
int cr_write_file(CrmsFile* file_desc, void* buffer, int n_bytes);
int cr_read(CrmsFile* file_desc, void* buffer, int n_bytes);
void cr_delete_file(CrmsFile* file_desc);
void cr_close(CrmsFile* file_desc);