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

void createKiller(pid_t pid, char param){
    pid_t child_id, child_id2;
    char cwd[100];
    getcwd(cwd, sizeof(cwd));
    int status;
    int test;

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

        if (param == 'a') strcpy(mode, " char *argv[] = {\"killall\", \"-s\", \"9\", \"soal2\", NULL}; execv(\"/usr/bin/killall\", argv);");
        else if (param == 'b') sprintf(mode, " char *argv[] = {\"kill\", \"%d\", NULL}; execv(\"/bin/kill\", argv);", pid);
        
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
                        " } else {"
                            " while (wait(&status)>0);"
                            " char *argv[] = {\"rm\", \"%s/killer\", NULL};"
                            " execv(\"/bin/rm\", argv);"
                        " }"
                        " }", mode, cwd);

        ptr = fopen(files, "w");
        fputs(data, ptr);
        fclose(ptr);

        char *argv[] = {"gcc", src, "-o", dst, NULL};
        execv("/usr/bin/gcc", argv);
    }
    
    while (wait(&status)>0);

    child_id2 = fork();
    // printf("%d\n", child_id2);
    if (child_id2 < 0) exit(EXIT_FAILURE);

    if (child_id2 == 0) {
        char src[100];

        sprintf(src, "%s/killer.c", cwd);
        char *argv[] = {"rm", src, NULL};
        execv("/bin/rm", argv);
    }
    
    while (wait(&status)>0);
}

int main(int argc, char *argv[]) {
    if(argc != 2 || argv[1][1] != 'a' && argv[1][1] != 'b'){
        printf("argumen only -a or -b");
        return 0;
    }

    char cwd[100];
    getcwd(cwd, sizeof(cwd));

    pid_t pid, sid;    

    pid = fork(); 
    // printf("%d\n", pid);

    if (pid < 0) {
        // printf("3 %d\n", getpid());
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        // printf("2 %d\n", getpid());
        exit(EXIT_SUCCESS);
    }


    createKiller(getpid(), argv[1][1]);
    // printf("1 %d\n", getpid());

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
    
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [80];
    char pathDir [100];

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    strftime (buffer,80,"%Y-%m-%d_%H:%M:%S",timeinfo);
    sprintf(pathDir, "%s/%s", cwd, buffer);

    
    if(fork()==0){
      int status1, i;
      if(fork()==0){
        char *argu[] = {"mkdir", "-p", pathDir, NULL};
        execv("/bin/mkdir", argu);
      }
      while ((wait(&status1)) > 0);
      for(i=0 ; i<=20 ; i++){
        // dirname2=dirname;
        if(i==20){
          int status2;
          if(fork()==0){
            char zip_name[100];
            sprintf(zip_name, "%s.zip", buffer);

            char *argv[] = {"zip", "-rm", zip_name, pathDir, NULL};
            execv("/usr/bin/zip", argv);
          }
          else{
            while ((wait(&status2)) > 0);
          }
        }

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

        if(fork()==0){
          char *argv[] = {"wget", "-O", down_dir, link, NULL};
          execv("/usr/bin/wget", argv);
        }
        sleep(5);
      }
    }
    sleep(30);
  }
}