#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>

int main(int argc, char** argv) {
    if(argc != 3) {
        fprintf(stderr, "error, usage: %s src dest\n", argv[0]);
        exit(1);
    }

    int input = open(argv[1], O_RDONLY);
    if(input == -1) {
        perror("open src failed");
        exit(1);
    }

    int output = open(
        argv[2],
        O_CREAT | O_WRONLY | O_TRUNC,
        S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH
    );

    if(output == -1) {
        perror("open dest failed");
        exit(1);
    }

    int n;
    char buffer[1024];
    while((n = read(input, buffer, 1024)) > 0) {
        if(write(output, buffer, n) != n) {
            fprintf(stderr, "write failed\n");
            exit(1);
        }
    }

    if(n == -1) {
        perror("read src failed");
        exit(1);
    }

    if(close(input) == -1) {
        perror("close src failed");
        exit(1);
    }

    if(close(output) == -1) {
        perror("close dest failed");
        exit(1);
    }

    return 0;
}
