#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../crms_API/crms_API.h"
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>



int main(int argc, char **argv)
{

    if (argc < 2)
    {
        printf("Al menos un argumento debe ser entregado\n");
        // ERROR
        return 0;
    }

    cr_mount("memfilled.bin");

    int instruction = atoi(argv[1]);

    if (instruction == 1) //listar procesos ./crms 1 
    {
        printf("cr_ls_process()\n");
        cr_ls_process();
    }

    else if (instruction == 2) //recibe 4 arg crms 2 proceso_id nombre
    {

        if (argc != 4)
        {
            printf("./crms 2 [process_id] [file_name]\n");
            cr_unmount();
            // ERROR
            return 0;
        }

        int process_id = atoi(argv[2]);
        char file_name[256];
        strcpy(file_name, argv[3]);
        int result = cr_exists(process_id, file_name);
        printf("cr_exists(%i, %s) = %i\n", process_id, file_name, result);
    }

    else if (instruction == 3) // Lista de archivos del proceso ./crms 3 proceso_id 
    {

        if (argc != 3)
        {
            printf("./crms 3 [process_id]\n");
            cr_unmount();
            // ERROR
            return 0;
        }

        int process_id = atoi(argv[2]);
        printf("cr_ls_files(%i)\n", process_id);
        cr_ls_files(process_id);
    }

    else if (instruction == 4) // incia procesocrms 4 proceso_id nombre
    {

        if (argc != 4)
        {
            printf("./crms 4 [process_id] [process_name]\n");
            cr_unmount();
            // ERROR
            return 0;
        }

        int process_id = atoi(argv[2]);
        char process_name[256];
        strcpy(process_name, argv[3]);
        cr_start_process(process_id, process_name);
        //cr_ls_process();
    }

    else if (instruction == 5) // Finaliza proceso ./crms 5 proceso id
    {

        if (argc != 3)
        {
            printf("./crms 5 [process_id]\n");
            cr_unmount();
            // ERROR
            return 0;
        }

        int process_id = atoi(argv[2]);
        cr_finish_process(process_id);
        //cr_ls_process();
    }


    else if (instruction == 6) // en consola abre archivo ./crms 6 proceso_id nombre bytes_decimal
    {

        if (argc != 5)
        {
            printf("./crms 6 [process_id] [file_name] [bytes_to_read]\n");
            cr_unmount();
            // ERROR
            return 0;
        }

        int process_id = atoi(argv[2]);
        char file_name[12];
        strcpy(file_name, argv[3]);
        int bytes_to_read = atoi(argv[4]);

        uint8_t* buffer = malloc(bytes_to_read * sizeof(uint8_t));
        CrmsFile* file = cr_open(process_id, file_name, 'r');
        int bytes_read = cr_read(file, buffer, bytes_to_read);
        cr_close(file);

        printf("Esto es bytes_read %i\n", bytes_read);
        for(int i = 0; i < bytes_read; i++)
        {
            printf("%c", (char) ((uint8_t*) buffer)[i]);
        }
        printf("\n");

        free(buffer);
    }

    else if (instruction == 7) // lee archivos ./crms 7 proceso_id 
    {

        if (argc != 5)
        {
            printf("./crms 7 [process_id] [file_name] [bytes_to_read]\n");
            cr_unmount();
            // ERROR
            return 0;
        }

        int process_id = atoi(argv[2]);
        char file_name[12];
        strcpy(file_name, argv[3]);
        int bytes_to_read = atoi(argv[4]);

        uint8_t* buffer = malloc(bytes_to_read * sizeof(uint8_t));
        CrmsFile* file = cr_open(process_id, file_name, 'r');
        if (file != NULL)
            { 
            int bytes_read = cr_read(file, buffer, bytes_to_read);
            cr_close(file);

            printf("Esto es bytes_read %i\n", bytes_read);
            
            FILE* new = fopen(file_name, "wb+");
            fwrite(buffer, 1, bytes_read, new);
            fclose(new);
        }
        free(buffer);
    }

    else if (instruction == 8) // escribe un archivo
    {

        if (argc != 5)
        {
            printf("./crms 8 [process_id] [file_name] [bytes_to_write]\n");
            cr_unmount();
            // ERROR
            return 0;
        }

        int process_id = atoi(argv[2]);
        char file_name[12];
        strcpy(file_name, argv[3]);
        int bytes_to_write = atoi(argv[4]);

        uint8_t* buffer = malloc(bytes_to_write * sizeof(uint8_t));
        FILE* old = fopen(file_name, "rb");
        bytes_to_write = fread(buffer, 1, bytes_to_write, old);
        fclose(old);

        CrmsFile* file = cr_open(process_id, file_name, 'w');
        if (file != NULL)
        { 
            int bytes_written = cr_write_file(file, buffer, bytes_to_write);
            cr_close(file);
            printf("Esto es bytes_read %i\n", bytes_written);
        }
        free(buffer);

    }

else if (instruction == 9) //borrar 
    {

        if (argc != 4)
        {
            printf("./crms 9 [process_id] [file_name]\n");
            cr_unmount();
            // ERROR
            return 0;
        }

        int process_id = atoi(argv[2]);
        char file_name[12];
        strcpy(file_name, argv[3]);

        CrmsFile* file = cr_open(process_id, file_name, 'r');
        if (file != NULL)
        {
            cr_delete_file(file);
        }
    }

    cr_unmount();

}

