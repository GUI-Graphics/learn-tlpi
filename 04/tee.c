#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>

static const int MAX_FILES = 10;
char errMsg[1024];

int main(int argc, char** argv) {
    int opt;
    int aflag = 0;

    while((opt = getopt(argc, argv, "a")) != -1) {
        if(opt != '?') aflag = 1;
        else {
            fprintf(stderr, "usage: %s [-a] [file...]\n", argv[0]);
            exit(1);
        }
    }

    int cnt;
    const char* filenames[MAX_FILES];

    for(cnt = 0; optind < argc; ++optind) {
        filenames[cnt++] = argv[optind];
        if(cnt == 10) break;
    }

    int i;
    int outfiles[cnt];

    int flag = O_CREAT | O_WRONLY;
    if(aflag) flag |= O_APPEND;

    for(i = 0; i < cnt; ++i) {
        outfiles[i] = open(
            filenames[i],
            flag,
            S_IWUSR | S_IRUSR | S_IRGRP
        );

        if(outfiles[i] == -1) {
            sprintf(errMsg, "open %s failed", filenames[i]);
            perror(errMsg);
            exit(1);
        }
    }

    int bytes;
    char buffer[1024];

    while((bytes = read(0, buffer, 1024)) > 0) {
        for(i = 0; i < cnt; ++i) {
            if(write(outfiles[i], buffer, bytes) != bytes) {
                sprintf(errMsg, "write %s failed", filenames[i]);
                perror(errMsg);
                exit(1);
            }
        }

        if(write(1, buffer, bytes) != bytes) {
            perror("write stdout failed");
            exit(1);
        }
    }

    if(bytes == -1) {
        perror("read stdin failed");
        exit(1);
    }

    for(i = 0; i < cnt; ++i) {
        if(close(outfiles[i]) == -1) {
            sprintf(errMsg, "close %s failed", filenames[i]);
            perror(errMsg);
            exit(1);
        }
    }

    return 0;
}
