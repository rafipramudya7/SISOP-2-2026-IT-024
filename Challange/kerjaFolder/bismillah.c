#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/prctl.h>
#include <sys/wait.h>

void logEvent(const char *proses, const char *status)
{
    FILE *f = fopen("isiLog.log", "a");
    if (!f)
        return;
    char waktu[512];
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(waktu, sizeof(waktu), "[%d:%m:%Y]-[%H:%M:%S]", tm_info);
    fprintf(f, "%s_%s_%s\n", waktu, proses, status);

    fclose(f);
}

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

void createFile(char *input)
{
    FILE *tmp = fopen("fileUtama.txt", "w");
    char waktu[512];
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(waktu, sizeof(waktu), "[%d:%m:%Y]-[%H:%M:%S]", tm_info);
    fprintf(tmp, "CREATED FILE AT %s CONTENT %s", waktu,input);
    fclose(tmp);
}

int main(int argc, char *argv[])
{
    srand(time(NULL));

    pid_t pid = fork();
    if (pid == 0)
    {
        execlp("sh", "sh", "-c", "mkdir folderLog");
        perror("error");
        _exit(1);
    }
    else
    {
        int status;
        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
        {
            logEvent("MKDIR", "SUCCES");
        }
        else
        {
            logEvent("MKDIR", "ERROR");
        }
    }
     pid = fork();
    if (pid == 0)
    {
        char input[100];
        scanf("%s",input);
        printf("%s","Masukkan Input:");
        createFile(input);

        execlp("sh", "sh", "-c", "cp fileUtama.txt  folderLog/");
        perror("error");
        _exit(1);
    }
    else
    {
        int status;
        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
        {
            logEvent("COPY", "SUCCES");
        }
        else
        {
            logEvent("COPY", "ERROR");
        }
    }
}