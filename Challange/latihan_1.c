#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/prctl.h>
#include <sys/wait.h>

void daemonInit()
{
    pid_t pid = fork();
    if (pid < 0)
        exit(EXIT_FAILURE);
    if (pid > 0)
        exit(EXIT_SUCCESS);

    umask(0);
    setsid();

    fclose(stdin);
    fclose(stdout);
    fclose(stderr);
}

void logEvent(char *proses, char *status)
{
    FILE *tmp = fopen("logFile.txt", "a");
    fprintf(tmp, "Proses [%s] berstatus [%s]\n", proses, status);
    fclose(tmp);
}
void cek()
{

    if (access("LoveLetter.txt", F_OK) != 0)
    {
        logEvent("CEK", "GAGAL");
    }
    else
    {
        FILE *tmp = fopen("LoveLetter.txt", "w");
        char waktu[50];
        time_t t = time(NULL);
        struct tm *tm_info = localtime(&t);
        strftime(waktu, sizeof(waktu), "[%d:%m:%Y]-[%H:%M:%S]", tm_info);
        fprintf(tmp, "sudah dicek ini ada terakhir: %s\n", waktu);
        fclose(tmp);
        logEvent("CEK", "GAGAL");

        return;
    }
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("Penggunaan:\n");
        printf("./angel -cekFile  : jalankan sebagai daemon (nama proses: maya)\n");
        printf("./angel -decrypt : decrypt LoveLetter.txt\n");
        printf("./angel -kill    : kill proses\n");
        return 0;
    }
    else if (strcmp(argv[1], "-cekFile") == 0)
    {

        daemonInit();
        prctl(PR_SET_NAME, "latihan", 0, 0, 0);
        memset(argv[0], 0, strlen(argv[0]));
        strcpy(argv[0], "latihan");
        srand(time(NULL));

        while (1)
        {
            sleep(7);
            cek();
        }
    }
    else if (argc > 1 && strcmp(argv[1], "-kill") == 0)
    {
        logEvent("kill", "RUNNING");
        pid_t pid = fork();
        if (pid == 0)
        {
            execlp("pkill", "pkill", "latihan", NULL);
            perror("exec gagal");
            _exit(1);
        }
        else
        {
            int status;
            wait(&status);
            if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
            {
                logEvent("kill", "SUCCESS");
            }
            else
            {
                logEvent("kill", "ERROR");
            }
        }
        return 0;
    }

    return 0;
}
