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
    // char* file = *file_direction;
    //printf("%s", *file_direction);
    //function();
    //cr_ls_files(27);
    //cr_exists(27, "nani");
    //cr_ls_files(27);
    //cr_ls_process();
    //cr_start_process(26, "Raulitoteamo");
    //cr_finish_process(25);
    //cr_open(27, "billetedeluk", 'w');
    //cr_ls_files(27);
    //void* buffer;
    //uint8_t aux;
    //int bytes_read = cr_read(cr_open(27, "amogus.mp4", 'r'), buffer, 10000000);
    //amogus.mp4
    //grub.mp4
    //drums.mp4
    //knowledg.jpg
    //nightcal.mp4
    //popcorn.mkv
    /*printf("Esto es bytes_read %i\n", bytes_read);
    for(int i = 0; i < bytes_read; i++)
    {
        aux = ((uint8_t *) buffer)[i];
        printf("%c", (char) aux);
    }
    printf("\n");

    free(buffer);
    */
}

