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

    if (argc < 3)
    {
        printf("Al menos un argumento debe ser entregado\n");
        // ERROR
        return 0;
    }

    // Se monta la memoria
    printf("cr_mount(%s)\n\n", argv[1]);
    cr_mount(argv[1]);

    int instruction = atoi(argv[2]);

    if (instruction == 1) //listar procesos ./crms 1 
    {
        printf("cr_ls_process()\n");
        cr_ls_process();
    }

    else if (instruction == 2) // Indica si existe archivo en proceso recibe 5 arg crms bin_file 2 proceso_id nombre
    {

        if (argc != 5)
        {
            printf("./crms 2 [bin_file] [process_id] [file_name]\n");
            cr_unmount();
            // ERROR
            return 0;
        }

        int process_id = atoi(argv[3]);
        char file_name[256];
        strcpy(file_name, argv[4]);
        int result = cr_exists(process_id, file_name);
        printf("cr_exists(%i, %s) = %i\n", process_id, file_name, result);
    }

    else if (instruction == 3) // Lista de archivos del proceso ./crms bin_file 3 proceso_id 
    {

        if (argc != 4)
        {
            printf("./crms [bin_file] 3 [process_id]\n");
            cr_unmount();
            // ERROR
            return 0;
        }

        int process_id = atoi(argv[3]);
        printf("cr_ls_files(%i)\n", process_id);
        cr_ls_files(process_id);
    }

    else if (instruction == 4) // incia proceso crms bin_file 4 proceso_id nombre
    {

        if (argc != 5)
        {
            printf("./crms [bin_file] 4 [process_id] [process_name]\n");
            cr_unmount();
            // ERROR
            return 0;
        }

        int process_id = atoi(argv[3]);
        char process_name[256];
        strcpy(process_name, argv[4]);
        cr_start_process(process_id, process_name);
        //cr_ls_process();
    }

    else if (instruction == 5) // Finaliza proceso ./crms bin_file 5 proceso id
    {

        if (argc != 4)
        {
            printf("./crms [bin_file] 5 [process_id]\n");
            cr_unmount();
            // ERROR
            return 0;
        }

        int process_id = atoi(argv[3]);
        cr_finish_process(process_id);
        //cr_ls_process();
    }


    else if (instruction == 6) // lee archivos y los escribe en el pc ./crms bin_file 6 proceso_id file_name bytes_to_read
    {

        if (argc != 6)
        {
            printf("./crms [bin_file] 6 [process_id] [file_name] [bytes_to_read]\n");
            cr_unmount();
            // ERROR
            return 0;
        }

        int process_id = atoi(argv[3]);
        char file_name[12];
        strcpy(file_name, argv[4]);
        int bytes_to_read = atoi(argv[5]);

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

    else if (instruction == 7) // escribe un archivo en la memoria entregada ./crms bin_file 7 proceso_id file_name bytes_to_write
    {

        if (argc != 6)
        {
            printf("./crms [bin_file] 7 [process_id] [file_name] [bytes_to_write]\n");
            cr_unmount();
            // ERROR
            return 0;
        }

        int process_id = atoi(argv[3]);
        char file_name[12];
        strcpy(file_name, argv[4]);
        int bytes_to_write = atoi(argv[5]);

        uint8_t* buffer = malloc(bytes_to_write * sizeof(uint8_t));
        FILE* old = fopen(file_name, "rb");
        bytes_to_write = fread(buffer, 1, bytes_to_write, old);
        fclose(old);

        CrmsFile* file = cr_open(process_id, file_name, 'w');
        if (file != NULL)
        { 
            int bytes_written = cr_write_file(file, buffer, bytes_to_write);
            cr_close(file);
            printf("Esto es bytes_write %i\n", bytes_written);
        }
        free(buffer);

    }

    else if (instruction == 8) // borrar un archivo ./crms bin_file 8 process_id file_name
    {

        if (argc != 5)
        {
            printf("./crms [bin_file] 8 [process_id] [file_name]\n");
            cr_unmount();
            // ERROR
            return 0;
        }

        int process_id = atoi(argv[3]);
        char file_name[12];
        strcpy(file_name, argv[4]);

        CrmsFile* file = cr_open(process_id, file_name, 'r');
        if (file != NULL)
        {
            cr_delete_file(file);
        }
    }

    else if (instruction == 9) //Se escriba un archivo que ya esté abierto en modo específico
    {

        if (argc != 5)
        {
            printf("./crms [bin_file] 9 [process_id] [file_name]\n");
            cr_unmount();
            // ERROR
            return 0;
        }

        int process_id = atoi(argv[3]);
        char file_name[12];
        strcpy(file_name, argv[4]);

        cr_open(process_id, file_name, 'w');
        cr_open(process_id, file_name, 'w');

        cr_unmount();
        return 0;

    }

    else if (instruction == 10) // lee un archivos que esta cerrado
    {

        if (argc != 6)
        {
            printf("./crms [bin_file] 6 [process_id] [file_name] [bytes_to_read]\n");
            cr_unmount();
            // ERROR
            return 0;
        }

        int process_id = atoi(argv[3]);
        char file_name[12];
        strcpy(file_name, argv[4]);
        int bytes_to_read = atoi(argv[5]);

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

    else if (instruction == 999) // en consola abre archivo ./crms bin_file 999 proceso_id nombre bytes_decimal
    {

        if (argc != 6)
        {
            printf("./crms [bin_file] 6 [process_id] [file_name] [bytes_to_read]\n");
            cr_unmount();
            // ERROR
            return 0;
        }

        int process_id = atoi(argv[3]);
        char file_name[12];
        strcpy(file_name, argv[4]);
        int bytes_to_read = atoi(argv[5]);

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

    cr_unmount();

}

