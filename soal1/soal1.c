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

int main(int argc, char *argv[]) {
	if(argc > 5) {
    	printf("Error\n");
  	}
  	else {
  		if(strlen(argv[1]) > 2 || strlen(argv[2]) > 2 || strlen(argv[3]) > 2) {
  			printf("Error\n");
		}
		else {
			int error = 0, set = 0;
			
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
