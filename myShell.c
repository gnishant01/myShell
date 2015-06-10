/*
A simple shell which works with most of the unix commands.
operators such as piping, redirection are not working currently.
The functions are defined as per the order of their call
*/

#include <stdio.h>  /* sprintf, printf, FILE, stdout, EOF, NULL*/
#include <stdlib.h>  /* malloc, free, exit, */
#include <unistd.h>  /* fork */
#include <signal.h>  /* SIGINT*/
#include <string.h>  /* strlen, strcpy, strcat, strncat, strstr, memset*/
#include <sys/wait.h>
#include <sys/types.h> 
#include <ctype.h>
#include <sys/stat.h>
#include <fcntl.h>

char *myArguments[100], *myEnvParams[100];
char *searchPath[10];


/* function to generate a prompt of the form '<userName>@<hostName>@myShell:'.
This works on some of the machines, but doesn't work on some others */
void printPrompt(){
	FILE *fp;
	char path[100], userName[100], hostName[100], pwd[100];

	fp = popen("whoami", "r");
	fgets(userName, sizeof(userName) - 1, fp);
	userName[strlen(userName) - 1] = '\0';
	strcpy(path, userName);

	strcat(path, "@");

	fp = popen("hostname", "r");
	fgets(hostName, sizeof(hostName) - 1, fp);
	hostName[strlen(hostName) - 1] = '\0';
	strcat(path, hostName);
	strcat(path, "@myShell");
	strcat(path, ":");

	fp = popen("pwd", "r");
	fgets(pwd, sizeof(pwd) - 1, fp);
	pwd[strlen(pwd) - 1] = '\0';
	char toSearch[200] = "/home/";
	strcat(toSearch, userName);
	char *t = strstr(pwd, toSearch);
	if(t != NULL) strcat(path, "~");
	strcat(path, t + strlen(toSearch));

	fprintf(stderr, "%s$ ", path);
	pclose(fp);
}

/* function to handle signals such as ctrl+C, ctrl+D */
void handleSignal(int signo){
	printf("\n");
	// printPrompt(); /* uncomment this line and comment the next line to change the prompt to '<userName>@<hostName>@myShell:' */
	printf("[myShell]: ");
	fflush(stdout);
}

/* get the environmental parameters and copy them in the string myEnvParams */
void copyEnvp(char *envp[]){
	int i;
	for(i = 0; envp[i] != NULL; i++){
		myEnvParams[i] = (char *)malloc(sizeof(char) * (strlen(envp[i]) + 1));
		strcpy(myEnvParams[i], envp[i]);
	}
}

/* search for Environmental variable paths and store it in string binPath */
void getPath(char *tempEnvParams[], char *binPath){
	int count = 0;
	char *tmp;
	while(1){
		tmp = strstr(tempEnvParams[count], "PATH");
		if(tmp == NULL){
			count++;
		}
		else{
			if(strstr(tempEnvParams[count], "DEFAULTS_PATH") == NULL) break;
			else count++;
		}
	}
    strcpy(binPath, tmp);
}

/* Store all the 'paths' and store them in the array of strings searchPath */
void insertPath(char *pathString){
	int i = 0;
	char *tmp = pathString;
	char ret[100];

	while(*tmp != '=') tmp++;
	tmp++;

	while(*tmp != '\0'){
		if(*tmp == ':'){
			strcat(ret, "/");
			searchPath[i] = (char *) malloc(sizeof(char) * (strlen(ret) + 1));
			strcat(searchPath[i], ret);
			strcat(searchPath[i], "\0");
			i++;
			memset(ret, '\0', 100);
		}
		else{
			sprintf(ret, "%s%c", ret, *tmp);			
		}
		tmp++;
	}
	strcat(ret, "/");
	searchPath[i] = (char *) malloc(sizeof(char) * (strlen(ret) + 1));
	strcat(searchPath[i], ret);
	strcat(searchPath[i], "\0");
	i++;
}

/* Store the options given with the command in the array of strings myArguments */
void fillArgv(char *tmp_argv){
	char *foo = tmp_argv;
	int i = 0;
	char ret[100];
	memset(ret, '\0', 100);
	while(*foo != '\0'){
		if(i == 10)
			break;

		if(*foo == ' '){
			if(myArguments[i] == NULL){
				myArguments[i] = (char *)malloc(sizeof(char) * strlen(ret) + 1);
			}
			else {
				memset(myArguments[i], '\0', strlen(myArguments[i]));
			}
			strcpy(myArguments[i], ret);
			strcat(myArguments[i], "\0");
			memset(ret, '\0', 100);
			i++;
		}
		else{
			strncat(ret, foo, 1);
		}
		foo++;
	}
	myArguments[i] = (char *)malloc(sizeof(char) * strlen(ret) + 1);
	strcpy(myArguments[i], ret);
	strcat(myArguments[i], "\0");
}

/* Search the path of the command and append it to string(char pointer) cmd*/
int attachPath(char *cmd){
	char ret[100];
	int i;
	int fd;
	memset(ret, '\0', 100);
	for(i = 0; searchPath[i] != NULL; i++){
		strcpy(ret, searchPath[i]);
		strcat(ret, cmd);
		if((fd = open(ret, O_RDONLY)) > 0) {
			strcpy(cmd, ret);
			close(fd);
			return 0;
		}
	}
	return 0;
}

/* Execute the command by creating a new process*/
void executeCommand(char *cmd){
	int i;
	if(fork() == 0){
		i = execve(cmd, myArguments, myEnvParams);
		if(i < 0){
			printf("%s: %s\n", cmd, "command not found");
			exit(1);		
		}
	}
	else{
		wait(NULL);
	}
}

/* free the memory before ending the program */
void freeMemory(){
	int i;
	for(i = 0; myArguments[i] != NULL; i++){
		memset(myArguments[i], '\0', strlen(myArguments[i]) + 1);
		myArguments[i] = NULL;
		free(myArguments[i]);
	}
}

int main(int argc, char *argv[], char *envp[]){
	char c;
	int i, fd;
	char *tmp = (char *)malloc(sizeof(char) * 100);
	char *pathString = (char *)malloc(sizeof(char) * 256);
	char *cmd = (char *)malloc(sizeof(char) * 100);
	
	signal(SIGINT, handleSignal);

	copyEnvp(envp);
	getPath(myEnvParams, pathString);
	insertPath(pathString);

	if(fork() == 0){
		execve("/usr/bin/clear", argv, myEnvParams);
		exit(1);
	}
	else{
		wait(NULL);
	}
	// printPrompt(); /* uncomment this line and comment the next line to change the prompt to '<userName>@<hostName>@myShell:' */
	printf("[myShell]: ");
	fflush(stdout);
	while(c != EOF){
		c = getchar();
		switch(c){
		case '\n':
			if(tmp[0] == '\0'){
				// printPrompt(); /* uncomment this line and comment the next line to change the prompt to '<userName>@<hostName>@myShell:' */
				printf("[myShell]: ");
			}
			else{
				fillArgv(tmp);
				strcpy(cmd, myArguments[0]);

				strcat(cmd, "\0");
				if(index(cmd, '/') == NULL){
					if(attachPath(cmd) == 0){
						executeCommand(cmd);
					}
					else{
						printf("%s: command not found\n", cmd);
					}
				}
				else{
					if((fd = open(cmd, O_RDONLY)) > 0){
						close(fd);
						executeCommand(cmd);
					}
					else{
						printf("%s: command not found\n", cmd);
					}
				}
				freeMemory();
				// printPrompt(); /* uncomment this line and comment the next line to change the prompt to '<userName>@<hostName>@myShell:' */
				printf("[myShell]: ");
				memset(cmd, '\0', 100);
			}
			memset(tmp, '\0', 100);
			break;
		default:
			strncat(tmp, &c, 1);
			break;
		}
	}
	free(tmp);
	free(pathString);
	for(i = 0; myEnvParams[i] != NULL; i++) free(myEnvParams[i]);
	for(i = 0; i < 10; i++) free(searchPath[i]);
	printf("\n");
	return 0;
}