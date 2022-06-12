#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include "../common/xalloc.h"

typedef char cflag_t;

#define BFLAG 1
#define EFLAG 2
#define NFLAG 4
#define SFLAG 8
#define TFLAG 16
#define VFLAG 32

char* help_msg =
"Usage: cat [OPTION]... [FILE]...\n"
"Concatenate FILE(s) to standard output.\n\n"
"With no FILE, or when FILE is -, read standard input.\n\n"
"-b, --number-nonblank    number nonempty output lines, overrides -n\n"
"-e                       equivalent to -vE\n"
"-E, --show-ends          display $ at end of each line\n"
"-n, --number             number all output lines\n"
"-s, --squeeze-blank      suppress repeated empty output lines\n"
"-t                       equivalent to -vT\n"
"-T, --show-tabs          display TAB characters as ^I\n"
"-v, --show-nonprinting   use ^ and M- notation, except for LFD and TAB\n"
"    --help     display this help and exit\n\n"
"Examples:\n"
"cat f - g  Output f's contents, then standard input, then g's contents.\n"
"cat        Copy standard input to standard output.\n";

char print_help() {
    printf("%s", help_msg);
    return -1;
}
char invalid_flag(char flag) {
    fprintf(stderr, "s21_cat: unknown option -- %c\n"
           "Try \'s21_cat --help\' for more information.", flag);
    return -1;
}
cflag_t get_flags(int argc, char* argv[]) {
    char format = 0;
    for (int i = 1; i < argc; i++) {
        if (*argv[i] == '-') {
            if (!strcmp(argv[i], "--number-nonblank")) {
                format |= BFLAG;
            } else if (!strcmp(argv[i], "--number")) {
                format |= NFLAG;
            } else if (!strcmp(argv[i], "--squeeze-blank")) {
                format |= SFLAG;
            } else if (!strcmp(argv[i], "--help")) {
                return print_help();
            } else {
                for (char* args = argv[i] + 1; *args; args++) {
                    if (*args == 'b')
                        format |= BFLAG;
                    else if (*args == 'e' || *argv[i] == 'E')
                        format |= EFLAG;
                    else if (*args == 'n')
                        format |= NFLAG;
                    else if (*args == 's')
                        format |= SFLAG;
                    else if (*args == 't' || *argv[i] == 'T')
                        format |= TFLAG;
                    else if (*args == '-')
                        continue;
                    else
                        return invalid_flag(*args);
                    if (*args == 'e' || *args == 't')
                        format |= VFLAG;
                }
            }
        }
    }
    if (format & BFLAG)
        format &= ~NFLAG;
    return format;
}
void printc(unsigned char c, char fls) {
    if (c == '\n' && (fls & EFLAG))
        printf("$\n");
    else if (c == '\t' && (fls & TFLAG))
        printf("^I");
    else if (c <= 0x1F && (fls & VFLAG) && c != '\n' && c != '\t')
        printf("^%c", c + 64);
    else if (c == 0x7F && (fls & VFLAG))
        printf("^?");
    else if (0x80 <= c && c <= 0x9F && (fls & VFLAG))
        printf("M-^%c", c - 64);
    else
        printf("%c", c);
}
void print_line(char* line, cflag_t fls) {
    for (; *line != '\n' && *line != EOF; line++)
        printc(*line, fls);
    if (*line == '\n')
        printc('\n', fls);
}
char* get_line(FILE* fil) {
    int bsize = 32, i = 1;
    char* mem = xmalloc(bsize);
    mem[0] = fgetc(fil);
    while (mem[i - 1] != '\n' && mem[i - 1] != EOF) {
        mem[i++] = fgetc(fil);
        if (i == bsize) {
            mem = xrealloc(mem, bsize*2);
            bsize *= 2;
        }
    }
    if (mem[0] == EOF) {
        xfree(mem);
        return NULL;
    }
    if (i == bsize)
        mem = xrealloc(mem, bsize + 1);
    return mem;
}

int cat(char* path, cflag_t fls) {
    FILE* txt = fopen(path, "r");;
    if (!path)
        txt = stdin;
    int lineno = 0;
    if (txt) {
        char* line = get_line(txt);
        while (line) {
            if ((fls & BFLAG) && *line != '\n')
                printf("%6d\t", ++lineno);
            else if ((fls & NFLAG))
                printf("%6d\t", ++lineno);
            print_line(line, fls);
            if ((fls & SFLAG) && line[0] == '\n') {
                while (line && line[0] == '\n') {
                    xfree(line);
                    line = get_line(txt);
                }
                continue;
            }
            xfree(line);
            line = get_line(txt);
        }
        fclose(txt);
    } else {
        fprintf(stderr, "s21_cat: %s: No such file or directory", path);
    }
    return 1;
}

int main(int argc, char* argv[]) {
    int fcount = 0;
    cflag_t fmt = get_flags(argc, argv);
    for (int i = 1; i < argc && fmt >= 0; i++)
        if (*argv[i] != '-')
            fcount += cat(argv[i], fmt);
        else if (!strcmp(argv[i], "-") || !strcmp(argv[i], "--"))
            fcount += cat(NULL, fmt);
    if (!fcount && fmt >= 0)
        cat(NULL, fmt);
    return -(fmt < 0);
}
