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
    //printf("file_direction = %s\n", *file_direction);
    cr_ls_process();
    
}

