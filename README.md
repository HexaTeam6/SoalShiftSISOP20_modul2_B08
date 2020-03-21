# SoalShiftSISOP20_modul2_B08

Soal Shift Modul 2 yang berfokus pada penggunaan exec, fork, wait, dan daemon.

## SOAL 1

Buatlah program C yang menyerupai crontab untuk menjalankan script bash dengan
ketentuan sebagai berikut:
  1.  Program menerima 4 argumen berupa:
      1.    Detik: 0-59 atau * (any value)
      2.    Menit: 0-59 atau * (any value)
      3.    Jam: 0-23 atau * (any value)
      4.    Path file .sh
  2.  Program akan mengeluarkan pesan error jika argumen yang diberikan tidak
      sesuai.
  3.  Program hanya menerima 1 config cron.
  4.  Program berjalan di background (daemon).
  5.  Tidak boleh menggunakan fungsi system().

Contoh: ./program \* 34 7 /home/somi/test.sh
Program dengan argumen seperti contoh di atas akan menjalankan script test.sh setiap
detik pada jam 07:34.

### Penyelesaian

Berikut ini merupakan kode C untuk menyelesaikan soal no.1
````
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>

int checkfile(const char *fname)
{
    FILE *file;
    if ((file = fopen(fname, "r")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[]) {
	struct stat sts;

	if(argc > 5) {
    	printf("Error\n");
  	}
  	else {
  		if(strlen(argv[1]) > 2 || strlen(argv[2]) > 2 || strlen(argv[3]) > 2) {
  			printf("Error\n");
		}
		else {
			int error = 0, set = 0;

			if(argv[1][0] == '-' || argv[2][0] == '-' || argv[3][0] == '-') {
				error = 1;
			}

			if(argv[1][0] == '*' && argv[1][1] != 0) {
				error = 1;
			}
			
			if(argv[2][0] == '*' && argv[2][1] != 0) {
				error = 1;
			}
			
			if(argv[3][0] == '*' && argv[3][1] != 0) {
				error = 1;
			}
			
			if(argv[1][0] != '*' && argv[1][1] == '*') {
				error = 1;
			}
			
			if(argv[2][0] != '*' && argv[2][1] == '*') {
				error = 1;
			}
			
			if(argv[3][0] != '*' && argv[3][1] == '*') {
				error = 1;
			}

			if(argv[1][0] != '*' && atoi(argv[1]) > 59) {
				error = 1;
			}

			if(argv[2][0] != '*' && atoi(argv[2]) > 59) {
				error = 1;
			}

			if(argv[3][0] != '*' && atoi(argv[3]) > 23) {
				error = 1;
			}

			if(checkfile(argv[4]) == 0) {
				error = 1;
			}

			if(error == 1) {
				printf("Error\n");
			}
			else {
				pid_t pid, sid;
			
				pid = fork();
			
				if (pid < 0) {
				    exit(EXIT_FAILURE);
				}
				
				if (pid > 0) {
				    exit(EXIT_SUCCESS);
				}
				
				umask(0);
				
				sid = setsid();
				if (sid < 0) {
				    exit(EXIT_FAILURE);
				}
				
				close(STDIN_FILENO);
				close(STDOUT_FILENO);
				close(STDERR_FILENO);
				
				while (1) {
				    time_t t = time(NULL);
				    struct tm tm = *localtime(&t);
				
				    int i, j, k;
					
					if(set == 0) {
						if(argv[1][0] == '*') {
				        	i = tm.tm_sec;
					    }
					    else {
					        i = atoi(argv[1]);
					    }
					
					    if(argv[2][0] == '*') {
					        j = tm.tm_min;
					    }
					    else {
					        j = atoi(argv[2]);
					    }
					
					    if(argv[3][0] == '*') {
					        k = tm.tm_hour;
					    }
					    else {
					        k = atoi(argv[3]);
					    }
					    
					    set++;
					}
				    
				    if(set > 0) {
				       if(argv[1][0] == '*') {
				          i = tm.tm_sec;
				       }
				       if(argv[2][0] == '*') {
				          j = tm.tm_min;
				       }
				       if(argv[3][0] == '*') {
				          k = tm.tm_hour;
				       }
				    }
				
				    if(tm.tm_sec == i && tm.tm_min == j && tm.tm_hour == k) {
				       	if(fork() == 0) {
							char *argm[] = {"bash", argv[4], NULL};
							execv("/bin/sh", argm);
						}
				    }
				
				    sleep(1);
				}
			}
		}
 	}
}


````

Dimana program hanya akan menerima 4 argumen yaitu detik, menit, jam, dan path file.sh menggunakan
`int main(int argc, char *argv[])` tetapi kita harus meng-include `#include <stdio.h>`.

Untuk mengatasi error pada argumen yang tidak sesuai yaitu menggunakan
````
if(argc > 5) {
    	printf("Error\n");
 }
 else {
  	if(strlen(argv[1]) > 2 || strlen(argv[2]) > 2 || strlen(argv[3]) > 2) {
  		printf("Error\n");
	}
	else {
		int error = 0, set = 0;

		if(argv[1][0] == '-' || argv[2][0] == '-' || argv[3][0] == '-') {
			error = 1;
		}

		if(argv[1][0] == '*' && argv[1][1] != 0) {
			error = 1;
		}
			
		if(argv[2][0] == '*' && argv[2][1] != 0) {
			error = 1;
		}
			
		if(argv[3][0] == '*' && argv[3][1] != 0) {
			error = 1;
		}
			
		if(argv[1][0] != '*' && argv[1][1] == '*') {
			error = 1;
		}
			
		if(argv[2][0] != '*' && argv[2][1] == '*') {
			error = 1;
		}
			
		if(argv[3][0] != '*' && argv[3][1] == '*') {
			error = 1;
		}

		if(argv[1][0] != '*' && atoi(argv[1]) > 59) {
			error = 1;
		}

		if(argv[2][0] != '*' && atoi(argv[2]) > 59) {
			error = 1;
		}

		if(argv[3][0] != '*' && atoi(argv[3]) > 23) {
			error = 1;
		}

		if(checkfile(argv[4]) == 0) {
			error = 1;
		}
        
  // Jika error = 1 print error, jika tidak maka program akan lanjut
}     
````
Dimana apabila inputan lebih dari 4 argumen atau inputan angka dengan bintang secara bersamaan atau digit yang diberikan lebih dari 2
atau bilangan negatif atau bilangan yang lebih dari batas waktu masing - masing (detik, menit, jam) atau file path yang diberikan
tidak ada maka akan mengeluarkan tulisan error. Tetapi jika config cron benar maka program akan lanjut.

Setelah mengammbil argumen, maka config cron akan dicek. Apabila terdapat bintang pada detik atau menit atau jam, maka kita akan mengeset i(detik) atau j(menit)atau k(jam) sesuai dengan local time.
Untuk mengambil waktu sesuai dengan local time, kita menggunakan
````
time_t t = time(NULL);
struct tm tm = *localtime(&t);
````
Lalu mengeset masing - masing variabel yang berhubungan dengan bintang dengan cara
````
i = tm.tm_sec;  // Untuk detik
j = tm.tm_min;  // Untuk menit
k = tm.tm_hour; // Untuk jam
````
Jika tidak ada bintang, maka kita akan mengeset masing - masing variable sesuai inputan yang ada menggunakan `atoi()` 
untuk merubah argumen string menjadi integer dan perlu meng-include `#include <stdlib.h>`.
````
i = atoi(argv[1]);  // Untuk detik
j = atoi(argv[2]);  // Untuk menit
k = atoi(argv[3]);  // Untuk jam
````

Lalu kita akan melakukan proses `bash` dengan menggunakan `execv` ketika waktu pada local time 
sudah sesuai dengan config cron yang diinputkan.
````
if(tm.tm_sec == i && tm.tm_min == j && tm.tm_hour == k) {
	if(fork() == 0) {
           char *argm[] = {"bash", argv[4], NULL};
           execv("/bin/sh", argm);
        }
}
````
Lalu kita akan mengeset `sleep();` menjadi `sleep(1);` agar proses daemon berjalan (mengecek) per detik.

Untuk mengatasi config cron yang memakai bintang yaitu kita perlu mengeset kembali dengan cara
````
if(set > 0) {
	if(argv[1][0] == '*') {
	   i = tm.tm_sec;
	}
	if(argv[2][0] == '*') {
	   j = tm.tm_min;
	}
	if(argv[3][0] == '*') {
	   k = tm.tm_hour;
	}
 }
 ````

## SOAL 2

Shisoppu mantappu! itulah yang selalu dikatakan Kiwa setiap hari karena sekarang dia
merasa sudah jago materi sisop. Karena merasa jago, suatu hari Kiwa iseng membuat
sebuah program.
  1.  Pertama-tama, Kiwa membuat sebuah folder khusus, di dalamnya dia membuat
      sebuah program C yang per 30 detik membuat sebuah folder dengan nama
      timestamp [YYYY-mm-dd_HH:ii:ss].
  2.  Tiap-tiap folder lalu diisi dengan 20 gambar yang di download dari
      https://picsum.photos/, dimana tiap gambar di download setiap 5 detik. Tiap
      gambar berbentuk persegi dengan ukuran (t%1000)+100 piksel dimana t adalah
      detik Epoch Unix. Gambar tersebut diberi nama dengan format timestamp [YYYYmm-
      dd_HH:ii:ss].
  3.  Agar rapi, setelah sebuah folder telah terisi oleh 20 gambar, folder akan di zip dan
      folder akan di delete(sehingga hanya menyisakan .zip).
  4.  Karena takut program tersebut lepas kendali, Kiwa ingin program tersebut mengenerate
      sebuah program "killer" yang siap di run(executable) untuk
      menterminasi semua operasi program tersebut. Setelah di run, program yang
      menterminasi ini lalu akan mendelete dirinya sendiri.
  5.  Kiwa menambahkan bahwa program utama bisa dirun dalam dua mode, yaitu
      MODE_A dan MODE_B. untuk mengaktifkan MODE_A, program harus dijalankan
      dengan argumen -a. Untuk MODE_B, program harus dijalankan dengan argumen
      -b. Ketika dijalankan dalam MODE_A, program utama akan langsung
      menghentikan semua operasinya ketika program killer dijalankan. Untuk
      MODE_B, ketika program killer dijalankan, program utama akan berhenti tapi
      membiarkan proses di setiap folder yang masih berjalan sampai selesai(semua
      folder terisi gambar, terzip lalu di delete).
      
Kiwa lalu terbangun dan sedih karena menyadari programnya hanya sebuah mimpi.
Buatlah program dalam mimpi Kiwa jadi nyata!

Catatan:
- Tidak boleh memakai system().
- Program utama harus ter-detach dari terminal.

Hint:
- Gunakan fitur picsum.photos untuk mendapatkan gambar dengan ukuran
tertentu.
- Epoch Unix bisa didapatkan dari time().

### Penyelesaian
Berikut code penyelesaian untuk soal2.
````
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <syslog.h>
#include <stdio.h>
#include <wait.h>
#include <string.h>
#include <dirent.h>
#include <time.h>   

int main(int argc, char *argv[]) {
    if(argc != 2 || argv[1][1] != 'a' && argv[1][1] != 'b')
        printf("argumen only -a or -b");

    pid_t child_id, child_id2;
    char cwd[100];
    getcwd(cwd, sizeof(cwd));
    int status;
    int test;
    int pid2 = getpid();

    child_id = fork();
//     printf("%d\n", child_id);
    if (child_id < 0) exit(EXIT_FAILURE);

    if (child_id == 0) {
        FILE *ptr;
        char files[100];
        char data[1000];
        char dst[100];
        char src[100];
        char mode[100];

        sprintf(src, "%s/killer.c", cwd);
        sprintf(dst, "%s/killer", cwd);

        sprintf(files, "%s/killer.c", cwd);

        if (argv[1][1] == 'a') strcpy(mode, " char *argv[] = {\"killall\", \"-s\", \"9\", \"soal2\", NULL};");
        else if (argv[1][1] == 'b') sprintf(mode, " char *argv[] = {\"kill\", \"%d\", NULL};", pid2);
        
        sprintf(data,   "#include <stdlib.h>\n"
                        "#include <sys/types.h>\n"
                        "#include <unistd.h>\n"
                        "#include <stdio.h>\n"
                        "#include <wait.h>\n"
                        "#include <string.h>\n"

                        " int main() {"
                        " pid_t child_id;"
                        " int status;"

                        " child_id = fork();"
                        " if (child_id < 0) exit(EXIT_FAILURE);"

                        " if (child_id == 0) {"
                            " %s"
                            " execv(\"/usr/bin/killall\", argv);"
                        " } else {"
                            " while (wait(&status)>0);"
                        " }"
                        " }", mode);

        ptr = fopen(files, "w");
        fputs(data, ptr);
        fclose(ptr);

        char *argv[] = {"gcc", src, "-o", dst, NULL};
        execv("/usr/bin/gcc", argv);
    }
    
    while (wait(&status)>0);

    child_id2 = fork();
//     printf("%d\n", child_id2);
    if (child_id2 < 0) exit(EXIT_FAILURE);

    if (child_id2 == 0) {
        char src[100];

        sprintf(src, "%s/killer.c", cwd);
        char *argv[] = {"rm", src, NULL};
        execv("/bin/rm", argv);
    }
    
    while (wait(&status)>0);

    pid_t pid, sid;        // Variabel untuk menyimpan PID

    pid = fork();     // Menyimpan PID dari Child Process
//     printf("%d\n", pid);
    /* Keluar saat fork gagal
    * (nilai variabel pid < 0) */
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    /* Keluar saat fork berhasil
    * (nilai variabel pid adalah PID dari child process) */
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    umask(0);

    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    // if ((chdir("/")) < 0) {
    //     exit(EXIT_FAILURE);
    // }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while (1) {
        pid_t child1, child2, child3, child4;
        int status;
        
        time_t rawtime;
        struct tm * timeinfo;
        char buffer [80];

        time (&rawtime);
        timeinfo = localtime (&rawtime);

        strftime (buffer,80,"%Y-%m-%d_%H:%M:%S",timeinfo);
        // puts (buffer);
        char *dir = buffer;
        mkdir(dir,0777);

        child1 = fork();
        if (child1 < 0) exit(EXIT_FAILURE);

        if (child1 == 0) {
            for(int i=0;i<20;i++)
            { 
                if(fork() == 0) 
                { 
                    unsigned long get_time = (unsigned long)time(NULL);
                    get_time = (get_time%1000) + 100;

                    char link[50];
                    sprintf(link, "https://picsum.photos/%lu", get_time);
                    
                    time_t filet;
                    struct tm * fileti;
                    char files [80] = "";
                    char down_dir[100];
                    
                    time (&filet);
                    fileti = localtime (&filet);

                    strftime (files,80,"%Y-%m-%d_%H:%M:%S",fileti);
                    sprintf(down_dir, "%s/%s", buffer, files);
                    // puts(down_dir);


                    char *argv[] = {"wget", "-O", down_dir, link, NULL};
                    execv("/usr/bin/wget", argv);
                    exit(0); 
                } 
                sleep(5);
            } 
            for(int i=0;i<20;i++)
            wait(NULL); 
        }

        while (wait(&status)>0);

        child2 = fork();
        if (child2 < 0) exit(EXIT_FAILURE);

        if (child2 == 0) {
            char zip_name[100];
            sprintf(zip_name, "%s.zip", buffer);

            char *argv[] = {"zip", "-rm", zip_name, buffer, NULL};
            execv("/usr/bin/zip", argv);
            exit(0); 
        }

        while (wait(&status)>0);
        sleep(30);
    }
}
````
Pertama program diminta untuk membuat folder dengan format nama folder sesuai [YYYYmm-dd_HH:ii:ss] sehingga kita perlu menggunakan time() untuk mendapatkan localtime sekarang. Lalu string nama untuk folder diformat menggunakan fungsi strtime(). Lalu tinggal membuat folder dengan mkdir().
````
time_t rawtime;
struct tm * timeinfo;
char buffer [80];

time (&rawtime);
timeinfo = localtime (&rawtime);

strftime (buffer,80,"%Y-%m-%d_%H:%M:%S",timeinfo);
// puts (buffer);
char *dir = buffer;
mkdir(dir,0777);
````
Setelah itu program diminta agar dapat mendownload gambar ke folder yang telah dibuat masing masing 20 gambar dengan ukuran gambar sesuai dengan (t%1000)+100, dimana t adalah epoch time. Sehingga hanya perlu mendaptakan epoch time dengan fungsi time() lalu diimplementasikan (t%1000)+100 dan hasilnya dijadikan slug untuk link downloadnya. lalu untuk mendownload menggunakan execv dengan perintah wget dan disimpan pada folder yang telah dibuat.
````
unsigned long get_time = (unsigned long)time(NULL);
get_time = (get_time%1000) + 100;

char link[50];
sprintf(link, "https://picsum.photos/%lu", get_time);

time_t filet;
struct tm * fileti;
char files [80] = "";
char down_dir[100];

time (&filet);
fileti = localtime (&filet);

strftime (files,80,"%Y-%m-%d_%H:%M:%S",fileti);
sprintf(down_dir, "%s/%s", buffer, files);
// puts(down_dir);


char *argv[] = {"wget", "-O", down_dir, link, NULL};
execv("/usr/bin/wget", argv);
````
Setelah gambar didownload sejumlah 20 maka folder dizip dan foldernya dihapus. Maka dapat menggunakan execv dengan menjalankan program zip dan diberi parameter -rm.
````
char zip_name[100];
sprintf(zip_name, "%s.zip", buffer);

char *argv[] = {"zip", "-rm", zip_name, buffer, NULL};
execv("/usr/bin/zip", argv);
````
Lalu program diminta dapat menggenerate sebuah executable program yang dapat menghentikan program utama. Untuk membuatnya tinggal membuat sebuah file .c dengan perintah killall atau kill pid (menyesuaikan argumen nantinya) dengan fungsi fopen() dan menginputkan isi dari file dengan fputs(). Lalu setelah dibuat file .c nya maka selanjutnya menjalankan execv untuk mengcompile file .c tersebut lalu menghapus filr .c nya.
````
 sprintf(data,   "#include <stdlib.h>\n"
		"#include <sys/types.h>\n"
		"#include <unistd.h>\n"
		"#include <stdio.h>\n"
		"#include <wait.h>\n"
		"#include <string.h>\n"

		" int main() {"
		" pid_t child_id;"
		" int status;"

		" child_id = fork();"
		" if (child_id < 0) exit(EXIT_FAILURE);"

		" if (child_id == 0) {"
		    " %s"
		    " execv(\"/usr/bin/killall\", argv);"
		" } else {"
		    " while (wait(&status)>0);"
		" }"
		" }", mode);

ptr = fopen(files, "w");
fputs(data, ptr);
fclose(ptr);

char *argv[] = {"gcc", src, "-o", dst, NULL};
execv("/usr/bin/gcc", argv);
}

while (wait(&status)>0);

child_id2 = fork();
//     printf("%d\n", child_id2);
if (child_id2 < 0) exit(EXIT_FAILURE);

if (child_id2 == 0) {
char src[100];

sprintf(src, "%s/killer.c", cwd);
char *argv[] = {"rm", src, NULL};
execv("/bin/rm", argv);
}
````
Lalu program juga diminta untuk dapat menjalan kan 2 mode dengan ditentukan dari argumen yang di passing, jika -a maka saat file killer dijalankan pogram akan langsung mati, namun jika -b program harus menyelesaikan satu proses lalu mati. Sehingga perlu adanya pengecekkan terkait argumen yang diterima serta dibuat perbedaan proses execv pada file killer dengan -a menggunakan killall dan -b menggunakan kill pid.
````
if(argc != 2 || argv[1][1] != 'a' && argv[1][1] != 'b'){
        printf("argumen only -a or -b");
        return 0;
    }

...

if (argv[1][1] == 'a') strcpy(mode, " char *argv[] = {\"killall\", \"-s\", \"9\", \"soal2\", NULL};");
else if (argv[1][1] == 'b') sprintf(mode, " char *argv[] = {\"kill\", \"%d\", NULL};", pid2);

````

## SOAL 3

Jaya adalah seorang programmer handal mahasiswa informatika. Suatu hari dia
memperoleh tugas yang banyak dan berbeda tetapi harus dikerjakan secara bersamaan
(multiprocessing).
  1.  Program buatan jaya harus bisa membuat dua direktori di
      “/home/[USER]/modul2/”. Direktori yang pertama diberi nama “indomie”, lalu
      lima detik kemudian membuat direktori yang kedua bernama “sedaap”.
  2.  Kemudian program tersebut harus meng-ekstrak file jpg.zip di direktori
      “/home/[USER]/modul2/”. Setelah tugas sebelumnya selesai, ternyata tidak
      hanya itu tugasnya.
  3.  Diberilah tugas baru yaitu setelah di ekstrak, hasil dari ekstrakan tersebut (di
      dalam direktori “home/[USER]/modul2/jpg/”) harus dipindahkan sesuai dengan
      pengelompokan, semua file harus dipindahkan ke
      “/home/[USER]/modul2/sedaap/” dan semua direktori harus dipindahkan ke
      “/home/[USER]/modul2/indomie/”.
  4.  Untuk setiap direktori yang dipindahkan ke “/home/[USER]/modul2/indomie/”
      harus membuat dua file kosong. File yang pertama diberi nama “coba1.txt”, lalu
      3 detik kemudian membuat file bernama “coba2.txt”.
      (contoh : “/home/[USER]/modul2/indomie/{nama_folder}/coba1.txt”).
      
Karena Jaya terlalu banyak tugas dia jadi stress, jadi bantulah Jaya agar bisa membuat
program tersebut.

Catatan :
- Tidak boleh memakai system().
- Tidak boleh memakai function C mkdir() ataupun rename().
- Gunakan exec dan fork.
- Direktori “.” dan “..” tidak termasuk.

### Penyelesaian

Berikut ini merupakan kode C untuk menyelesaikan no.3
````
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <wait.h>
#include <string.h>
#include <dirent.h>

// fungsi untuk buat direktori
void createDir(char name[]){
    char dir[50] = "/home/hexa/modul2/";
    strcat(dir, name);

    char *argv[] = {"mkdir", "-p", dir, NULL};
    execv("/bin/mkdir", argv);
}

// fungsi untuk extract file zip
void extractZip(char fileName[]){
    char dir[50] = "/home/hexa/modul2/";
    char temp[50] = "/home/hexa/modul2/";
    strcat(temp, fileName); //menggabungkan string

    char *argv[] = {"unzip", temp, "-d", dir, NULL};
    execv("/usr/bin/unzip", argv);
}

// fungsi untuk memindahkan file atau folder
void move(int isFolder, char name[]){
    char src[50] = "/home/hexa/modul2/jpg/"; 
    char dst[50];
    
    if(isFolder)strcpy(dst, "/home/hexa/modul2/indomie/");
    else strcpy(dst, "/home/hexa/modul2/sedaap/");
    
    strcat(src, name);

    char *argv[] = {"mv", src, dst, NULL};
    execv("/bin/mv", argv);
}

int main() {
    pid_t child_id;
    int status;

    //buat proses
    child_id = fork();
    if (child_id < 0) {
        exit(EXIT_FAILURE);
    }

    if (child_id == 0) {

        pid_t child2_id;
        int status2;

        // buat proses dari proses child 1
        child2_id = fork();
        if (child2_id < 0) {
            exit(EXIT_FAILURE);
        }

        if (child2_id == 0) {

            pid_t child3_id;
            int status3;

            //buat proses dari proses child 2
            child3_id = fork();
            if (child3_id < 0) {
                exit(EXIT_FAILURE);
            }

            if (child3_id == 0) {
                createDir("indomie"); //membuat direktori indomie
            } else {
                while (wait(&status3)>0);
                sleep(5);           // delay 5 detik
                createDir("sedaap"); //membuat direktori sedaap
            }

        } else {
            while (wait(&status2)>0);
            extractZip("jpg.zip"); //extract file jpg.zip
        }

    } else {
        while (wait(&status)>0);

        int file_count = 0;
        DIR * dirp;
        struct dirent * entry;

        //membuka isi dari direktori jpg lalu dilakukan loop sejumlah item yang ada didalamnya
        dirp = opendir("/home/hexa/modul2/jpg/"); 
        while ((entry = readdir(dirp)) != NULL) {
            if(fork() == 0){
                if (entry->d_type == DT_REG) { //dicek jika item adalah sebuah file
                file_count++;
                move(0, entry->d_name);  //pindah file ke folder indomie

            }
            else if (entry->d_type == DT_DIR && entry->d_name[0] != '.') { 
	    	//cek jika item adalah sebuah folder yang bukan . dan .. 
                file_count++;
                move(1, entry->d_name); //pindah ke folder sedaap
            }
            exit(0);
            }
        }
        for(int i=0;i<file_count;i++) //wait semua proses child diatas
        wait(NULL);
        closedir(dirp);
        sleep(10); //delay 10 detik

        char dir[300];
        FILE * ptr;
	//membuka isi dari direktori indomie lalu dilakukan loop sejumlah item yang ada didalamnya
        dirp = opendir("/home/hexa/modul2/indomie/"); 
	
        while ((entry = readdir(dirp)) != NULL) {
            if (entry->d_name[0] != '.') { // dicek apakah item bukan folder . dan ..
                //buat file coba1.txt pada setiap item
                FILE * ptr;
                sprintf(dir, "/home/hexa/modul2/indomie/%s/coba1.txt", entry->d_name);
                puts(dir);
                ptr = fopen(dir, "w");
                fclose(ptr);

                sleep(3); //delay 3 detik

                //buat file coba2.txt pada setiap item
                sprintf(dir, "/home/hexa/modul2/indomie/%s/coba2.txt", entry->d_name);
                ptr = fopen(dir, "w");
                fclose(ptr);
            }
        }
        closedir(dirp);
    }
}
````

Pertama - tama kita diminta untuk membuat dua buah direktori dengan nama `indomie` dan `sedaap` 
maka kita membuat sebuah fungsi untuk membuat direktori tersebut yaitu `void createDir()`.
````
void createDir(char name[]){
    char dir[50] = "/home/hexa/modul2/";
    strcat(dir, name);

    char *argv[] = {"mkdir", "-p", dir, NULL};
    execv("/bin/mkdir", argv);
}
````

Selanjutnya kita diminta untuk meng-esktrak file, maka kita membuat sebuah fungsi 
untuk meng-ekstrak file tersebut yaitu `void extractZip()`.
````
void extractZip(char fileName[]){
    char dir[50] = "/home/hexa/modul2/";
    char temp[50] = "/home/hexa/modul2/";
    strcat(temp, fileName); //menggabungkan string

    char *argv[] = {"unzip", temp, "-d", dir, NULL};
    execv("/usr/bin/unzip", argv);
}
````

Setelah meng-ekstrak file, kita diminta untuk memindahkan file dengan pengelompokan dimana semua file harus dipindahkan ke
“/home/[USER]/modul2/sedaap/” dan semua direktori harus dipindahkan ke “/home/[USER]/modul2/indomie/”. Untuk mengatasi hal tersebut,
maka dibuat fungsi untuk memindahkan yaitu `void move()`.
```
void move(int isFolder, char name[]){
    char src[50] = "/home/hexa/modul2/jpg/"; 
    char dst[50];
    
    if(isFolder)strcpy(dst, "/home/hexa/modul2/indomie/");
    else strcpy(dst, "/home/hexa/modul2/sedaap/");
    
    strcat(src, name);

    char *argv[] = {"mv", src, dst, NULL};
    execv("/bin/mv", argv);
}
````

Lalu kita dimita untuk setiap direktori yang dipindahkan ke “/home/[USER]/modul2/indomie/” harus membuat dua file kosong. 
File yang pertama diberi nama “coba1.txt”, lalu 3 detik kemudian membuat file bernama “coba2.txt”. Maka,
````
if (entry->d_name[0] != '.') { // dicek apakah item bukan folder . dan ..
        //buat file coba1.txt pada setiap item
        FILE * ptr;
        sprintf(dir, "/home/hexa/modul2/indomie/%s/coba1.txt", entry->d_name);
        puts(dir);
        ptr = fopen(dir, "w");
        fclose(ptr);

        sleep(3); //delay 3 detik

        //buat file coba2.txt pada setiap item
        sprintf(dir, "/home/hexa/modul2/indomie/%s/coba2.txt", entry->d_name);
        ptr = fopen(dir, "w");
        fclose(ptr);
}
````

Untuk melakukan proses semua yang diatas, maka kita perlu membuat 3 proses anak dimana 
  1. Anak ketiga akan melakukan proses membuat direktori	(menunggu proses anak kedua)
  2. Anak kedua akan melakukan proses meng-ekstrak file 	(menunggu proses anak ketiga)
  3. Anak pertama akan melakukan pemindahan secara pengelompokan

Parent akan menunggu semua proses child, apabila semua proses child selesai maka parent akan memindahkan setiap direktori ke “/home/[USER]/modul2/indomie/” serta membuat dua file kosong. File yang pertama diberi nama “coba1.txt”, lalu 3 detik kemudian 
membuat file bernama “coba2.txt”.
