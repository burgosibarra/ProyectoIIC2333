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
const int PROCESS_FILE_MAX_SIZE = 32;


CrmsFile* crmsfile_init()
{
    CrmsFile* crmsfile = malloc(sizeof(CrmsFile));
    crmsfile -> validity = 0x00;
    crmsfile -> name = malloc(NAME_SIZE * sizeof(char));
    crmsfile -> file_size = 0;
    crmsfile -> mode = '-';
    crmsfile -> start_address = 0;
    crmsfile -> finished_address = 0;
    return crmsfile;
}

void crmsfile_create(CrmsFile* crmsfile, char* name, uint32_t file_size, uint32_t start_address, uint32_t finished_address, char mode)
{
    crmsfile -> validity = 0x01;
    for (int index = 0; index < NAME_SIZE; index++)
    {
        crmsfile -> name[index] = name[index];
    }
    crmsfile -> file_size = file_size;
    crmsfile -> start_address = start_address;
    crmsfile -> finished_address = finished_address;
    crmsfile -> mode = mode;
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
    printf("Función -> file_direction = %s\n", file_direction);
}

void print_name(uint8_t* buffer, int size)
{
    for(int i = 0; i < size; i++)
    {
        printf("%c", (char) buffer[i]);
    }
    printf("\n");
}

void cr_mount(char* memory_path)
{
    //Función para guardar ruta de memoria
    //file_direction = malloc(strlen(memory_path) * sizeof(char));
    //for (int i = 0; i < strlen(memory_path); i++)
    //{
    //    file_direction[i] = memory_path[i];
    //}
    char aux[14];
    for (int i = 0; i < (int) strlen(memory_path) + 1; i++)
    {
        aux[i] = memory_path[i];
    }
    strcpy(file_direction, aux);
    //printf("ESTE ES EL FILE DIRECTION: %s\n", file_direction);
    /*for (int i = 0; i < (int) strlen(memory_path); i++)
    {
        printf("Letra: %c\n", (file_direction)[i]);
    }
    printf("\n");*/
    FILE* memory;
    memory = fopen(file_direction, "rb");  // r for read, b for binary
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
                file_size = bswap_32(file_size); //OJOOOOOOO

                fread(&virtual_memory, PROCESS_FILE_VIRTUAL_MEMORY, 1, memory);
                virtual_memory = bswap_32(virtual_memory); // OJOOOOOOOO

                if (validation == 0x01)
                {
                    // Crear el struct
                    // ojo que parametros están en el stack
                    uint32_t start_virtual_address = virtual_memory;
                    uint32_t finished_virtual_address = virtual_memory + file_size;
                    crmsfile_create(pcb_table[pcb]->files[file], file_name, file_size, start_virtual_address, finished_virtual_address, '-');
                    printf("Process: [%i] File name: [%s] FILE SIZE: %i\n", pcb_table[pcb] -> id, pcb_table[pcb] -> files[file]->name, pcb_table[pcb]->files[file]->file_size);
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
        FILE* memory = fopen(file_direction, "rb+");
        uint8_t status;
        uint8_t process_id_found;
        for (int process = 0; process < PROCESS_AMOUNT; process++)
        {
            fread(&status, 1, 1, memory);
            fread(&process_id_found, 1, 1, memory);
            if (process_id_found == process_id && status == 0x01)
            {
                fseek(memory,- 2, SEEK_CUR);
                status = 0x00;
                fwrite(&status, 1, 1, memory);
                break;
            }
            else
            {
                fseek(memory, PCB_SIZE - 2, SEEK_CUR);
            }
        }
    

        pcb_table[i] -> state = 0x00;
        uint8_t entry;
        uint8_t pfn;
        uint8_t validity = 0b10000000;
        uint8_t pfn_mask = 0b01111111;
        int byte_position;
        int bit_position;
        uint8_t frame_bitmap;

        fseek(memory, 4096, SEEK_SET);        
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
                frame_bitmap = frame_bitmap & !((int) pow(2,bit_position));
                fseek(memory, -1, SEEK_CUR);
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

CrmsFile* cr_open(int process_id, char* file_name, char mode)
{
    
    for (int process = 0; process < PROCESS_AMOUNT; process++)
    {
        if (pcb_table[process]->id == process_id && pcb_table[process]->state == 0x01)
        {

            switch(mode)
            {
                case 'r':;
                    for (int file = 0; file < PROCESS_FILES_AMOUNT; file++)
                    {
                        if (name_coincidence(pcb_table[process]->files[file]->name, file_name) && pcb_table[process]->files[file]->validity == 0x01)
                        {
                            //Crear el struct
                            pcb_table[process]->files[file]->mode = 'r';
                            return pcb_table[process]->files[file];
                        }
                    }
                    // Manejar error
                    break;

                case 'w':;
                    int virtual_addresses_used[PROCESS_FILES_AMOUNT];
                    int sized_used[PROCESS_FILES_AMOUNT];

                    int available_space = -1;
                    for (int file = 0; file < PROCESS_FILES_AMOUNT; file++)
                    {
                        if ((pcb_table[process]->files[file]->validity) == 0x01)
                        {
                            virtual_addresses_used[file] = (int) pcb_table[process]->files[file]->start_address;
                            sized_used[file] = (int) pcb_table[process]->files[file]->file_size;
                        }
                        else
                        {
                            if (available_space == -1)
                            {
                                available_space = file;
                            }
                            virtual_addresses_used[file] = -1;
                            sized_used[file] = -1;
                        }

                        if (name_coincidence(pcb_table[process]->files[file]->name, file_name) && pcb_table[process]->files[file]->validity == 0x01)
                        {
                            //¿Manejar el error
                        }
                    }

                    if (available_space == -1)
                    {
                        // manejar error falta de espacio (ya hay 10 archivos)
                    }

                    mergeSort(virtual_addresses_used, sized_used, 0, PROCESS_FILES_AMOUNT - 1);

                    int start_virtual_address = 0;
                    int finished_virtual_address = PROCESS_FILES_AMOUNT * PROCESS_FILE_MAX_SIZE;
                    
                    int found = 0;
                    for (int file=0; file < PROCESS_FILES_AMOUNT; file++)
                    {
                        if (virtual_addresses_used[file] != -1 && file < PROCESS_FILES_AMOUNT - 1)
                        {
                            if (virtual_addresses_used[file] + sized_used[file] < virtual_addresses_used[file + 1])
                            {
                                start_virtual_address = virtual_addresses_used[file] + sized_used[file];
                                finished_virtual_address = fmin(start_virtual_address + PROCESS_FILE_MAX_SIZE, virtual_addresses_used[file + 1]);
                                found = 1;
                                break;
                            }
                        }
                        else if (virtual_addresses_used[file] != -1 && file == PROCESS_FILES_AMOUNT - 1)
                        {
                            if (virtual_addresses_used[file] + sized_used[file] < finished_virtual_address)
                            {
                                start_virtual_address = virtual_addresses_used[file] + sized_used[file];
                                finished_virtual_address = fmin(start_virtual_address + PROCESS_FILE_MAX_SIZE, finished_virtual_address);
                                found = 1;
                                break;
                            }
                        }
                    }
                    if (found == 0)
                    {
                        finished_virtual_address = PROCESS_FILE_MAX_SIZE;
                    }

                    int file_size = finished_virtual_address - start_virtual_address;

                    pcb_table[process]->files[available_space]->validity = 0x01;
                    crmsfile_create(pcb_table[process]->files[available_space], file_name, (uint32_t) file_size, (uint32_t) start_virtual_address, (uint32_t) finished_virtual_address, 'w');
                    FILE* memory = fopen(file_direction, "rb+");
                    fseek(memory, PCB_SIZE * process + 14 + 21 * available_space, SEEK_SET);
                    uint8_t validity = pcb_table[process]->files[available_space]->validity;
                    //char name[FILE_NAME_SIZE] = ;
                    uint32_t size = 0;
                    uint32_t address = pcb_table[process]->files[available_space]->start_address;
                    fwrite(&validity, 1, 1, memory);
                    uint8_t aux;
                    for (int char_name = 0; char_name < FILE_NAME_SIZE; char_name++)
                    {
                        aux = (uint8_t) pcb_table[process]->files[available_space]->name[char_name];
                        fwrite(&aux, 1, 1, memory);
                    }
                    fwrite(&size, 4, 1, memory);
                    fwrite(&address, 4, 1, memory);
                    fclose(memory);
                    return pcb_table[process]->files[available_space];

                    break;
            }
        }
    }

    // Manejar error

}

//Funcion para escribir archivo
int cr_write_file(CrmsFile* file_desc, void* buffer, int n_bytes) 
{
    return 0;
}

//Funcion para leer archivo
int cr_read(CrmsFile* file_desc, void* buffer, int n_bytes)
{

    if (file_desc->mode != 'r')
    {
        // LLORAR
    }

    FILE* memory;
    memory = fopen(file_direction, "rb+");
    // en file_desc debría estar la dirección virtual del archivo 

    uint8_t status;
    uint8_t process_id;
    uint32_t virtual_dir = file_desc->start_address;
    uint32_t SEG_MASK = 0b00001111100000000000000000000000;
    uint32_t OFFSET_MASK = 0b00000000011111111111111111111111;
    int SEG_SHIFT = 23;
    int VPN;
    int page;
    uint8_t valid_mask = 128;
    uint8_t pfn_mask = 127;
    int valid_shift = 7;
    int valid;
    int PFN;
    int offset;
    int physicalAddress;
    int page_table;
    int bytes_read;
    n_bytes = fmin(n_bytes, (int) file_desc->file_size);
    uint8_t* buffer_aux = malloc(n_bytes * sizeof(uint8_t));
    for (int pointer = 0; pointer < PROCESS_AMOUNT; pointer++)
    {
        fread(&status, 1, 1, memory); // se lee el status del proceso
        fread(&process_id, 1, 1, memory); // se lee id de el proceso
        if (process_id == 27 && status == 0x01) // si el proceso es el mismo del archivo se busca la tabla de páginas
        {

            fseek(memory,210, SEEK_CUR); // pasamos las entradas de archivos
            page_table = ftell(memory); // guardamos la posición de la tabla de páginas del proceso
            // separamos la VPN de el offset en la dirección virtual
            VPN = (virtual_dir & SEG_MASK) >> SEG_SHIFT;
            offset = virtual_dir & OFFSET_MASK;

            
            fseek(memory ,VPN, SEEK_CUR); // nos movemos hasta la entrada número segment
            fread(&page, 1, 1, memory); // leemos la entrada 1bit de validez y 7 de PFN
            // se paramos el bit de validez del PFN
            valid = (page & valid_mask) >> valid_shift;
            PFN = page & pfn_mask;
            physicalAddress = PFN + offset; // obtenemos la dirección fisica como la dirección de la pagina + el offset
            // continuamos moviendonos hasta llegar al final de las tablas
        }
        else
        {
            fseek(memory, PCB_SIZE - 2, SEEK_CUR);
            
        }
    }
    fseek(memory, 16, SEEK_CUR); // nos saltamos el frame bit map
    fseek(memory, PFN*8388608, SEEK_CUR); // llegamos a la página designada por el pfn*8MB
    if (n_bytes+offset<=8388608) // si solo tenemos que leer una página
    {
        /* leer los n_bytes */
        fseek(memory, offset, SEEK_CUR); // avanzamos el offset
        fread(buffer_aux, n_bytes, 1, memory); //leemos los n bytes
        bytes_read = n_bytes;
        
    }else // si hay que leer más de una página
    {
        /*hay que leer hasta donde se pueda y seguir a la siguiente página*/
        bytes_read = 0;
        
        while (bytes_read<n_bytes) // mientras no terminemos de leer 
        {
            /*leemos lo que podamos*/
            if (bytes_read==0) // si es la primera página
            { 
                fread(buffer_aux, 8388608-offset, 1, memory);
                bytes_read = bytes_read + (8388608-offset);
            }else if ((n_bytes-bytes_read)>8388608) // es una página intermedia
            {
                fread(&(buffer_aux)[bytes_read], 8388608, 1, memory);
                bytes_read = bytes_read + 8 * 1024 * 1024;
            }else if ((n_bytes-bytes_read)<=8388608) // es la última página
            {
                fread(&(buffer_aux)[bytes_read], n_bytes-bytes_read, 1, memory);
                bytes_read = bytes_read + n_bytes-bytes_read;
            }
            
            if (bytes_read<n_bytes) // si aún no terminamos de leer
            {
                fseek(memory, page_table, SEEK_SET); //volvemos a la tabla de páginas
                VPN = VPN+1; // nos movemos a la siguiente página
                fseek(memory ,VPN, SEEK_CUR); // nos movemos hasta la entrada número segment
                fread(&page, 1, 1, memory); // leemos la entrada 1bit de validez y 7 de PFN

                // se paramos el bit de validez del PFN
                valid = (page & valid_mask) >> valid_shift;
                PFN = virtual_dir & OFFSET_MASK;
                physicalAddress = PFN + offset;

                fseek(memory, 16, SEEK_CUR); // nos saltamos el frame bit map
                fseek(memory, PFN*8388608, SEEK_CUR); // llegamos a la página designada por el pfn*8MB
            }
            
        }
    }
    fclose(memory);
    buffer = buffer_aux;
    return bytes_read;
    
    
    
}

//Funcion para borrar archivo
void cr_delete_file(CrmsFile* file_desc)
{
    
}

//Funcion para cerrar archivo
void cr_close(CrmsFile* file_desc)
{
    file_desc->mode = '-';
}