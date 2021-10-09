//FUNCIONES GENERALES
#include "crms_API.h"

const int FILE_NAME_SIZE = 12;
const int PROCESS_AMOUNT = 16;
const int NAME_SIZE = 12;
const int PROCESS_FILE_VIRTUAL_MEMORY = 4;
const int PROCESS_FILES_AMOUNT = 12;
const int PROCESS_FILE_SIZE = 4;
const int PCB_SIZE = 256;
const int PAGE_TABLE_ENTRIES = 32;


CrmsFile* crmsfile_init()
{
    CrmsFile* crmsfile = malloc(sizeof(CrmsFile));
    crmsfile -> validity = 0x00;
    crmsfile -> name = malloc(NAME_SIZE * sizeof(char));
    crmsfile -> file_size = 0;
    crmsfile -> start_address = 0;
    crmsfile -> finished_address = 0;
    return crmsfile;
}

void crmsfile_create(CrmsFile* crmsfile, char* name, uint32_t file_size, uint32_t start_address, uint32_t finished_address)
{
    crmsfile -> validity = 0x01;
    for (int index = 0; index < NAME_SIZE; index++)
    {
        crmsfile -> name[index] = name[index];
    }
    crmsfile -> file_size = file_size;
    crmsfile -> start_address = start_address;
    crmsfile -> finished_address = finished_address;
}


PCB* pcb_init(uint8_t state, uint8_t id, char* name)
{
    PCB* pcb = malloc(sizeof(PCB));
    pcb -> state = state;
    pcb -> id = id;
    pcb -> name = malloc(NAME_SIZE * sizeof(char));
    for (int index = 0; index < NAME_SIZE; index++)
    {
        pcb -> name[index] = name[index];
    }
    pcb -> files = malloc(PROCESS_FILES_AMOUNT * sizeof(CrmsFile*));
    for (int file = 0; file < PROCESS_FILES_AMOUNT; file++)
    {
        pcb -> files[file] = crmsfile_init();
    }
    pcb -> page_table = page_table_init();
    return pcb;
}

void pcb_destroy(PCB* pcb)
{
    for (int file = 0; file < PROCESS_FILES_AMOUNT; file++)
    {
        crmsfile_destroy(pcb -> files[file]);
    }

    page_table_destroy(pcb -> page_table);

    free(pcb->name);
    free(pcb);
}


void crmsfile_destroy(CrmsFile* crmsfile)
{
    free(crmsfile -> name);
    free(crmsfile);
}


void cr_mount(char* memory_path)
{
    //Función para guardar ruta de memoria
    file_direction = &(memory_path);

    FILE* memory;
    memory = fopen(*file_direction, "rb");  // r for read, b for binary
    uint8_t status;
    uint8_t process_id_found;
    char* process_name = malloc(NAME_SIZE * sizeof(char));
    char* file_name = malloc(NAME_SIZE * sizeof(char)); // PROCESS_FILE_NAME
    for (int pcb = 0; pcb < PROCESS_AMOUNT; pcb++)
    {
        pcb_table[pcb] = pcb_init(0x00, 0, "MEGUSTAELPAN");
        fread(&status, 1, 1, memory);
        fread(&process_id_found, 1, 1, memory);

        if (status == 0x01)
        {

            fread(process_name, 1, NAME_SIZE, memory); //OJOOOOOO
            pcb_table[pcb]->state = 0x01;
            pcb_table[pcb]->id = process_id_found;
            
            for (int index=0; index < NAME_SIZE; index++)
            {
                pcb_table[pcb]->name[index] = process_name[index];
            }

            uint8_t validation;
            uint32_t file_size; // PROCESS_FILE_SIZE
            uint32_t virtual_memory; // PROCESS_FILE_VIRTUAL_MEMORY
            for (int file = 0; file < PROCESS_FILES_AMOUNT; file++)
            {
                fread(&validation, 1, 1, memory);
                fread(file_name, 1, NAME_SIZE, memory); //OJOOOOOOOO

                fread(&file_size, PROCESS_FILE_SIZE, 1, memory);
                //file_size = bswap_32(file_size); //OJOOOOOOO

                fread(&virtual_memory, PROCESS_FILE_VIRTUAL_MEMORY, 1, memory);
                //virtual_memory = bswap_32(virtual_memory); // OJOOOOOOOO

                if (validation == 0x01)
                {
                    // Crear el struct
                    // ojo que parametros están en el stack
                    uint32_t start_virtual_address = virtual_memory;
                    uint32_t finished_virtual_address = virtual_memory + file_size;
                    crmsfile_create(pcb_table[pcb]->files[file], file_name, file_size, start_virtual_address, finished_virtual_address);                    
                }
            }
            uint8_t page_info;
            for (int page = 0; page < PAGE_TABLE_ENTRIES; page++)
            {
                fread(&page_info, 1, 1, memory);
                pcb_table[pcb]->page_table->entries[page] = page_info;
            }
        }
        else
        {
            fseek(memory, PCB_SIZE - 2, SEEK_CUR);
        }
    }
    free(process_name);
    free(file_name);
    fclose(memory);
}

void cr_ls_process()
{
    //Funcion que muestra los procesos en ejecucion
    printf("Los ids y nombres de los procesos en ejecución son los siguientes: \n");
    for (int i = 0; i < PROCESS_AMOUNT ; i++)
    {
        if (pcb_table[i] -> state == 0x01)
        {
            printf(" * [%i] ", pcb_table[i] -> id);
            for (int character = 0; character < NAME_SIZE; character++)
            {
                printf("%c", (char) ((int) pcb_table[i]->name[character]));
            }
            printf("\n");
        }
    }
}

int cr_exists(int process_id, char* file_name)
{
    //Funcion para revisar si existe el archivo en la memoria del proceso
    return 0;
}

void cr_ls_files(int process_id)
{
    //Funcion para listar archivos dentro de la memoria del proceso
}
  

//FUNCIONES PROCESOS
void cr_start_process(int process_id, char* process_name)
{
    //Inicia proceso con nombre process_name y id process_id
}

void cr_finish_process(int process_id)
{
    //Funcion que termina el proceso con id process_id
}


