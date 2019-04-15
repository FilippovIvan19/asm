#include <stdio.h>
#include <sys/stat.h>


int main()
{
    FILE *lock_file = fopen("lock", "r+b");

    struct stat file_info;
    stat("lock",&file_info);

    if (file_info.st_size != 9440)
    {
        printf("wrong size of file ""lock"", i don't want to work\n");
        return 1;
    }

    char jmp_byte[1] = "";

    fseek(lock_file, 4177, SEEK_SET);
    fread(jmp_byte, sizeof(char), 1, lock_file);

    if (*jmp_byte != 116)
    {
        printf("file ""lock"" is incorrect or has already fixed, i don't want to work\n");
        printf("%d\n", *jmp_byte);
        fclose(lock_file);
        return 2;
    }

    fseek(lock_file, -1, SEEK_CUR);
    *jmp_byte = 117;
    fwrite(jmp_byte, sizeof(char), 1, lock_file);

    fclose(lock_file);
    printf("success\n");
    return 0;
}
