#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../crms_API/crms_API.h"


int main(int argc, char **argv)
{
    cr_mount("oli");
    printf("file_direction = %s\n", *file_direction);
    cr_mount("oliii");
    printf("file_direction = %s\n", *file_direction);
}
