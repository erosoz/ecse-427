#define MEM_SIZE 1000
void mem_init();
char *mem_get_value(char *var);
void mem_set_value(char *var, char *value);
int frame_read_store(char *filename, int start);
extern int frame_store_count;
extern char *frame_store[1000];
