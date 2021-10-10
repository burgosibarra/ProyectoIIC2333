//FUNCIONES GENERALES
#include "crms_API.h"
#include <string.h>

const int FILE_NAME_SIZE = 12;
const int PROCESS_AMOUNT = 16;
const int NAME_SIZE = 12;
const int PROCESS_FILE_VIRTUAL_MEMORY = 4;
const int PROCESS_FILES_AMOUNT = 10;
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

void function()
{
    printf("Función -> file_direction = %s\n", *file_direction);
}

void cr_mount(char* memory_path)
{
    //Función para guardar ruta de memoria
    //file_direction = malloc(strlen(memory_path) * sizeof(char));
    //for (int i = 0; i < strlen(memory_path); i++)
    //{
    //    *file_direction[i] = memory_path[i];
    //}
    char* aux = malloc(14);
    for (int i = 0; i < (int) strlen(memory_path) + 1; i++)
    {
        strcpy(&aux[i], &memory_path[i]);
    }
    file_direction = &(aux);
    //printf("ESTE ES EL FILE DIRECTION: %s\n", *file_direction);
    /*for (int i = 0; i < (int) strlen(memory_path); i++)
    {
        printf("Letra: %c\n", (*file_direction)[i]);
    }
    printf("\n");*/
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
            // printf("- %i\n", status);
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
          // printf("%i\n", status);
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

int name_coincidence(char* file_name_found, char* file_name)
{
    for (int index = 0; index < NAME_SIZE; index++)
    {
        if ((char) ((int) file_name_found[index]) != file_name[index]) return 0;
    }
    return 1;
}

//Funcion para revisar si existe el archivo en la memoria del proceso
int cr_exists(int process_id, char* file_name)
{
    for (int i=0; i < PROCESS_AMOUNT; i++)
    {
        if (pcb_table[i]->state == 0x01 && pcb_table[i]->id == process_id)
        {
            for (int j=0; j < PROCESS_FILES_AMOUNT; j++)
            {
                if (pcb_table[i]->files[j]->validity == 0x01 && name_coincidence(pcb_table[i]->files[j]->name, file_name))
                {
                    return 1;
                }
            }
            return 0;
        }
    }
    // levantar un error porque el proceso no exite
  return 0;
}

void cr_ls_files(int process_id)
{
    int process_found = 0;
    for (int i=0; i < PROCESS_AMOUNT; i++)
    {
        if (pcb_table[i]->state == 0x01 && pcb_table[i]->id == process_id)
        {
            process_found = 1;
          printf("Los archivos del proceso [%i] ", process_id);
          for (int character = 0; character < NAME_SIZE; character++)
            {
                printf("%c", (char) ((int) pcb_table[i]->name[character]));
            }
            printf(" son:\n");
            int file_number = 1;
            for (int j=0; j < PROCESS_FILES_AMOUNT; j++)
            {
                if (pcb_table[i]->files[j]->validity == 0x01)
                {
                    printf("%i. ", file_number);
                    for (int character = 0; character < NAME_SIZE; character++)
                    {
                        printf("%c", (char) ((int) pcb_table[i]->files[j]->name[character]));
                    }
                    printf("\n");
                    file_number++;
                }
            }
        }
    }
    if (process_found == 0)
    {
        // levantar error
    }
}
  

//FUNCIONES PROCESOS
void cr_start_process(int process_id, char* process_name)
{
    for (int index = 0; index < PROCESS_AMOUNT; index++)
    {
        if (pcb_table[index]->state == 0x01 && pcb_table[index]->id == process_id)
        {
            printf("El proceso que quieres iniciar ya está iniciado uwu WARNING!!!!!!!!!!!!\n");
            return;
        }
    }
    for (int index = 0; index < PROCESS_AMOUNT; index++)
    {
        if (pcb_table[index]->state == 0x00)
        {
            pcb_table[index]->state = 0x01;
            pcb_table[index]->id = process_id;
            int name_length = strlen(process_name);
            if (name_length < NAME_SIZE)
            {
                for (int j = 0; j < name_length; j++)
                {
                    pcb_table[index]->name[j] = process_name[j]; 
                }
                for (int j = name_length; j < NAME_SIZE; j++)
                {
                    pcb_table[index]->name[j] = ""; 
                }
                return;
            } else {
                for (int j = 0; j < NAME_SIZE; j++)
                {
                    pcb_table[index]->name[j] = process_name[j]; 
                }
                return;
            }
        }
    }
}

void cr_finish_process(int process_id)
{
    
  for (int i = 0; i < PROCESS_AMOUNT; i++)
  {
    if (pcb_table[i] -> state == 0x01 && pcb_table[i] -> id == process_id)
    {
        //TERMINAR PROCESO Y LIBERAR MEMORIA
        printf("file_direction = %s\n", *file_direction);
        FILE* memory = fopen(*file_direction, "rb");
        fseek(memory, 4096, SEEK_SET);
        pcb_table[i] -> state = 0x00;
        uint8_t entry;
        uint8_t pfn;
        uint8_t validity = 0b10000000;
        uint8_t pfn_mask = 0b01111111;
        int byte_position;
        int bit_position;
        uint8_t frame_bitmap;
        for (int page_number = 0; page_number < 16; page_number++)
        {
            entry = (uint8_t) pcb_table[i]->page_table -> entries[page_number];
            if ((entry & validity) == validity)
            {
                pfn = entry & pfn_mask;
                byte_position = pfn/8; //1
                bit_position = pfn % 8; //3
                fseek(memory, byte_position, SEEK_CUR);
                fread(&frame_bitmap, 1, 1, memory);
                fclose(memory);
                frame_bitmap = frame_bitmap & !((int) pow(2,bit_position));
                //fseek(memory, -1, SEEK_CUR);
                FILE* memory = fopen(*file_direction, "wb");
                fseek(memory, 4096 + byte_position, SEEK_SET);
                fwrite(&frame_bitmap, 1, 1, memory);
            }
        }
        fclose(memory);
    }
  
    if (i==16)
    {
        //Implementar Error
        printf("No existe un proceso con ese ID");
    }
    }
    //Funcion que termina el proceso con id process_id
    }


