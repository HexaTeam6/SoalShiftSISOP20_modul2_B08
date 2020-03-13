 
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
            else if (entry->d_type == DT_DIR && entry->d_name[0] != '.') { //cek jika item adalah sebuah folder yang bukan . dan .. 
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
        dirp = opendir("/home/hexa/modul2/indomie/"); //membuka isi dari direktori indomie lalu dilakukan loop sejumlah item yang ada didalamnya
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