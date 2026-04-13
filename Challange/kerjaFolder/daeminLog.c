#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/prctl.h>
#include <sys/wait.h>

const char *listKata[] = {
    "aku akan fokus pada diriku sendiri",
    "aku mencintaimu dari sekarang hingga selamanya",
    "aku akan menjauh darimu, hingga takdir mempertemukan kita di versi kita yang terbaik",
    "kalau aku dilahirkan kembali, aku tetap akan terus menyayangimu"};

void logEvent(const char *proses, const char *status)
{
    FILE *f = fopen("ethereal.log", "a");
    if (!f)
        return;
    char waktu[512];
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(waktu, sizeof(waktu), "[%d:%m:%Y]-[%H:%M:%S]", tm_info);
    fprintf(f, "%s_%s_%s\n", waktu, proses, status);
    fclose(f);
}

void infoSignal(int sig)
{
    logEvent("DAEMON", "EXIT");
    exit(EXIT_SUCCESS); // ✅ fix bug 4
}

void secret()
{
    int r = rand() % 4;
    logEvent("secret", "RUNNING");
    FILE *tmp = fopen("LoveLetter.txt", "w");
    if (!tmp)
    {
        logEvent("secret", "ERROR");
        return;
    }
    fprintf(tmp, "%s", listKata[r]);
    fclose(tmp);
    logEvent("secret", "SUCCESS");
}

void createFile()
{
    logEvent("CREATE", "RUNNING");
    FILE *tmp = fopen("LoveLetter.txt", "w");
    if (tmp)
    {
        fclose(tmp);
        logEvent("CREATE", "SUCCESS"); // ✅ fix bug 1
    }
    else
    {
        logEvent("CREATE", "ERROR"); // ✅ fix bug 1
    }
}

void daemonInit()
{
    logEvent("DAEMON", "RUNNING"); // ✅ fix bug 2
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
    if (argc == 1)
    {
        printf("Penggunaan:\n");
        printf("./angel -daemon  : jalankan sebagai daemon (nama proses: maya)\n");
        printf("./angel -decrypt : decrypt LoveLetter.txt\n");
        printf("./angel -kill    : kill proses\n");
        return 0;
    }

    if (strcmp(argv[1], "-daemon") == 0)
    {
        createFile();
        daemonInit();
        prctl(PR_SET_NAME, "maya", 0, 0, 0);
        memset(argv[0], 0, strlen(argv[0]));
        strcpy(argv[0], "maya");
        srand(time(NULL));
        while (1)
        {
            sleep(10);
            secret();
        }
    }
    else if (strcmp(argv[1], "-kill") == 0)
    {
        logEvent("kill", "RUNNING");
        pid_t pid = fork();
        if (pid == 0)
        {
            execlp("pkill", "pkill", "-9","-x", "maya", NULL); // ✅ fix bug 3
            perror("exec gagal");
            _exit(1);
        }
        else
        {
            int status;
            wait(&status);
            if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
                logEvent("kill", "SUCCESS");
            else
                logEvent("kill", "ERROR");
        }
    }

    return 0;
}