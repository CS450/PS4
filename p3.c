/*
When file is opened for the first time and read it works perfectly.
When unlink() is called the file is deleted and because the parent waits for the child to finish all subsequent 
operations of read, and write fail because the file as already been deleted.  If they had been done in parallel
the unlink() may not have been completed because another thread could have had it open and been reading/writing to it.
*/
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1000

int main(int argc, char *argv[]) {
    FILE *f = fopen(argv[1], "r+");

    char buffer[BUFFER_SIZE];
    printf("Reading file:\n");
    while(fgets(buffer, BUFFER_SIZE - 1, f) != NULL) {
        printf("%s\n", buffer);
    }

    //fork the child
    pid_t pid;
    pid = fork();
    if (pid != 0) {
        printf("<Unlinking file>\n");
        unlink(argv[1]);
    }
    else {
        int status;
        waitpid(pid, &status, 0);
        char buffer2[BUFFER_SIZE];
        printf("Reading file:\n");
        while(fgets(buffer2, BUFFER_SIZE - 1, f) != NULL) {
            printf("%s\n", buffer2);
        }
        printf("Writing 'test' to file:\n");
        fprintf (f, "test %d\n", 1);
        printf("Reading file:\n");
    }
    fclose(f);
    return 0;
}