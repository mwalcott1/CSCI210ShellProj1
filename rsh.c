#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12

extern char **environ;

char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};

int isAllowed(const char*cmd) {
	// TODO
	
	for(int i = 0; i < 12; i++){
		if(!strcmp(cmd, allowed[i])){
			return 1;
		}
	}
	// return 1 if cmd is one of the allowed commands
	// return 0 otherwise
	
	return 0;
}

int main() {

    // TODO
    // Add variables as needed

    char line[256];

    while (1) {

	fprintf(stderr,"rsh>");

	if (fgets(line,256,stdin)==NULL) continue;

	if (strcmp(line,"\n")==0) continue;

	line[strlen(line)-1]='\0';

	// TODO
	// Add code to spawn processes for the first 9 commands
	
	char* rootCom;
	rootCom = strtok(line, " ");
	if(!isAllowed(rootCom)){
		printf("NOT ALLOWED!\n");
		continue;
	}
	if(!strcmp(rootCom, "exit")){
		break;
	}
	if(!strcmp(rootCom, "help")){
		printf("The allowed commands are:\n1:cp\n2:touch\n3:mkdir\n4:ls\n5:pwd\n6:cat\n7:grep\n8:chmod\n9:diff\n10:cd\n11:exit\n12:help\n");
		continue;
	}

	
	//if we get a non-cd/exit/help command, we'll spawn it
	
	char* argv[20];
	argv[0] = rootCom;
	
	char* nextArg;
	nextArg = strtok(NULL, " ");
	int i = 1;

	while(nextArg != NULL){
		argv[i] = nextArg;
		nextArg = strtok(NULL, " ");
		i++;
	}

	argv[i] = NULL;
	

	// do cd
	if(!strcmp(rootCom, "cd")){
		
		char* targetDir;

		if(argv[1] == NULL){
			targetDir = getenv("HOME");
		}
		else if(argv[2] == NULL){
			targetDir = argv[1];
		}
		else{
			printf("-rsh: cd: too many arguments");
			continue;
		}
		chdir(targetDir);
		continue;
	}
	// end cd
	
	//spawn new process (not entirely clear on how this works)
	int cpid;
        int status;
    	posix_spawnattr_t attr;

    	// Initialize spawn attributes
    	posix_spawnattr_init(&attr);

    	// Set flags if needed, for example, to specify the scheduling policy
    	// posix_spawnattr_setflags(&attr, POSIX_SPAWN_SETSCHEDULER);

    	// Spawn a new process
    	if (posix_spawnp(&cpid, rootCom, NULL, &attr, argv, environ) != 0) {
        	perror("spawn failed");
        	exit(EXIT_FAILURE);
   	}

 	// Wait for the spawned process to terminate
    	if (waitpid(cpid, &status, 0) == -1) {
        	perror("waitpid failed");
        	exit(EXIT_FAILURE);
 	}

	if (WIFEXITED(status)) {
        	//printf("Spawned process exited with status %d\n", WEXITSTATUS(status));
 	}

    	// Destroy spawn attributes
    	posix_spawnattr_destroy(&attr);
	//end spawn new process

	
	// And add code to execute cd, exit, help commands
	// Use the example provided in myspawn.c

    }
    return 0;
}
