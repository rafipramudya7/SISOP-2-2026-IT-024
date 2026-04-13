#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/prctl.h>
#include <sys/wait.h>

void logEvent(char *proses, char *status)
{

    FILE *tmp = fopen("log.txt", "a");
    char waktu[50];
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(waktu, sizeof(waktu), "[%d:%m:%Y]-[%H:%M:%S]", tm_info);
    fprintf(tmp, "Proses [%s] berstatus: [%s] pada waktu: %s\n", proses, status, waktu);
    fclose(tmp);
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
int main(int argc, char *argv[])
{
    daemonInit

    if (argc == 1)
    {
        printf("cara menggunakan program ini: \n");
        printf("-daemon : berguna mengaktifkan daemon\n");
        return 0;
    }

    execlp("sh", "sh", "-c",
           "if mkdir kerjaFolder 2>> log.txt; then "
           "echo \"Proses [mkdir] berstatus: [SUCCESS] pada waktu: $(date '+[%d:%m:%Y]-[%H:%M:%S]')\n \" >> log.txt; "
           "else "
           "echo \"Proses [mkdir] berstatus: [ERROR] pada waktu: $(date '+[%d:%m:%Y]-[%H:%M:%S]')\n \" >> log.txt; "
           "fi",
           NULL);

    while (1)
    {
        pause();
    }
}