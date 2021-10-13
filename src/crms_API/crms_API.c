//FUNCIONES GENERALES
#include "crms_API.h"

const int FILE_NAME_SIZE = 12;
const int PROCESS_AMOUNT = 16;
const int NAME_SIZE = 12;
const int PROCESS_FILE_VIRTUAL_MEMORY = 4;
const int PROCESS_FILES_AMOUNT = 10;
const int PROCESS_FILE_SIZE = 4;
const int PCB_SIZE = 256;
const int PAGE_TABLE_ENTRIES = 32;
const int PROCESS_FILE_MAX_SIZE = 32 * 1024 * 1024;

PCB* pcb_table[16];
// https://stackoverflow.com/questions/48538425/c-modify-global-char-array
char file_direction[];

void name_assign(char* destination, char* origin)//Funcion auxiliar para asignar nombres
{

    for (int index = 0; index < NAME_SIZE + 1; index++)
    {
        destination[index] = '\0';
    }
    memmove(destination, origin, NAME_SIZE);
}

CrmsFile* crmsfile_init(PCB* pcb)
{
    CrmsFile* crmsfile = malloc(sizeof(CrmsFile));
    crmsfile -> validity = 0x00;
    crmsfile -> name = malloc((NAME_SIZE + 1) * sizeof(char));
    crmsfile -> file_size = 0;
    crmsfile -> mode = '-';
    crmsfile -> start_address = 0;
    crmsfile -> finished_address = 0;
    crmsfile -> pcb = pcb;
    return crmsfile;
}

void crmsfile_create(CrmsFile* crmsfile, char* name, uint32_t file_size, uint32_t start_address, uint32_t finished_address, char mode)
{
    crmsfile -> validity = 0x01;
    name_assign(crmsfile -> name, name);
    crmsfile -> file_size = file_size;
    crmsfile -> start_address = start_address;
    crmsfile -> finished_address = finished_address;
    crmsfile -> mode = mode;
}


PCB* pcb_init()
{
    PCB* pcb = malloc(sizeof(PCB));
    pcb -> state = 0x00;
    pcb -> id = 0;
    pcb -> name = malloc((NAME_SIZE + 1) * sizeof(char));
    pcb -> files = malloc(PROCESS_FILES_AMOUNT * sizeof(CrmsFile*));
    for (int file = 0; file < PROCESS_FILES_AMOUNT; file++)
    {
        pcb -> files[file] = crmsfile_init(pcb);
    }
    pcb -> page_table = page_table_init();
    return pcb;
}

void pcb_create(PCB* pcb, int state, int id, char* name)
{
    pcb -> state = state;
    pcb -> id = id;
    name_assign(pcb -> name, name);
}

void pcb_destroy(PCB* pcb)
{
    for (int file = 0; file < PROCESS_FILES_AMOUNT; file++)
    {
        crmsfile_destroy(pcb -> files[file]);
    }
    free(pcb->files);

    page_table_destroy(pcb -> page_table);

    free(pcb->name);
    free(pcb);
}


void crmsfile_destroy(CrmsFile* crmsfile)
{
    free(crmsfile -> name);
    free(crmsfile);
}
/*
void function()
{
    printf("Función -> file_direction = %s\n", file_direction);
}*/

void cr_mount(char* memory_path)
{
    /*char aux[14]; //Variable auxiliar: copiar el nombre de memoria a file_direction
    for (int i = 0; i < (int) strlen(memory_path); i++)
    {
        aux[i] = memory_path[i];
    }*/
    strcpy(file_direction, memory_path);


    FILE* memory;
    memory = fopen(file_direction, "rb");  // r for read, b for binary
    uint8_t status;
    uint8_t process_id_found;
    char* process_name = malloc(NAME_SIZE * sizeof(char));
    char* file_name = malloc(NAME_SIZE * sizeof(char)); // PROCESS_FILE_NAME
    for (int pcb = 0; pcb < PROCESS_AMOUNT; pcb++)
    {
        pcb_table[pcb] = pcb_init(); //Se inicializa vacío
        fread(&status, 1, 1, memory);
        fread(&process_id_found, 1, 1, memory);

        if (status == 0x01)
        {
            // printf("- %i\n", status);
            fread(process_name, 1, NAME_SIZE, memory);
            pcb_create(pcb_table[pcb], 0x01, process_id_found, process_name);

            uint8_t validation;
            uint32_t file_size;
            uint32_t virtual_memory;
            for (int file = 0; file < PROCESS_FILES_AMOUNT; file++)
            {
                fread(&validation, 1, 1, memory);
                fread(file_name, 1, NAME_SIZE, memory);

                fread(&file_size, PROCESS_FILE_SIZE, 1, memory);
                file_size = bswap_32(file_size);//Se da vuelta ya que leemos n bytes 1 vez

                fread(&virtual_memory, PROCESS_FILE_VIRTUAL_MEMORY, 1, memory);
                virtual_memory = bswap_32(virtual_memory);//Se da vuelta ya que leemos n bytes 1 vez

                if (validation == 0x01)
                {
                    uint32_t start_virtual_address = virtual_memory;
                    uint32_t finished_virtual_address = virtual_memory + file_size;
                    crmsfile_create(pcb_table[pcb]->files[file], file_name, file_size, start_virtual_address, finished_virtual_address, '-');
                    //printf("Process: [%i] File name: [%s] FILE SIZE: %i\n", pcb_table[pcb] -> id, pcb_table[pcb] -> files[file]->name, pcb_table[pcb]->files[file]->file_size);
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
            /*for (int character = 0; character < NAME_SIZE; character++)
            {
                printf("%c", (char) ((int) pcb_table[i]->name[character]));
            }*/
            printf("%s", pcb_table[i]->name);
            printf("\n");
        } 
    }
}

int name_coincidence(char* file_name_found, char* file_name)
{
    if (strcmp(file_name_found, file_name) == 0)
    {
        return 1;
    }

    else
    {
        return 0;
    }

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
    
    //ERROR -> Levantar un error porque el proceso no existe
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
                    /*for (int character = 0; character < (int) strlen(pcb_table[i]->files[j]->name); character++)
                    {
                        printf("%c", (char) ((int) pcb_table[i]->files[j]->name[character]));
                    }*/
                    printf("%s", pcb_table[i]->files[j]->name);
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
            // ERROR Levantar error
            printf("El proceso que quieres iniciar ya está iniciado uwu WARNING!!!!!!!!!!!!\n");
            return;
        }
    }
    int process_created = 0; //Para saber si ya podemos crearlo
    for (int index = 0; index < PROCESS_AMOUNT; index++)
    {
        if (pcb_table[index]->state == 0x00 && process_created == 0) //Si no esta creado, y se puede crear en este espacio
        {
            process_created = 1;

            pcb_create(pcb_table[index], 0x01, process_id, process_name); //Creamos el proceso en tabla pcb
            FILE* memory = fopen(file_direction, "rb+");

            fseek(memory, PCB_SIZE * index, SEEK_SET);
            fwrite(&(pcb_table[index]->state), 1, 1, memory);//Escribimos en memoria física
            fwrite(&(pcb_table[index]->id), 1, 1, memory); 
            for (int char_name = 0; char_name < NAME_SIZE; char_name++)
            {
                fwrite(&(pcb_table[index]->name[char_name]), 1, 1, memory);
            }
            uint8_t aux = 0x00;
            fwrite(&aux, 1, 242, memory);//Llenamos los 242 bytes de 0x00

            fclose(memory);

            break;
        }
    }

    if (process_created == 0)
    {
        // ERROR Levantar error, no cabe otro proceso
        printf("Borrar este print");
    }
}

void cr_finish_process(int process_id)
{
    int i;
    for (i = 0; i < PROCESS_AMOUNT; i++)
    {
        if (pcb_table[i] -> state == 0x01 && pcb_table[i] -> id == process_id)
        {

            pcb_table[i] -> state = 0x00;

            //TERMINAR PROCESO Y LIBERAR MEMORIA
            FILE* memory = fopen(file_direction, "rb+");
            fseek(memory, i * PCB_SIZE, SEEK_SET);
            uint8_t status = 0x00;
            fwrite(&status, 1, 1, memory);

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
            break;
        }
    }
    if (i==16)
    {
        //ERROR
        printf("No existe un proceso con ese ID");
    }
}

void virtual_address_calculator(CrmsFile* file_desc, int file_size) //Calculamos la dirección virtual
{
    PCB* pcb = file_desc -> pcb;

    int virtual_addresses_used[PROCESS_FILES_AMOUNT];
    int sized_used[PROCESS_FILES_AMOUNT];

    for (int file = 0; file < PROCESS_FILES_AMOUNT; file++)
    {
        if ((pcb->files[file]->validity) == 0x01)
        {
            virtual_addresses_used[file] = (int) pcb->files[file]->start_address;
            sized_used[file] = (int) pcb->files[file]->file_size;
        }
        else
        {
            virtual_addresses_used[file] = -1;
            sized_used[file] = -1;
        }
    }

    mergeSort(virtual_addresses_used, sized_used, 0, PROCESS_FILES_AMOUNT - 1);

    int start_virtual_address = 0;
    int finished_virtual_address = PROCESS_FILES_AMOUNT * PROCESS_FILE_MAX_SIZE;
    
    int found = 0;
    for (int file=0; file < PROCESS_FILES_AMOUNT; file++)
    {
        if (virtual_addresses_used[file] != -1)
        {
            if (start_virtual_address < virtual_addresses_used[file])
            {
                finished_virtual_address = virtual_addresses_used[file];
                found = 1;
                break;
            }
            else
            {
                start_virtual_address = virtual_addresses_used[file] + sized_used[file];
            }
        }

    }
    if (found == 0)
    {
        finished_virtual_address = fmin(start_virtual_address + file_size, PROCESS_FILES_AMOUNT * PROCESS_FILE_MAX_SIZE);
    }

    file_desc -> start_address = start_virtual_address;
    file_desc -> finished_address = finished_virtual_address;

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
                        if (pcb_table[process]->files[file]->validity == 0x01 && name_coincidence(pcb_table[process]->files[file]->name, file_name))
                        {

                            if (pcb_table[process]->files[file]->mode != '-')
                            {
                                //ERROR, el archivo ya está abierto en algún modo
                            }
                            else
                            {
                                pcb_table[process]->files[file]->mode = 'r';
                                return pcb_table[process]->files[file];   
                            }
                        }
                    }
                    // ERROR El archivo no existe
                    break;
                    
                case 'w':;
                    
                    int available_space = -1;
                    for (int file = 0; file < PROCESS_FILES_AMOUNT; file++)
                    {
                        if (pcb_table[process]->files[file]->validity == 0x01 && name_coincidence(pcb_table[process]->files[file]->name, file_name))
                        {
                            // ERROR, el archivo ya está abierto en algún modo
                            /* CR_ERROR = 6;
                            cr_sterror(6); */
                            return NULL; // OJO
                        }
                        else if (pcb_table[process]->files[file]->validity == 0x00 && available_space == -1)
                        {
                            available_space = file;
                        }
                    }

                    if (available_space == -1)
                    {
                        // ERROR ya hay 10 archivos
                    }

                                       
                    crmsfile_create(pcb_table[process]->files[available_space], file_name, (uint32_t) 0, (uint32_t) 0, (uint32_t) 0, 'w');

                    FILE* memory = fopen(file_direction, "rb+");
                    fseek(memory, PCB_SIZE * process + 14 + 21 * available_space, SEEK_SET); // 14 -> info pcb, 21 -> info archivo
                    uint8_t validity = pcb_table[process]->files[available_space]->validity;
                    uint32_t size = 0;
                    uint32_t address = 0;
                    fwrite(&validity, 1, 1, memory);
                    uint8_t char_aux;
                    for (int char_name = 0; char_name < FILE_NAME_SIZE; char_name++)
                    {
                        char_aux = (uint8_t) pcb_table[process]->files[available_space]->name[char_name];
                        fwrite(&char_aux, 1, 1, memory);
                    }
                    fwrite(&size, 4, 1, memory);
                    fwrite(&address, 4, 1, memory);
                    fclose(memory);

                    return pcb_table[process]->files[available_space];

                    break;
            }
        }
    }
    // ERROR El proceso no existe

}

int available_frame()
{
    uint8_t frame_bitmap[16];
    FILE* memory = fopen(file_direction, "rb+");
    fseek(memory, PCB_SIZE * 16, SEEK_SET);
    fread(frame_bitmap, 1, 16, memory);
    int available = 0b10000000;
    uint8_t byte_analize;
    for (int byte = 0; byte < 16; byte++)
    {
        byte_analize = frame_bitmap[byte];
        for (int bit = 0; bit < 8; bit++)
        {
            if ((byte_analize & available) == 0b00000000)
            {
                uint8_t to_modify = 0b10000000 >> bit;
                byte_analize = frame_bitmap[byte] | to_modify;
                fclose(memory);
                return byte * 8 + bit;
            }
            else
            {
                byte_analize = byte_analize << 1;
            }
        }
    }
    fclose(memory);
    return -1;
}

//Funcion para escribir archivo
int cr_write_file(CrmsFile* file_desc, void* buffer, int n_bytes) 
{
    virtual_address_calculator(file_desc, n_bytes);
    uint32_t virtual_dir = file_desc->start_address;

    uint32_t SEG_MASK = 0b00001111100000000000000000000000;
    uint32_t OFFSET_MASK = 0b00000000011111111111111111111111;
    int SEG_SHIFT = 23;
    int VPN;
    uint8_t page;

    uint8_t valid_mask = 0b10000000;//128   
    uint8_t pfn_mask = 0b01111111;//127
    int valid_shift = 7;
    int valid;
    int PFN;

    int offset;

    n_bytes = fmin(n_bytes, (file_desc->finished_address - file_desc->start_address));
    FILE* memory = fopen(file_direction, "rb+");
    int bytes_written = 0;
    while (bytes_written < n_bytes)
    {
        VPN = (virtual_dir & SEG_MASK) >> SEG_SHIFT;
        offset = virtual_dir & OFFSET_MASK;

        page = file_desc -> pcb -> page_table -> entries [VPN];
        // se paramos el bit de validez del PFN
        valid = (page & valid_mask) >> valid_shift;

        if (valid == 0)
        {
            PFN = available_frame();
            if (PFN == -1)
            {
                file_desc -> file_size = bytes_written; // OJO esto implica que solo se puede escribir 1 vez
                fclose(memory);
                return bytes_written;
            }
            else
            {
                file_desc -> pcb -> page_table -> entries [VPN] = 0b10000000 + PFN;
            }

        }
        else
        {
            PFN = page & pfn_mask;
        }

        fseek(memory, PCB_SIZE * 16 + 16 + PFN * (8 * 1024 * 1024) + offset, SEEK_SET);
        if ((8 * 1024 * 1024) - offset >= (n_bytes - bytes_written))
        {
            fwrite(&(((uint8_t*) buffer)[bytes_written]), 1, (n_bytes - bytes_written), memory);
            bytes_written = bytes_written + (n_bytes - bytes_written);
            file_desc -> file_size = bytes_written; // OJO esto implica que solo se puede escribir 1 vez
            fclose(memory);
            return bytes_written;
        }
        else
        {
            fwrite(&(((uint8_t*) buffer)[bytes_written]), 1, (8 * 1024 * 1024) - offset, memory);
            bytes_written = bytes_written + (8 * 1024 * 1024) - offset;
            virtual_dir = virtual_dir + bytes_written;
        }


    }

    // ERROR n_bytes invalido
    return 0;
}


int cr_read(CrmsFile* file_desc, void* buffer, int n_bytes)
{

    if (file_desc->mode != 'r')
    {
        // LLORAR
    }

    // en file_desc debría estar la dirección virtual del archivo 

    uint32_t virtual_dir = file_desc->start_address;

    uint32_t SEG_MASK = 0b00001111100000000000000000000000;
    uint32_t OFFSET_MASK = 0b00000000011111111111111111111111;
    int SEG_SHIFT = 23;
    int VPN;
    uint8_t page;

    uint8_t valid_mask = 0b10000000;//128   
    uint8_t pfn_mask = 0b01111111;//127
    int valid_shift = 7;
    int valid;
    int PFN;

    int offset;

    int bytes_read;
    n_bytes = fmin(n_bytes, (int) file_desc->file_size);

    //uint8_t* buffer_aux = malloc(n_bytes * sizeof(uint8_t));
    
    
    // separamos la VPN de el offset en la dirección virtual
    VPN = (virtual_dir & SEG_MASK) >> SEG_SHIFT;
    offset = virtual_dir & OFFSET_MASK;

    page = file_desc -> pcb -> page_table -> entries [VPN];
    // se paramos el bit de validez del PFN
    valid = (page & valid_mask) >> valid_shift;

    if (valid == 0)
    {
        // ERROR, pagina de direccion virtual no tiene frame asignado
        printf("ERROR, pagina de direccion virtual no tiene frame asignado");
        return;
    }

    PFN = page & pfn_mask;
    // OJO physical address not used
    //physicalAddress = PFN + offset; // obtenemos la dirección fisica como la dirección de la pagina + el offset
    // continuamos moviendonos hasta llegar al final de las tablas
    

    FILE* memory;
    memory = fopen(file_direction, "rb+");
    fseek(memory, PCB_SIZE * 16 + 16 + PFN * (8 * 1024 * 1024), SEEK_SET); // llegamos a la página designada por el pfn*8MB
    if (n_bytes + offset <= (8 * 1024 * 1024)) // si solo tenemos que leer una página
    {
        /* leer los n_bytes */
        fseek(memory, offset, SEEK_CUR); // avanzamos el offset
        //fread(buffer_aux, n_bytes, 1, memory); //leemos los n bytes
        fread((uint8_t*) buffer, n_bytes, 1, memory); //leemos los n bytes
        bytes_read = n_bytes;
        
    }
    else // si hay que leer más de una página
    {
        /*hay que leer hasta donde se pueda y seguir a la siguiente página/frame */
        bytes_read = 0;
        
        while (bytes_read < n_bytes) // mientras no terminemos de leer 
        {
            /*leemos lo que podamos*/
            if (bytes_read == 0) // si es el primer frame
            { 
                //fread(buffer_aux, (8 * 1024 * 1024) - offset, 1, memory);
                fread((uint8_t*) buffer, 1, (8 * 1024 * 1024) - offset, memory);
                bytes_read = bytes_read + ((8 * 1024 * 1024) - offset);

            }
            else if ( (n_bytes-bytes_read) > (8 * 1024 * 1024)) // es una página intermedia
            {
                //fread(&(buffer_aux[bytes_read]), (8 * 1024 * 1024), 1, memory);
                fread(&(((uint8_t*) buffer)[bytes_read]), 1, (8 * 1024 * 1024), memory);
                bytes_read = bytes_read + 8 * 1024 * 1024;
            }
            else if ((n_bytes-bytes_read) <= (8 * 1024 * 1024)) // es la última página
            {
                //fread(&(buffer_aux[bytes_read]), (n_bytes - bytes_read), 1, memory);
                fread(&(((uint8_t*) buffer)[bytes_read]), 1, (n_bytes - bytes_read), memory);
                bytes_read = bytes_read + (n_bytes - bytes_read);
            }
            
            if (bytes_read < n_bytes) // si aún no terminamos de leer
            {

                VPN = VPN + 1; // nos movemos a la siguiente página
                page = file_desc -> pcb -> page_table -> entries [VPN];
                // se paramos el bit de validez del PFN
                valid = (page & valid_mask) >> valid_shift;

                if (valid == 0)
                {
                    // ERROR, pagina de direccion virtual no tiene frame asignado
                    printf("NUNCA DEBERIAMOS LLEGAR AQUI");
                    printf("ERROR, pagina de direccion virtual no tiene frame asignado");
                    return;
                }

                PFN = page & pfn_mask;

                fseek(memory, PCB_SIZE * 16 + 16 + PFN * (8 * 1024 * 1024), SEEK_SET); // llegamos a la página designada por el pfn*8MB
            }
            
        }
    }
    fclose(memory);
    //buffer = buffer_aux;
    return bytes_read;

}



//Funcion para borrar archivo
void cr_delete_file(CrmsFile* file_desc)
{
    
}

//Funcion para cerrar archivo
void cr_close(CrmsFile* file_desc)
{
    // escribir en memfilled.bin el nuevo tamaño y direccion del archivo si corresponde
    if (file_desc -> mode == 'r')
    {
        file_desc->mode = '-';
    }
    else if (file_desc -> mode == 'w')
    {
        FILE* memory = fopen(file_direction, "rb+");

        uint8_t status;
        uint8_t process_id_found;
        char* file_name = malloc(NAME_SIZE * sizeof(char)); // PROCESS_FILE_NAME

        int frame_used[PAGE_TABLE_ENTRIES];

        for (int pcb = 0; pcb < PROCESS_AMOUNT; pcb++)
        {
            fread(&status, 1, 1, memory);
            fread(&process_id_found, 1, 1, memory);

            if (status == 0x01 && process_id_found == file_desc -> pcb -> id)
            {
                fseek(memory, NAME_SIZE, SEEK_CUR);
                uint8_t validation;
                uint32_t file_size;
                uint32_t virtual_memory;
                for (int file = 0; file < PROCESS_FILES_AMOUNT; file++)
                {
                    fread(&validation, 1, 1, memory);
                    fread(file_name, 1, NAME_SIZE, memory);

                    fread(&file_size, PROCESS_FILE_SIZE, 1, memory);
                    file_size = bswap_32(file_size);//Se da vuelta ya que leemos n bytes 1 vez

                    fread(&virtual_memory, PROCESS_FILE_VIRTUAL_MEMORY, 1, memory);
                    virtual_memory = bswap_32(virtual_memory);//Se da vuelta ya que leemos n bytes 1 vez

                    if (validation == 0x01 && name_coincidence(file_name, file_desc -> name))
                    {
                        fseek(memory, - PROCESS_FILE_VIRTUAL_MEMORY - PROCESS_FILE_SIZE, SEEK_CUR);
                        uint32_t aux1 = bswap_32(file_desc -> file_size);
                        uint32_t aux2 = bswap_32(file_desc -> start_address);
                        fwrite(&aux1, PROCESS_FILE_SIZE, 1, memory);
                        fwrite(&aux2, PROCESS_FILE_VIRTUAL_MEMORY, 1, memory);
                    }
                }

                for (int entrie = 0; entrie < PAGE_TABLE_ENTRIES; entrie++)
                {
                    uint8_t byte_entrie = file_desc -> pcb -> page_table -> entries[entrie];
                    fwrite(&byte_entrie, 1, 1, memory);
                    if ((byte_entrie & 0b10000000) == 0b10000000)
                    {
                        frame_used[entrie] = byte_entrie & 0b01111111;
                    }
                    else
                    {
                        frame_used[entrie] = 0b11111111;
                    }
                }
                

            }
            else
            {
                fseek(memory, PCB_SIZE - 2, SEEK_CUR);
            }
        }
        free(file_name);

        for (int entrie = 0; entrie < PAGE_TABLE_ENTRIES; entrie++)
        {
            fseek(memory, PROCESS_AMOUNT * PCB_SIZE, SEEK_SET);
            if (frame_used[entrie] < 256)
            {
                fseek(memory, (int) frame_used[entrie] / 8, SEEK_CUR);
                uint8_t to_replace;
                fread(&to_replace, 1, 1, memory);
                fseek(memory, -1, SEEK_CUR);
                to_replace = to_replace | (0b10000000 >> (frame_used[entrie] % 8));
                fwrite(&to_replace, 1, 1, memory);
            }
        }

        fclose(memory);
    }
    else
    {
        // Error
    }
    
}

void cr_unmount()
{
    for (int pcb = 0; pcb < 16; pcb++)
    {
        pcb_destroy(pcb_table[pcb]);
    }
}