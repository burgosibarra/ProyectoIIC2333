#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


void print_byte_as_bits(int val)
{
  for (int i = 7; 0 <= i; i--) {
    printf("%c", (val & (1 << i)) ? '1' : '0');
  }
}

int main()
{
    uint8_t bytes[16];
    while (1)
    {
        int testInteger;
        printf("Enter an integer: ");
        scanf("%d", &testInteger);

        FILE* fl = fopen("memfilled.bin", "rb");
        fseek(fl, 4096, SEEK_SET);
        fread(bytes, 1, 16, fl);
        for (int i=0; i<16;i++)
        {
            print_byte_as_bits(bytes[i]);
        }
        printf("\n");
        fclose(fl);
    }
    return 0;
}