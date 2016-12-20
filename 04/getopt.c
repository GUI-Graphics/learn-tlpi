/* getopt解析命令行
 * getopt的第三个参数optstring规定了命令行的格式
 * 选项用单字符表示，字符后面带冒号(：)表明，该选项必须带一个参数
 *
 * 当解析到正确的选项时，getopt返回选项字符，如果该选项必须带参数，则optarg指向选项参数
 * 否则返回？表明遇到错误，比如未规定的选项，或者必须带参数的选项没有给参数
 * 除此之外函数还会设置optopt，表明是哪个字符出现了问题
 * 默认还会打印错误提示，如果不想自己给出错误提示，默认的错误提示已经很好了
 * 也可以设置opterr=0（默认为1），不输出错误提示，这时最好自己给出错误提示
 * 没有选项可以解析时返回-1
 *
 * getopt还会设置optind，该值表明函数下一次会从哪个命令行字中解析选项
 * 初次调用getopt时，optind=1
 * 当选项是紧密连接的，形如-abc，则optind会三次指向该字
 * 同时函数内部会有状态标志，从而从这一个字中解析出三个选项a，b，c
 * 当选项解析完后，optind就指向了第一个参数，[opting, argc)就是命令行的所有参数
 */

#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv) {
    int opt;
    int ind = 1;

    while((opt = getopt(argc, argv, "ab:cd")) != -1) {
        if(opt != '?') {
            printf("optind: %d, opt: %c", ind, opt);
            if(optarg) printf(", arg: %s", optarg);
            printf("\n");
            ind = optind;
        }
    }

    for(; optind < argc; ++optind) {
        printf("arg: %s\n", argv[optind]);
    }

    return 0;
}
