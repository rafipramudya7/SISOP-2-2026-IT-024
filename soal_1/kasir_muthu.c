#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void error(int a)
{
    printf("[ERROR] Aiyaa! Proses gagal, file atau folder tidak ditemukan. %d\n",a);
    exit(1);
}

int main()
{

    pid_t pid = fork();
    int status;

    if (pid < 0)
    {
        error(1);
    }
    else if (pid == 0)
    {
        execlp("mkdir", "mkdir", "brankas_kedai", NULL);
        exit(1);
    }
    else
    {
        waitpid(pid, &status, 0);
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
        {
            error(8);
        }
    }

    pid = fork();

    if (pid < 0)
    {
        error(2);
    }
    else if (pid == 0)
    {
        execlp("sh", "sh", "-c", "cp buku_hutang.csv brankas_kedai/", NULL);
        exit(1);
    }
    else
    {
        waitpid(pid, &status, 0);
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
        {
            error(7);
        }
    }
    pid = fork();


    pid = fork();

    if (pid < 0)
    {
        error(5);
    }
    else if (pid == 0)
    {
        execlp("zip", "zip","-q", "-r", "rahasia_muthu.zip", "brankas_kedai", NULL);
        exit(1);
    }
    else
    {
        waitpid(pid, &status, 0);
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
        {
            error(6);
        }
    }
    printf("[INFO] Fuhh, selamat! Buku hutang dan daftar penagihan berhasil diamankan.\n");
    return 0;
}
