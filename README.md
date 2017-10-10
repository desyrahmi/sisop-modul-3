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

## Objectives
1. Peserta mengetahui thread
2. Peserta memahami bagaimana thread bekerja
3. Peserta memahami bagaimana cara membuat thread

## Daftar Isi
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

### 1.2 

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


### References 
https://notes.shichao.io/apue/
https://www.gta.ufrj.br/ensino/eel878/sockets/index.html
http://advancedlinuxprogramming.com/alp-folder/alp-ch05-ipc.pdf

