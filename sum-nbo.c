#include <stdio.h>
#include <stdint.h>

int read_file(char *filename, uint32_t *number)
{
    FILE *fp;
    unsigned char buffer[4];

    fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Error: can't open file %s\n", filename);
        return 0;
    }

    if (fread(buffer, 1, 4, fp) != 4) {
        printf("Error: file %s is smaller than 4 bytes\n", filename);
        fclose(fp);
        return 0;
    }

    fclose(fp);

    *number = ((uint32_t)buffer[0] << 24) |
              ((uint32_t)buffer[1] << 16) |
              ((uint32_t)buffer[2] << 8)  |
              (uint32_t)buffer[3];

    return 1;
}

int main(int argc, char *argv[])
{
    int i;
    uint32_t numbers[argc - 1];
    uint32_t sum = 0;

    if (argc < 2) {
        printf("usage : sum-nbo <file1> [<file2>...]\n");
        return 0;
    }

    for (i = 0; i < argc - 1; i++) {
        if (read_file(argv[i + 1], &numbers[i]) == 0) {
            return 0;
        }
    }

    for (i = 0; i < argc - 1; i++) {
        printf("%u(0x%08x)", numbers[i], numbers[i]);
        sum += numbers[i];

        if (i != argc - 2)
            printf(" + ");
    }

    printf(" = %u(0x%08x)\n", sum, sum);

    return 0;
}
