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
    cr_mount("memfilled.bin");

    int instruction = atoi(argv[1]);

    if (instruction == 1)
    {
        printf("cr_ls_process()\n");
        cr_ls_process();
    }

    else if (instruction == 2)
    {
        int process_id = atoi(argv[2]);
        printf("cr_ls_files(%i)\n", process_id);
        cr_ls_files(process_id);
    }

    else if (instruction == 3)
    {
        //cr_exists(27, "nani");
        int process_id = atoi(argv[2]);
        char file_name[256];
        strcpy(file_name, argv[3]);
        int result = cr_exists(process_id, file_name);
        printf("cr_exists(%i, %s) = %i\n", process_id, file_name, result);
    }

    else if (instruction == 4) //txt en consola
    {
        int process_id = atoi(argv[2]);
        char name[12];
        strcpy(name, argv[3]);
        int bytes_to_read = atoi(argv[4]);

        uint8_t* buffer = malloc(bytes_to_read * sizeof(uint8_t));
        int bytes_read = cr_read(cr_open(process_id, name, 'r'), buffer, bytes_to_read);

        printf("Esto es bytes_read %i\n", bytes_read);
        for(int i = 0; i < bytes_read; i++)
        {
            printf("%c", (char) ((uint8_t*) buffer)[i]);
        }
        printf("\n");

        free(buffer);
    }

    else if (instruction == 5) //jpg
    {
        int process_id = atoi(argv[2]);
        char name[12];
        strcpy(name, argv[3]);
        int bytes_to_read = atoi(argv[4]);

        uint8_t* buffer = malloc(bytes_to_read * sizeof(uint8_t));
        int bytes_read = cr_read(cr_open(process_id, name, 'r'), buffer, bytes_to_read);

        printf("Esto es bytes_read %i\n", bytes_read);
        
        FILE* new = fopen("new.jpg", "wb+");
        fwrite(buffer, 1, bytes_read, new);
        fclose(new);

        free(buffer);

    }

    else if (instruction == 6) //wav
    {
        int process_id = atoi(argv[2]);
        char name[12];
        strcpy(name, argv[3]);
        int bytes_to_read = atoi(argv[4]);

        uint8_t* buffer = malloc(bytes_to_read * sizeof(uint8_t));
        int bytes_read = cr_read(cr_open(process_id, name, 'r'), buffer, bytes_to_read);

        printf("Esto es bytes_read %i\n", bytes_read);
        
        FILE* new = fopen("new.wav", "wb+");
        fwrite(buffer, 1, bytes_read, new);
        fclose(new);

        free(buffer);

    }

    else if (instruction == 7) //mp4
    {
        int process_id = atoi(argv[2]);
        char name[12];
        strcpy(name, argv[3]);
        int bytes_to_read = atoi(argv[4]);

        uint8_t* buffer = malloc(bytes_to_read * sizeof(uint8_t));
        int bytes_read = cr_read(cr_open(process_id, name, 'r'), buffer, bytes_to_read);

        printf("Esto es bytes_read %i\n", bytes_read);
        
        FILE* new = fopen("new.mp4", "wb+");
        fwrite(buffer, 1, bytes_read, new);
        fclose(new);

        free(buffer);

    }

    else if (instruction == 8) //mkv
    {
        int process_id = atoi(argv[2]);
        char name[12];
        strcpy(name, argv[3]);
        int bytes_to_read = atoi(argv[4]);

        uint8_t* buffer = malloc(bytes_to_read * sizeof(uint8_t));
        int bytes_read = cr_read(cr_open(process_id, name, 'r'), buffer, bytes_to_read);

        printf("Esto es bytes_read %i\n", bytes_read);
        
        FILE* new = fopen("new.mkv", "wb+");
        fwrite(buffer, 1, bytes_read, new);
        fclose(new);

        free(buffer);

    }

    else if (instruction == 9) //txt
    {
        int process_id = atoi(argv[2]);
        char name[12];
        strcpy(name, argv[3]);
        int bytes_to_read = atoi(argv[4]);

        uint8_t* buffer = malloc(bytes_to_read * sizeof(uint8_t));
        int bytes_read = cr_read(cr_open(process_id, name, 'r'), buffer, bytes_to_read);

        printf("Esto es bytes_read %i\n", bytes_read);
        
        FILE* new = fopen("new.txt", "wb+");
        fwrite(buffer, 1, bytes_read, new);
        fclose(new);

        free(buffer);

    }

    else if (instruction == 10) //png
    {
        int process_id = atoi(argv[2]);
        char name[12];
        strcpy(name, argv[3]);
        int bytes_to_read = atoi(argv[4]);

        uint8_t* buffer = malloc(bytes_to_read * sizeof(uint8_t));
        int bytes_read = cr_read(cr_open(process_id, name, 'r'), buffer, bytes_to_read);

        printf("Esto es bytes_read %i\n", bytes_read);
        
        FILE* new = fopen("new.png", "wb+");
        fwrite(buffer, 1, bytes_read, new);
        fclose(new);

        free(buffer);

    }

    else if (instruction == 11) //bin
    {
        int process_id = atoi(argv[2]);
        char name[12];
        strcpy(name, argv[3]);
        int bytes_to_read = atoi(argv[4]);

        uint8_t* buffer = malloc(bytes_to_read * sizeof(uint8_t));
        int bytes_read = cr_read(cr_open(process_id, name, 'r'), buffer, bytes_to_read);

        printf("Esto es bytes_read %i\n", bytes_read);
        
        FILE* new = fopen("new.bin", "wb+");
        fwrite(buffer, 1, bytes_read, new);
        fclose(new);

        free(buffer);

    }

    else if (instruction == 12)
    {
        //cr_start_process(26, "Raulitoteamo");
        int process_id = atoi(argv[2]);
        char name[256];
        strcpy(name, argv[3]);
        cr_start_process(process_id, name);
        cr_ls_process();
    }

    //cr_ls_files(27);
    //cr_finish_process(25);
    //cr_open(27, "billetedeluk", 'w');
    //cr_ls_files(27);
    
    //amogus.mp4
    //grub.mp4
    //drums.mp4
    //knowledg.jpg
    //nightcal.mp4
    //popcorn.mkv
}

