#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>

int restor_token = 0;
int runStatus = 1;

const char *listStatus[] = {
    "[awake]", "[drifting]", "[numbness]"};

void infoSignal(int sig)
{

    FILE *tmp = fopen("work.log", "a");

    if (tmp)
    {
        fprintf(tmp, "We really weren’t meant to be together\n");
        fclose(tmp);
        /* code */
    }
    runStatus = 0;
}

int cekFile(const char *filename)
{
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

void createContract()
{
    FILE *tmp = fopen("contract.txt", "w");
    if (!tmp)
        return;

    time_t now = time(NULL);
    fprintf(tmp,
            "“A promise to keep going, even when unseen.”\ncreated at: %s",
            ctime(&now));

    fclose(tmp);
}

void logChild()
{
    while (runStatus)
    {
        sleep(5);
        FILE *tmp = fopen("work.log", "a");
        if (tmp)
        {
            int r = rand() % 3;
            fprintf(tmp, "still working… %s\n", listStatus[r]);
            fclose(tmp);
        }
    }
}

void auditChild()
{

    char lastContent[512] = {0};
    while (runStatus)
    {
        sleep(1);

        // cek apakah file ada jika tidak ada buat
        if (!cekFile("contract.txt"))
        {
            createContract();
        }

        // ambil isi file untuk dibandingkan
        FILE *tmp = fopen("contract.txt", "r");
        if (!tmp)
            continue;

        char current[512] = {0};

        fread(current, 1, sizeof(current) - 1, tmp);
        fclose(tmp);

        //jika baru awal inisiasi maka  copy current file ke last content
        if (strlen(lastContent )==0)
        {
            strcpy(lastContent, current);
            continue;
        }

        //jika terdeteksi ada yang diedit
        if (strcmp(lastContent, current) != 0)
        {
            FILE *tmp = fopen("work.log", "a");
            if (tmp)
            {
                fprintf(tmp, "contract violated.\n");
                fclose(tmp);
            }

            FILE *fw = fopen("contract.txt", "w");
            if (!fw)
                continue;

            time_t now = time(NULL);

            char buffer[512];
            snprintf(buffer, sizeof(buffer),
                     "“A promise to keep going, even when unseen.”\nrestored at: %s",
                     ctime(&now));

            fprintf(fw, "%s", buffer);
            fclose(fw);
            strcpy(lastContent, buffer);
        }
    }
}

void daemonize(){
    pid_t pid = fork();
    if(pid < 0)exit(EXIT_FAILURE);
    if(pid > 0 )exit(EXIT_SUCCESS);
    setsid();
    fclose(stdin);
    fclose(stdout);
    fclose(stderr);
}

int main(){
    srand(time(NULL));
    signal(SIGTERM,infoSignal);
    signal(SIGINT,infoSignal);

    daemonize();
    
    createContract();

    pid_t c1  = fork();
    if(c1 == 0){
        auditChild();
        exit(0);
    }

    pid_t c2 = fork();

    if(c2 == 0){
        logChild();
        exit(0);
    }

    while (runStatus)
    {
        pause();
    }
    
    return 0; 


}