#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shellmemory.h"
#include "shell.h"
#include <dirent.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>



int MAX_ARGS_SIZE = 3;

int badcommand() {
    printf("Unknown Command\n");
    return 1;
}

// For source command only
int badcommandFileDoesNotExist() {
    printf("Bad command: File not found\n");
    return 3;
}

int help();
int quit();
int set(char *var, char *value);
int print(char *var);
int source(char *script);
int badcommandFileDoesNotExist();
int echo(char *input);
int my_ls();
int my_mkdir(char *dirname);
int my_touch(char *filename);
int my_cd(const char *path);
int run(char *command, char *variables[]);

// Interpret commands and their arguments
int interpreter(char *command_args[], int args_size) {
    int i;

    if (args_size < 1 || args_size > MAX_ARGS_SIZE) {
        return badcommand();
    }

    for (i = 0; i < args_size; i++) {   // terminate args at newlines
        command_args[i][strcspn(command_args[i], "\r\n")] = 0;
    }

    if (strcmp(command_args[0], "help") == 0) {
        //help
        if (args_size != 1)
            return badcommand();
        return help();

    } else if (strcmp(command_args[0], "quit") == 0) {
        //quit
        if (args_size != 1)
            return badcommand();
        return quit();

    } else if (strcmp(command_args[0], "set") == 0) {
        //set
        if (args_size != 3)
            return badcommand();
        return set(command_args[1], command_args[2]);

    } else if (strcmp(command_args[0], "print") == 0) {
        if (args_size != 2)
            return badcommand();
        return print(command_args[1]);

    } else if (strcmp(command_args[0], "source") == 0) {
        if (args_size != 2)
            return badcommand();
        return source(command_args[1]);
    } else if (strcmp(command_args[0], "my_ls") == 0) {
	if (args_size != 1)
	    return badcommand();
	return my_ls();

    } else if (strcmp(command_args[0], "my_mkdir") == 0) {
	if (args_size != 2)
		return badcommand();
	return my_mkdir(command_args[1]);

    } else if (strcmp(command_args[0], "echo") == 0) {
	if (args_size != 2)
	    return badcommand();
	return echo(command_args[1]);
    } else if (strcmp(command_args[0], "my_touch") == 0) {
	if (args_size != 2)
	    return badcommand();
	return my_touch(command_args[1]);
    } else if (strcmp(command_args[0], "my_cd") == 0) {
	if (args_size != 2)
	    return badcommand();
	return my_cd(command_args[1]);
    } else if (strcmp(command_args[0], "run") == 0) {
      if (args_size < 2) return badcommand();
      command_args[args_size] = NULL; 
      return run(command_args[1], &command_args[1]);  
    } else
        return badcommand();
}

int my_cd(const char *path) {
	if (chdir(path) == -1) {
        printf("Bad command: my_cd\n");
        return 1;
    }
    return 0;


}


int run(char *command, char *variables[]) {
	pid_t pid = fork();
	if (pid == 0) {
	execvp(command, variables);
	exit(1);
	}
	else {
	int status;
        waitpid(pid, &status, 0);
        return 0;
}
}




int my_touch(char *filename) {
	FILE *newfile = fopen(filename, "w");
	fclose(newfile);
	return 0;

}

void swap(char** xp, char** yp){
    char *temp = *xp;
    *xp = *yp;
    *yp = temp;
}

int my_ls() {
	DIR *current_directory = opendir("."); 
	if (current_directory == NULL) {perror("Cannot open");
		exit(1);}
	
	struct dirent *file_name;
	char *names[1024];
	int count = 0;
	while ((file_name = readdir(current_directory)) != NULL) {
		names[count++] = file_name->d_name;
}
	int i, j;
   	bool swapped;
    	for (i = 0; i < count - 1; i++) {
        	swapped = false;
        	for (j = 0; j < count - i - 1; j++) {
            		if (strcmp(names[j], names[j + 1]) > 0) {
    				swap(&names[j], &names[j + 1]);
    				swapped = true;

            }
        }

        // If no two elements were swapped by inner loop,
        // then break
        if (swapped == false)
            break;
    }
        for (int k = 0; k < count; k++) {
              printf("%s\n", names[k]);
}
	closedir(current_directory);
	return 0;
}


int my_mkdir(char *dirname) {
	if (dirname[0] == '$') {
		char *variable = dirname + 1;
		if (strcmp(mem_get_value(variable), "Variable does not exist") == 0) {printf("Bad command: my_mkdir");}
		else {
			mkdir(mem_get_value(variable), 0755);
}}
	else {
		mkdir(dirname, 0755);
}
	return 0;

}


int help() {

    // note the literal tab characters here for alignment
    char help_string[] = "COMMAND			DESCRIPTION\n \
help			Displays all the commands\n \
quit			Exits / terminates the shell with “Bye!”\n \
set VAR STRING		Assigns a value to shell memory\n \
print VAR		Displays the STRING assigned to VAR\n \
source SCRIPT.TXT	Executes the file SCRIPT.TXT\n ";
    printf("%s\n", help_string);
    return 0;
}

int quit() {
    printf("Bye!\n");
    exit(0);
}

int set(char *var, char *value) {
    // Challenge: allow setting VAR to the rest of the input line,
    // possibly including spaces.

    // Hint: Since "value" might contain multiple tokens, you'll need to loop
    // through them, concatenate each token to the buffer, and handle spacing
    // appropriately. Investigate how `strcat` works and how you can use it
    // effectively here.

    mem_set_value(var, value);
    return 0;
}


int print(char *var) {
    printf("%s\n", mem_get_value(var));
    return 0;
}

int echo(char *input) {
// Basically, it first checks if there is a $ if there is not prints as it is.
//But if there is it first disreagrds the $ character and take the rest as a variable which then it looks to memory for the value
    if (input[0] == '$') {

	char *variable = input + 1;
	if (strcmp(mem_get_value(variable), "Variable does not exist") == 0) {
		printf("\n");
}	else {
		printf("%s\n", mem_get_value(variable));
}}
    else {
	printf("%s\n", input);
}
    return 0;
}

int source(char *script) {
    int errCode = 0;
    char line[MAX_USER_INPUT];
    FILE *p = fopen(script, "rt");      // the program is in a file

    if (p == NULL) {
        return badcommandFileDoesNotExist();
    }

    fgets(line, MAX_USER_INPUT - 1, p);
    while (1) {
        errCode = parseInput(line);     // which calls interpreter()
        memset(line, 0, sizeof(line));

        if (feof(p)) {
            break;
        }
        fgets(line, MAX_USER_INPUT - 1, p);
    }

    fclose(p);

    return errCode;
}
