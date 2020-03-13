#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <wait.h>
#include <string.h>
#include <dirent.h>
#include <time.h>   

int main() {
    while(1){
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
            for(int i=0;i<20;i++) // loop will run n times (n=5) 
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
            for(int i=0;i<20;i++) // loop will run n times (n=5) 
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