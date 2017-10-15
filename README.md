```
@@@@@@@@@@    @@@@@@   @@@@@@@   @@@  @@@  @@@          @@@@@@ 
@@@@@@@@@@@  @@@@@@@@  @@@@@@@@  @@@  @@@  @@@          @@@@@@@
@@! @@! @@!  @@!  @@@  @@!  @@@  @@!  @@@  @@!              @@@
!@! !@! !@!  !@!  @!@  !@!  @!@  !@!  @!@  !@!              @!@
@!! !!@ @!@  @!@  !@!  @!@  !@!  @!@  !@!  @!!          @!@!!@ 
!@!   ! !@!  !@!  !!!  !@!  !!!  !@!  !!!  !!!          !!@!@! 
!!:     !!:  !!:  !!!  !!:  !!!  !!:  !!!  !!:              !!:
:!:     :!:  :!:  !:!  :!:  !:!  :!:  !:!   :!:             :!:
:::     ::   ::::: ::   :::: ::  ::::: ::   :: ::::     :: ::::
 :      :     : :  :   :: :  :    : :  :   : :: : :      : : : 

      +-++-++-++-++-++-+ +-++-++-+ +-++-++-+
      |T||h||r||e||a||d| |d||a||n| |I||P||C|
      +-++-++-++-++-++-+ +-++-++-+ +-++-++-+

```

Thread dan IPC

##### PraModul 3

Requirement:
1. Linux
2. gcc / g++
3. vlc-nox
4. gcc -pthread -o [output] input.c

## Objectives
1. Peserta mengetahui thread
2. Peserta memahami bagaimana thread bekerja
3. Peserta memahami bagaimana cara membuat thread

## Daftar Isi
- [1. Thread](https://github.com/desyrahmi/sisop-modul-3#1-thread )
- [1.1 Thread](https://github.com/desyrahmi/sisop-modul-3#11-thread ) 
- [1.2 Join Thread](https://github.com/desyrahmi/sisop-modul-3#12-join-thread ) 
- [1.3 Mutual Exclusion](https://github.com/desyrahmi/sisop-modul-3#13-mutual-exclusion) 
- [2. IPC](https://github.com/desyrahmi/sisop-modul-3#2-ipc-interprocess-communication)
- [2.1 IPC](https://github.com/desyrahmi/sisop-modul-3#21-ipc)
- [2.2 Pipes](https://github.com/desyrahmi/sisop-modul-3#22-pipes)
- [2.2.1 Membuat Unnamed Pipe](https://github.com/desyrahmi/sisop-modul-3#221-membuat-unnamed-pipe)
- [2.2.2 Membuat Named Pipe](https://github.com/desyrahmi/sisop-modul-3#222-membuat-named-pipe)
- [2.3 Socket](https://github.com/desyrahmi/sisop-modul-3#23-sockets)
- [2.4 Message Queue](https://github.com/desyrahmi/sisop-modul-3#24-message-queues)
- [2.5 Semaphores](https://github.com/desyrahmi/sisop-modul-3#24-message-queues)
- [2.6 Shared Memory](https://github.com/desyrahmi/sisop-modul-3#25-semaphores)
- [Appendix](#appendix)

# 1. Thread 
### 1.1 Thread
Thread merupakan unit terkecil dalam suatu proses yang bisa dijadwalkan oleh sistem operasi. Dimana THREAD juga bisa menjalankan suatu tugas secara bersamaan. Dalam thread terdapat ID Thread, Program COunter, Register dan Stack.
Untuk mengecek bisa menggunakan perintah
```
$ top -H
```
Untuk membuat suatu Thread menggunakan :
```c
int pthread_create(pthread_t * thread,const pthread_attr_t * attr,void * (*start_routine)(void *),void *arg);
```
Hasil kembalian dari inisialisasi adalah 0 atau 1, 0 berarti tidak ada error sedangkan 1 berarti ada error.
Penjelasan Syntax:
```
- thread : mengembalikan thread id.
- attr   : Set menjadi NULL jika default thread attribut digunakan. Jika
           tidak definisi members dari struct pthread_attr_t didefinisikan menjadi bits/pthreadtypes.h) Atribut-atribut yang termasuk:
    * detached state (joinable? Default: PTHREAD_CREATE_JOINABLE. Other option: PTHREAD_CREATE_DETACHED)
    * scheduling policy (real-time? PTHREAD_INHERIT_SCHED,PTHREAD_EXPLICIT_SCHED,SCHED_OTHER)
    * scheduling parameter
    * inheritsched attribute (Default: PTHREAD_EXPLICIT_SCHED Inherit from parent thread: PTHREAD_INHERIT_SCHED)
    * scope (Kernel threads: PTHREAD_SCOPE_SYSTEM User threads: PTHREAD_SCOPE_PROCESS Pick one or the other not both.)
    * guard size
    * stack address (See unistd.h and bits/posix_opt.h _POSIX_THREAD_ATTR_STACKADDR)
    * stack size (default minimum PTHREAD_STACK_SIZE set in pthread.h)
- void * (*start_routine) : pointer ke fungsi yang akan menjadi thread. Fungsi menjadi sebuah argument yang menunjuk ke fungsi void.
- *arg - pointer ke argument fungsi. 


```

Contoh membuat program tanpa menggunakan thread [playtanpathread.c](https://github.com/desyrahmi/sisop-modul-3/blob/master/playtanpathread.c):
```c
#include<stdio.h>
#include <unistd.h>
#include <stdlib.h>
/*************************************************************************************
*Download file lagu di  https://drive.google.com/open?id=0Byp4qmu5MH1heHNfSUt5VE8zWVk*
*************************************************************************************/
int main()
{
    int i,length=10;
    system("clear");
    for(i=length;i>0;i--)
    {
        printf("%d",i);
        fflush(stdout);
        sleep(1);
        system("clear");
    }
    system("cvlc bagimu-negri.mp3");
}

```

Contoh membuat program menggunakan thread [playthread.c](https://github.com/desyrahmi/sisop-modul-3/blob/master/playthread.c):
```c
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdlib.h>
/*******************************************************
*compile dengan cara gcc -pthread -o [output] input.c *
*******************************************************/

pthread_t tid[2];//inisialisasi array untuk menampung thread dalam kasusu ini ada 2 thread

int length=157;//inisialisasi jumlah looping
void* playandcount(void *arg)
{
    unsigned long i=0;
    pthread_t id=pthread_self();
    int iter;
    if(pthread_equal(id,tid[0]))//thread untuk menjalankan counter
    {
        system("clear");
        for(iter=length;iter>0;iter--)
        {
            printf("%i",iter);
            fflush(stdout);
            sleep(1);
            system("clear");
        }
    }
    else if(pthread_equal(id,tid[1]))
    {
        system("cvlc bagimu-negri.mp3");
    }
    return NULL;
}
int main(void)
{
    int i=0;
    int err;
    while(i<2)//looping membuat thread 2x
    {
        err=pthread_create(&(tid[i]),NULL,&playandcount,NULL);//membuat thread
        if(err!=0)//cek error
        {
            printf("\n can't create thread : [%s]",strerror(err));
        }
        else
        {
            printf("\n create thread success");
        }
        i++;
    }
    pthread_join(tid[0],NULL);
    pthread_join(tid[1],NULL);
    return 0;
}

```

Kesimpulan : terlihat ketika program menggunakan thread dapat menjalankan dua task secara bersamaan.
### 1.2 Join Thread
Fungsi untuk melakukan penggabungan dengan thread lain yang telah di-terminasi (telah di exit).Bila thread yang ingin di-join belum diterminasi,Maka fungsi ini akan menunggu hingga thread yang diinginkan telah terminated.

Contoh program C Join_Thread [thread_join.c](https://github.com/desyrahmi/sisop-modul-3/blob/master/thread_join.c):
```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> //library thread

void *print_message_function( void *ptr );

int main()
{
     pthread_t thread1, thread2;//inisialisasi awal
     const char *message1 = "Thread 1";
     const char *message2 = "Thread 2";
     int  iret1, iret2;

     iret1 = pthread_create( &thread1, NULL, print_message_function, (void*) message1);//membuat thread pertama
     if(iret1)//jika eror
     {
         fprintf(stderr,"Error - pthread_create() return code: %d\n",iret1);
         exit(EXIT_FAILURE);
     }

     iret2 = pthread_create( &thread2, NULL, print_message_function, (void*) message2);//membuat thread kedua
     if(iret2)//jika gagal
     {
         fprintf(stderr,"Error - pthread_create() return code: %d\n",iret2);
         exit(EXIT_FAILURE);
     }

     printf("pthread_create() for thread 1 returns: %d\n",iret1);
     printf("pthread_create() for thread 2 returns: %d\n",iret2);

     //pthread_join( thread1, NULL);
     //pthread_join( thread2, NULL); 

     exit(EXIT_SUCCESS);
}

void *print_message_function( void *ptr )
{
     char *message;
     message = (char *) ptr;
     printf("%s \n", message);
}
```
Keterangan :
- Pada program di atas kita mengcomment baris pthread_join hasilnya tidak akan memunculkan tulisan Thread 1 dan Thread 2 padahal diinisialisasi thread program akan menjalankan fungsi print_message. 
- Sekarang kita mencoba menghapus comment pada Pthread_join. Hasilnya program akan mengeluarkan output Thread 1 dan Thread 2. 

Kesimpulan :
Pada program pertama tidak menjalankan fungsi print_message karena sebelum kedua Thread dijadwalkan, parent_thread telah selesai dieksekusi sehingga tidak menjalankan fungsi bawaan. Pada program kedua pthread_join digunakan untuk menunda eksekusi calling thread hingga target thread selesai dieksekusi, dengan fungsi ini parent_thread akan disuspend hingga target thread selesai dieksekusi.

### 1.3 Mutual Exclusion
Suatu cara yang menjamin jika ada sebuah proses yang menggunakan variabel atau berkas yang sama (digunakan juga oleh proses lain), maka proses lain akan dikeluarkan dari pekerjaan yang sama.

Contoh program Mutual_Exclusion [threadmutex.c](https://github.com/desyrahmi/sisop-modul-3/blob/master/threadmutex.c):
```c
#include<stdio.h>
#include<pthread.h>
 
int i=0;
pthread_t tid[2];
pthread_mutex_t lock;
 
void *tulis(void *ptr)
{
    //pthread_mutex_lock(&lock);
    i=i+1;
    printf("Thread %d dimulai\n",i);
    printf("Tekan enter untuk mengakhiri thread\n");
    getchar();
    printf("Thread %d selesai\n",i);
    //pthread_mutex_unlock(&lock);
    return NULL;
}
 
int main()
{
    int j,err;
    for(j=0;j<2;j++)
    {
     err=pthread_create(&tid[j],NULL,&tulis,NULL);
     if(err==0)printf("Thread Created\n");
    }
    for(j=0;j<2;j++)
    {
     pthread_join(tid[j],NULL);
    }
}

```
Keterangan :
- Jika kita menjalankan program diatas hasilnya tidak sesuai dengan yang kita inginkan di mana Thread 1 belum selesai processing, scheduler telah menjadwalkan Thread 2. 
- Selanjutnya kita mencoba menghapus comment pada pthread_mutex_lock dan pthread_mutex_unlock. Hasilnya akan memunculkan Thread 1 selesai sesuai yang program seharusnya jalankan. 

Kesimpulan :
Kegunaan dari Mutex adalah untuk menjaga sumber daya suatu thread tidak digunakan oleh thread lain.


# 2. IPC (*Interprocess Communication*)
### 2.1 IPC
IPC (*Interprocess Communication*) adalah cara atau mekanisme pertukaran data antara satu proses dengan proses lain, baik pada komputer yang sama atau komputer jarak jauh yang terhubung melalui suatu jaringan.

### 2.2 Pipes
Pipe merupakan komunikasi sequensial antar proses yang saling terelasi. Kelemahannya, hanya dapat digunakan untuk proses yang saling berhubungan dan secara sequensial.
Terdapat dua jenis pipe:
- unnamed pipe : Komunikasi antara parent dan child proses.
- named pipe : Biasa disebut sebagai FIFO, digunakan untuk komunikasi yang berjalan secara independen. **Hanya bisa digunakan jika kedua proses menggunakan *filesystem* yang sama.**
```
$ ls | less
```
#### 2.2.1 Membuat Unnamed Pipe
Example : [unnamed-pipe.c](https://github.com/desyrahmi/sisop-modul-3/blob/master/unnamed-pipe.c)
```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
	int pfds[2];
	pipe(pfds);
	if ( !fork() ) {
		close(1);
		dup(pfds[1]);
		close(pfds[0]);
		execlp("ls", "ls", NULL);
	} else {
		close(0);
		dup(pfds[0]);
		close(pfds[1]);
		char *grep[] = {"grep", "pipe", NULL};
		execv("/bin/grep", grep);
	}
	return 0;
}
```
Hasilnya
```
unamed-pipe.c
```
#### 2.2.2 Membuat Named Pipe
Example: [write.c](https://github.com/desyrahmi/sisop-modul-3/blob/master/write.c) [read.c](https://github.com/desyrahmi/sisop-modul-3/blob/master/read.c)

Write
```c
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
 
int main() {
    int fd;
    char * myfifo = "/tmp/myfifo";

    mkfifo(myfifo, 0666);

    char arr1[80], arr2[80];
    while (1) {
        fd = open(myfifo, O_WRONLY);

        fgets(arr2, 80, stdin);

        write(fd, arr2, strlen(arr2)+1);
        close(fd);

        fd = open(myfifo, O_RDONLY);

        read(fd, arr1, sizeof(arr1));

        printf("User2: %s\n", arr1);
        close(fd);
    }
    return 0;
}
```
Read
```c
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
 
int main() {
    int fd1;
    char * myfifo = "/tmp/myfifo";

    mkfifo(myfifo, 0666);
    char str1[80], str2[80];
    while (1) {
        fd1 = open(myfifo,O_RDONLY);
        read(fd1, str1, 80);
 
        printf("User1: %s\n", str1);
        close(fd1);

        fd1 = open(myfifo,O_WRONLY);
        fgets(str2, 80, stdin);
        write(fd1, str2, strlen(str2)+1);
        close(fd1);
    }
    return 0;
}
```

### 2.3 Sockets
*Socket* merupakan sebuah *end-point* dalam sebuah proses yang saling berkomunikasi. Biasanya *socket* digunakan untuk komunikasi antar proses pada komputer yang berbeda, namun dapat juga digunakan dalam komputer yang sama.

Example : [socket-server.c](https://github.com/desyrahmi/sisop-modul-3/blob/master/socket-server.c) [socket-client.c](https://github.com/desyrahmi/sisop-modul-3/blob/master/socket-client.c)

Server
```c
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080

int main(int argc, char const *argv[]) {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";
      
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    valread = read( new_socket , buffer, 1024);
    printf("%s\n",buffer );
    send(new_socket , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
    return 0;
}
```
Client
```c
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080
  
int main(int argc, char const *argv[]) {
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    send(sock , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
    valread = read( sock , buffer, 1024);
    printf("%s\n",buffer );
    return 0;
}
```
Jalankan proses server dulu, kemudian jalankan clientnya. Dan amati apa yang terjadi.

### 2.4 Message Queues
Merupakan komunikasi antar proses dimana proses tersebut menciptakan internal linked-list pada alamat kernel Sistem Operasi. Pesannya disebut sebagai *queue* sedangkan pengenalnya disebut *queue* ID. *Queue* ID berguna sebagai *key* untuk menandai pesan mana yang akan dikirim dan tujuan pengiriman pesannya.

### 2.5 Semaphores
Semaphore berbeda dengan jenis-jenis IPC yang lain. Pada pengaplikasiannya, semaphore merupakan sebuah counter yang digunakan untuk controlling resource oleh beberapa proses secara bersamaan.
- Jika suatu counter block memory memiliki nilai positif, semaphore dapat menggunakan resource untuk prosesnya, dan mengurangi nilai counter block dengan 1 untuk menandai bahwa suatu block memory tengah digunakan.
- Sebaliknya, jika semaphore bernilai 0, proses akan masuk pada mode sleep sampai semaphore bernilai lebih besar dari 0.

### 2.6 Shared Memory
Sebuah mekanisme *mapping area(segments)* dari suatu blok *memory* untuk digunakan bersama oleh beberapa proses. Sebuah proses akan menciptakan *segment memory*, kemudian proses lain yang diijinkan dapat mengakses *memory* tersebut. *Shared memory* merupakan cara yang efektif untuk melakukan pertukaran data antar program.

Example: [Proses 1](https://github.com/desyrahmi/sisop-modul-3/blob/master/proses1.c) [Proses 2](https://github.com/desyrahmi/sisop-modul-3/blob/master/proses2.c)

Proses 1
```c
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>

void main()
{
        key_t key = 1234;
        int *value;

        int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
        value = shmat(shmid, NULL, 0);

        *value = 10;

        printf("Program 1 : %d\n", *value);

        sleep(5);

        printf("Program 1: %d\n", *value);
        shmdt(value);
        shmctl(shmid, IPC_RMID, NULL);
}
```
Proses 2
```c
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>

void main()
{
        key_t key = 1234;
        int *value;

        int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
        value = shmat(shmid, NULL, 0);

        printf("Program 1 : %d\n", *value);
	*value = 30;

        sleep(5);

        printf("Program 1: %d\n", *value);
        shmdt(value);
        shmctl(shmid, IPC_RMID, NULL);
}
```
Jalankan proses 1 terlebih dahulu, lalu proses 2. 
Hasilnya
Proses 1
```
Program 1 : 10
Program 1 : 30
```
Proses 2
```
Program 1 : 30
Program 1 : 10
```

# Appendix
### Libraries documentation (and functions) :sparkles: :sparkles: :camel:
```
$ man {anything-you-want-to-know}
$ man mkfio
$ man fcntl.h
$ man unistd.h
```

## Soal Latihan 

#### Latihan 1
Buatlah sebuah program multithreading yang dapat menyalin isi file baca.txt ke dalam file salin1.txt. Kemudian menyalin isi dari file salin1.txt ke dalam file salin2.txt!
#### Latihan 2
Buatlah sebuah program multithreading yang dapat menampilkan N bilangan prima pertama. program akan dieksekusi menggunakan thread sebanyak T dimana setiap thread akan melakukan print sebanayk N/T bilangan prima.
Input : N = banyak bilangan prima
		T = banyak thread yang digunakan
#### Latihan 3
Ohan adalah seorang network administrator, dia bekerja menggunakan linux server. Suatu ketika Ohan
merasa jenuh dengan pekerjaannya dia ingin mendengarkan lagu, tetapi linux server tidak memiliki GUI
sehingga Ohan harus memutar musik menggunakan konsol/terminal. Bantulah Ohan membuat pemutar
musik berbasis konsol.
Pemutar musik memiliki spesifikasi sebagai berikut :
1. Perintah help untuk menampilkan daftar perintah yang dapat digunakan.
2. Memiliki fitur list untuk menampilkan semua lagu pada folder playlist
3. Memiliki fitur play untuk menjalankan lagu
4. Memiliki fitur pause setelah t detik
5. Memiliki fitur continue setelah t detik
6. Memiliki fitur stop setelah t detik

### References 
https://notes.shichao.io/apue/
https://www.gta.ufrj.br/ensino/eel878/sockets/index.html
http://advancedlinuxprogramming.com/alp-folder/alp-ch05-ipc.pdf

