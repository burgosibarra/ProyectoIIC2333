#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../crms_API/crms_API.h"
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

extern char** file_direction;

int main(int argc, char **argv)
{
    cr_mount("memfilled.bin");
    // char* file = *file_direction;
    //printf("%s", *file_direction);
    //function();
    //cr_ls_process();
    //cr_exists(27, "nani");
    //cr_ls_files(27);
    //cr_start_process(26, "Raulitoteamo");
    cr_ls_process();
    cr_finish_process(27);
    cr_ls_process();
}

