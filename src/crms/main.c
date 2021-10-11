#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../crms_API/crms_API.h"
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char **argv)
{
    cr_mount("memfilled.bin");
    // char* file = *file_direction;
    //printf("%s", *file_direction);
    //function();
    //cr_ls_files(27);
    //cr_exists(27, "nani");
    //cr_ls_files(27);
    //cr_start_process(26, "Raulitoteamo");
    //cr_finish_process(27);
    //cr_open(27, "billetedeluk", 'w');
    //cr_ls_files(27);
    void* buffer;
    int bytes_read = cr_read(cr_open(27, "caramel.wav", 'r'), buffer, 2);
    printf("Esto es bytes_read %i\n", bytes_read);
    for(int i = 0; i < bytes_read; i++)
    {
        printf("%c", (char) ((uint8_t *) buffer)[i]);
    }
    printf("\n");

    free(buffer);
}

