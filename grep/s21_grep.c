#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include "../common/xalloc.h"
#include <string.h>
#include <regex.h>
#include <getopt.h>
#include "s21_list.h"
#include "s21_storage.h"

typedef char gflag_t;

#define IFLAG 1
#define VFLAG 2
#define CFLAG 4
#define LFLAG 8
#define NFLAG 16
#define HFLAG 32
#define SFLAG 64
#define OFLAG 128
#define ERRORFLAG ((char)OFLAG | (char)VFLAG)

char *options = "ivclnhsoe:f:";
const struct option help_option[] =
    {
        {"help", no_argument, NULL, 'h'},
        {NULL, 0, NULL, 0}};

char *symbols = "?+*()|";
char *help_msg =
    "Usage: grep [OPTION]... PATTERN [FILE]...\n"
    "Search for PATTERN in each FILE or standard input.\n"
    "PATTERN is, by default, a basic regular expression (BRE).\n"
    "Example: grep -i 'hello world' menu.h main.c\n\n"
    "Regexp selection and interpretation:\n"
    "  -e      use PATTERN for matching\n"
    "  -f      obtain PATTERN from FILE\n"
    "  -i      ignore case distinctions\n\n"
    "Miscellaneous:\n"
    "  -s      suppress error messages\n"
    "  -v,     select non-matching lines\n"
    "  --help  display this help text and exit\n\n"
    "Output control:\n"
    "  -n      print line number with output lines\n"
    "  -h      suppress the file name prefix on output\n"
    "  -o      show only the part of a line matching PATTERN\n"
    "  -l      print only names of FILEs containing matches\n"
    "  -c      print only a count of matching lines per FILE\n\n"
    "When FILE is - or it is not given, read standard input.\n"
    "If fewer than two FILEs are given, assume -h.\n"
    "Exit status is 0 if any line is selected, 1 otherwise;\n"
    "if any error occurs the exit status is 2.\n";

char invalid_flag()
{
    fprintf(stderr, "Try \'s21_grep --help\' for more information.\n");
    return ERRORFLAG;
}

char *get_line(FILE *fil)
{
    int bsize = 32, i = 1;
    char *mem = xmalloc(bsize);
    mem[0] = fgetc(fil);
    while (mem[i - 1] != '\n' && mem[i - 1] != EOF)
    {
        mem[i++] = fgetc(fil);
        if (i == bsize)
        {
            mem = xrealloc(mem, bsize * 2);
            bsize *= 2;
        }
    }
    if (mem[0] == EOF)
    {
        xfree(mem);
        return NULL;
    }
    if (i == bsize)
        mem = xrealloc(mem, bsize + 1);
    mem[--i] = '\0';
    return mem;
}
char *get_regerror(int errcode, regex_t *compiled)
{
    size_t length = regerror(errcode, compiled, NULL, 0);
    char *buffer = xmalloc(length);
    (void)regerror(errcode, compiled, buffer, length);
    return buffer;
}
void printe(int ecode, regex_t regex)
{
    char *msg = get_regerror(ecode, &regex);
    printf("%s", msg);
    xfree(msg);
}

char *translator(char *pattern)
{
    int bsize = 32;
    char *translated = xmalloc(bsize), *offs = translated;
    for (; *pattern; pattern++, translated++)
    {
        if (*pattern == '\\')
        {
            if (strchr(symbols, *(pattern + 1)))
                *translated = *++pattern;
            else
                *translated = *pattern;
        }
        else if (strchr(symbols, *pattern))
        {
            *translated++ = '\\';
            *translated = *pattern;
        }
        else
        {
            *translated = *pattern;
        }
        if (translated - offs >= bsize - 2)
        {
            int d = translated - offs;
            offs = xrealloc(offs, bsize * 2);
            translated = offs + d;
            bsize *= 2;
        }
    }
    if (translated - offs == bsize)
    {
        offs = xrealloc(offs, bsize + 1);
        translated = offs + bsize;
    }
    *translated = '\0';
    return offs;
}
int get_patternsf(char *path)
{
    FILE *pattern_file = fopen(path, "r");
    if (pattern_file)
    {
        char *line = get_line(pattern_file);
        while (line)
        {
            storage_put(line);
            xfree(line);
            line = get_line(pattern_file);
        }
    }
    else
    {
        fprintf(stderr, "s21_grep: %s: No such file or directory", path);
        return -1;
    }
    fclose(pattern_file);
    return 'f';
}
gflag_t get_flags(int argc, char *argv[])
{
    gflag_t format = 0;
    int index, opt;
    while ((opt = getopt_long(argc, argv, options, help_option, &index)) != EOF)
    {
        if (opt == 'i')
            format |= IFLAG;
        else if (opt == 'v')
            format |= VFLAG;
        else if (opt == 'c')
            format |= CFLAG;
        else if (opt == 'l')
            format |= LFLAG;
        else if (opt == 'n')
            format |= NFLAG;
        else if (opt == 'h')
            format |= HFLAG;
        else if (opt == 's')
            format |= SFLAG;
        else if (opt == 'o')
            format |= OFLAG;
        else if (opt == 'e')
            storage_put(optarg);
        else if (opt == 'f')
            opt = get_patternsf(optarg);
        else
            return invalid_flag();
    }
    if (format & VFLAG)
        format &= ~OFLAG;
    return format;
}
int push_patterns(gflag_t fls)
{
    if (storage_get(S_CUR))
        *(storage_get(S_CUR) - 1) = '\0';
    char *translated = translator(storage_get(S_BEG));
    char *pbegin = translated, *pend = pbegin;
    while (pbegin && *pbegin != '\0')
    {
        for (; *pend != '\n' && *pend != '\0' && *pend != EOF; pend++)
        {
        }
        if (*pend)
            *pend++ = '\0';
        regex_t *regex = xmalloc(sizeof(regex_t));
        char ecode = regcomp(regex, pbegin, (((fls & IFLAG) << 1) & REG_ICASE) | REG_EXTENDED);
        if (ecode)
        {
            printe(ecode, *regex);
            regfree(regex);
            xfree(translated);
            xfree(regex);
            return -1;
        }
        else
        {
            pbegin = pend;
            list_push(regex);
        }
    }
    xfree(translated);
    storage_destroy();
    return 0;
}
void free_regexes()
{
    int pcount = list_size();
    for (int i = 0; i < pcount; i++)
    {
        regex_t *regex = list_pop();
        regfree(regex);
        xfree(regex);
    }
}

void print_matches(regex_t *r, char *line)
{
    regmatch_t match;
    char is_match = !regexec(r, line, 1, &match, 0), halt = 0;
    while (is_match)
    {
        if (match.rm_eo == match.rm_so)
            halt = 1;
        for (int i = match.rm_so; i < match.rm_eo; i++)
            printf("%c", line[i]);
        printf("\n");
        line += match.rm_eo;
        is_match = !regexec(r, line, 1, &match, 0);
        if (halt)
            break;
    }
}
void midgrep(char *line, int lineno, char *file, gflag_t fls)
{
    if (!(fls & (CFLAG | LFLAG)))
    {
        if (!(fls & HFLAG))
            printf("%s:", file);
        if (fls & NFLAG)
            printf("%d:", lineno);
        if (!(fls & OFLAG))
            printf("%s\n", line);
    }
}
void postgrep(char *file, int count, gflag_t fls)
{
    if (fls & CFLAG)
    {
        if (!(fls & HFLAG))
            printf("%s:", file);
        if (fls & LFLAG && count)
            count = 1;
        printf("%d\n", count);
    }
    if (fls & LFLAG && count)
        printf("%s\n", file);
}

int mgrep(char *line, int lineno, char *file, gflag_t fls)
{
    int pcount = list_size(), is_match = 0;
    for (int i = 0; i < pcount; i++)
    {
        char match = !regexec(list_index(i), line, 0, NULL, 0);
        if (match && (fls & OFLAG) && !(fls & (CFLAG | LFLAG)))
        {
            if (fls & NFLAG)
                printf("%d:", lineno);
            print_matches(list_index(i), line);
        }
        is_match |= match;
    }
    if (fls & VFLAG)
        is_match = !is_match;
    if (is_match && !(fls & OFLAG))
        midgrep(line, lineno, file, fls);
    return is_match;
}
int grep(char *path, gflag_t fls)
{
    int count = 0;
    FILE *txt = fopen(path, "r");
    if (!path)
        txt = stdin;
    if (txt)
    {
        char *line = get_line(txt);
        int lineno = 1;
        while (line)
        {
            count += mgrep(line, lineno++, path, fls);
            xfree(line);
            line = get_line(txt);
        }
        postgrep(path, count, fls);
        fclose(txt);
    }
    else if (!(fls & SFLAG))
    {
        fprintf(stderr, "s21_grep: %s: No such file or directory\n", path);
    }
    return 1;
}

int main(int argc, char *argv[])
{
    int fcount = 0, offs = 0;
    gflag_t fmt = get_flags(argc, argv);
    if (argc == optind)
        return fprintf(stderr, "Try \'s21_grep --help\' for more information.\n");
    offs = optind++;
    if (!storage_get(S_BEG) && offs)
        storage_put(argv[offs++]);
    push_patterns(fmt);
    for (int i = offs; i < argc && fmt != ERRORFLAG; i++)
        fcount++;
    if (fcount == 1)
        fmt |= HFLAG;
    for (int i = offs, fcount = 0; i < argc && fmt != ERRORFLAG; i++)
        fcount += grep(argv[i], fmt);
    if (!fcount && fmt >= 0)
        grep(NULL, fmt);
    free_regexes();
    list_destroy();
    return 0;
}
