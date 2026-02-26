#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "shellmemory.h"
int frame_store_count = 0;

struct memory_struct {
    char *var;
    char *value;
};

char *frame_store[1000];


struct memory_struct shellmemory[MEM_SIZE];

// Helper functions
int match(char *model, char *var) {
    int i, len = strlen(var), matchCount = 0;
    for (i = 0; i < len; i++) {
        if (model[i] == var[i])
            matchCount++;
    }
    if (matchCount == len) {
        return 1;
    } else
        return 0;
}

// Shell memory functions

void mem_init() {
    int i;
    for (i = 0; i < MEM_SIZE; i++) {
        shellmemory[i].var = "none";
        shellmemory[i].value = "none";
	frame_store[i] = NULL;
    }
}

// Set key value pair
void mem_set_value(char *var_in, char *value_in) {
    int i;

    for (i = 0; i < MEM_SIZE; i++) {
        if (strcmp(shellmemory[i].var, var_in) == 0) {
            shellmemory[i].value = strdup(value_in);
            return;
        }
    }

    //Value does not exist, need to find a free spot.
    for (i = 0; i < MEM_SIZE; i++) {
        if (strcmp(shellmemory[i].var, "none") == 0) {
            shellmemory[i].var = strdup(var_in);
            shellmemory[i].value = strdup(value_in);
            return;
        }
    }

    return;
}

//get value based on input key
char *mem_get_value(char *var_in) {
    int i;

    for (i = 0; i < MEM_SIZE; i++) {
        if (strcmp(shellmemory[i].var, var_in) == 0) {
            return strdup(shellmemory[i].value);
        }
    }
    return NULL;
}

int frame_read_store(char *filename, int start) {
	FILE *f = fopen(filename, "r");
	if (f == NULL) return -1;
	
	char line[100];
	int index = start;
	while (fgets(line, 100, f) != NULL) {
    	line[strcspn(line, "\r\n")] = 0;  // strip newline
    	frame_store[index] = strdup(line);
    	index++;
}

	fclose(f);
	
	return index - start;
}

