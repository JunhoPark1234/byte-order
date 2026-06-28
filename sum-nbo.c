#include <stdio.h>
#include <stdint.h>

uint32_t my_ntohl(uint32_t n)
{
    uint32_t res1 = (n & 0xff000000) >> 24;
    uint32_t res2 = (n & 0x00ff0000) >> 8;
    uint32_t res3 = (n & 0x0000ff00) << 8;
    uint32_t res4 = (n & 0x000000ff) << 24;

    return res1 | res2 | res3 | res4;
}

int read_file(char *filename, uint32_t *number)
{
    FILE *fp;

    fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Error: can't open file %s\n", filename);
        return 0;
    }

    if (fread(number, sizeof(uint32_t), 1, fp) != 1) {
        printf("Error: file %s is smaller than 4 bytes\n", filename);
        fclose(fp);
        return 0;
    }

    fclose(fp);

    *number = my_ntohl(*number);

    return 1;
}

int main(int argc, char *argv[])
{
    int i;
    uint32_t numbers[argc - 1];
    uint32_t sum = 0;

    if (argc < 2) {
        printf("syntax : sum-nbo <file1> [<file2>...]\n");
        printf("sample : sum-nbo a.bin b.bin c.bin\n");
        return 1;
    }

    for (i = 0; i < argc - 1; i++) {
        if (read_file(argv[i + 1], &numbers[i]) == 0) {
            return 1;
        }
    }

    for (i = 0; i < argc - 1; i++) {
        printf("%u(0x%08x)", numbers[i], numbers[i]);
        sum += numbers[i];

        if (i != argc - 2) {
            printf(" + ");
        }
    }

    printf(" = %u(0x%08x)\n", sum, sum);

    return 0;
}
