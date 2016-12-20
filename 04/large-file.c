/* 64位系统默认支持LFS了
 * ./a.out test 10000000000
 * ls -lh
 * 将看到test文件大小为9.4G
 * 只有在移动偏移位置至文件末尾之后，且进行写之后才会有空洞
 * gcc -m32 large-file.c会报编译错误，不知为何
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <fcntl.h>

#include <unistd.h>

char errMsg[1024];

int main(int argc, char** argv) {
    if(argc != 3 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "error, usage: %s pathname offset\n", argv[0]);
        exit(1);
    }

    int fd = open(argv[1], O_CREAT | O_RDWR, S_IWUSR | S_IRUSR);
    if(fd == -1) {
        sprintf(errMsg, "open %s failed", argv[1]);
        perror(errMsg);
        exit(1);
    }

    off_t offset = atoll(argv[2]);
    if(lseek(fd, offset, SEEK_SET) == -1) {
        perror("seek failed");
        exit(1);
    }

    if(write(fd, "test", 4) == -1) {
        perror("write failed");
        exit(1);
    }

    if(close(fd) == -1) {
        sprintf(errMsg, "close %s failed", argv[1]);
        perror(errMsg);
        exit(1);
    }

    return 0;
}
