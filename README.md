# SISOP-1-2026-IT-024

<div align="center">

<img src="https://img.shields.io/badge/Sistem_Operasi-1_2026-1a1a2e?style=for-the-badge&logo=linux&logoColor=white"/>
<img src="https://img.shields.io/badge/Shell-Bash-4EAA25?style=for-the-badge&logo=gnu-bash&logoColor=white"/>
<img src="https://img.shields.io/badge/Tool-AWK-FF6B35?style=for-the-badge&logo=gnu&logoColor=white"/>

</div>

---

<div align="center">

|  Nama |  NRP |
|:-------:|:------:|
| **Muhammad Rafi Pramudya Putra** | `5027251024` |

</div>



##  Kasbon Warga Kampung Durian Runtuh

> **Description :**
> membuat file c yang mengatur 3 child




####  Child 1 
Membuat sebuah child yang berfungsi untuk membuat sebuah folder bernama `brankas_kedai` dengan cara mengugnkana fungsi `execlp()` dan argumen `mkdir`. DIsini diberkian juga opengecekan pid agar process mana yang kita inginkan untuk melakukan sebuah proses , dan buat parrent nya utnuk menunggu child 1

```c
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
```

####  Child 2
Membuat sebuah child yang berfungsi untuk mencari baris yang memiliki value `Belum Lunas` dan memindahkanya ke `brankas_kedai/daftar_penunggak.txt`

```c
    if (pid < 0)
    {
        error(3);
    }
    else if (pid == 0)
    {
        execlp("sh", "sh", "-c",
               "grep 'Belum Lunas' brankas_kedai/buku_hutang.csv > brankas_kedai/daftar_penunggak.txt",
               NULL);
        exit(1);
    }
    else
    {
        waitpid(pid, &status, 0);
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
        {
            error(4);
        }
    }
```

####  Child 3
Menjadikan folder `brankas_kedai` menjadi sebuah zip.
```c
    if (pid < 0)
    {
        error(3);
    }
    else if (pid == 0)
    {
        execlp("sh", "sh", "-c",
               "grep 'Belum Lunas' brankas_kedai/buku_hutang.csv > brankas_kedai/daftar_penunggak.txt",
               NULL);
        exit(1);
    }
    else
    {
        waitpid(pid, &status, 0);
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
        {
            error(4);
        }
    }
```
##  The World never stops, even when you feel tired.

> **Description :**
> membuat sebuah daemon yang bertugas mengecek perubahan file dan menuliskan sebuah log

disini saya mengugnkana 3 child proses yang nantinya akan berjalan di background prosess. child 1 berfungsi sebagai pengganti parent yang memilki `setid()` yang dijalankan dengan fungsi `demonize()`.
```c
void daemonize(){
    pid_t pid = fork();
    if(pid < 0)exit(EXIT_FAILURE);
    if(pid > 0 )exit(EXIT_SUCCESS);
    setsid();
    fclose(stdin);
    fclose(stdout);
    fclose(stderr);
}
```

maksut dari fungsi ini adalah mematikan parent dan mengatur session id untuk child dan menutup terminal yang tidak perlu

lalu untuk fungsi utama 

```c
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
```
pertama ada fungsi `srand()` berguna mengatur seed agar fungsi `rand()` yang dijalankan program bervariasi dan ada fungsi `signal()` yang berfungsi mengatur signal pada setiap child yang berarti jika ada exit dari setiap child baik dengan `kill` maupun `ctrl+c` akan menjalankan fungsi `infoSignal()`.

lalu kita ada 2 prosess pemanggilan 2 child dengan nama `c1` dan `c2` dan ada  `while()` yang berfungsi menjaga daemon pengganti parent yang berguna menjaga child tetap aktif.


###  `createContract()`
sebuah fungsi yang berguna untuk menginisiasi file `contract.txt`

```c
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
```

disini kita open dulu dengan `fopen()` lalu menyimpanya terlebih dahulu ke `tmp` lalu kita isi tmp dengan format sesuai perintah dan memasukan waktu  ke dalam `tmp`



###  `logChild()`
Fungsi untuk mengatur apa yang dikerjakan child 2
```c
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
```

disini kita membuka file `work.log` dengan argument `"a"` yang berarti append (menambah bukan menimpa) disini kita akan memasukan salah satu element array secara random dari list yang telah dibuat
```c
const char *listStatus[] = {
    "[awake]", "[drifting]", "[numbness]"};

```

dengan ditambah kata kata sesuai format.

###  `auditChild()`
Fungsi untuk mengatur apa yang dikerjakan child 1.

berikut perinciannya

```c
        if (!cekFile("contract.txt"))
        {
            createContract();
        }

```
cek apakah file ada atau tidak jika tidak ada maka buat menggaunkana fungsi `createContract()`

```c
        FILE *tmp = fopen("contract.txt", "r");
        if (!tmp)
            continue;

        char current[512] = {0};

        fread(current, 1, sizeof(current) - 1, tmp);
        fclose(tmp);
```

ambil isi file dengan menggunakan pointer `tmp` lalu mencopy nya ke variable `current` agar bisa dibandingkan.

```c
        if (strlen(lastContent )==0)
        {
            strcpy(lastContent, current);
            continue;
        }
```
cek jika masih awal inisiasi maka copy file saat ini ke `lastContent` variable


```c
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

```
deteksi jika variable `lastContent` ada yang berbedan dengan isi file saat ini maka buka file `work.log` lalu append `"contract violated."` ke dalam `work.log`  lalu untuk file `contract.txt` kita timpa dengan text baru sesuai format dengan ditambah waktu pengganti sekarang . dan yagn terakhir jangan lupa ganti isis `lastContent` menjadi isi yang baru yang berguna untuk pengecekan selanjutnya.

###  `cekFile()`
sebuah fungsi yang berguna untuk mengecek apakah ada file yang dicari

```c
int cekFile(const char *filename)
{
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}
```
###  `infoSignal()`
sebuah fungsi yang berguna untuk melakukan penulisan ke dalam `work.log` jika ada condisi sebuah child yang exit. dan merubah status running menjadi off guna mematikan daemon pengganti parent

```c
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
```




---
<div align="center">

**SISOP 1 · 2026 · IT-024**

</div>