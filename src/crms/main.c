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
    cr_ls_process();
    //cr_exists(27, "nani");
    //cr_ls_files(27);
    //cr_start_process(26, "Raulitoteamo");
    //cr_ls_process();
    cr_finish_process(27);
    
}

