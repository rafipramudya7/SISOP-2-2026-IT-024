
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/prctl.h>
#include <sys/wait.h>
int main()
{
    FILE *fptr;
    char buffer[1024];

    // Hint 1: Buka file ""pantun.txt""
    fptr = fopen("pantun.txt","r");

    if (fptr == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    // Hint 2: Buat perulangan untuk membaca baris demi baris
    while (fgets(buffer, sizeof(buffer), fptr))
    {
        // (Baris ini untuk menghapus karakter enter bawaan fgets)
        buffer[strcspn(buffer, "\n")] = 0;

        // Hint 3: Cetak kalimat yang dibaca
        
        printf("[%s]\n", buffer);

        
    }
    fclose(fptr);
    // Hint 4: Tutup file yang sudah dibuka

    return 0;
}