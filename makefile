CC = gcc
CFLAGS =  -Wall -Wextra -Werror -std=c11 -g

COMMONDIR = ./common
OBJDIR = ./obj
BUILDDIR = ./build

CAT_SRC:= ./cat/cat.c
CAT_TEST:= ./cat/test_cat.sh

GREP_SRC:= ./grep/grep.c
GREP_LIBS:= ./grep/libs/*.c
GREP_TEST:= ./grep/test_grep.sh

all: cat grep
#-----cat-----
cat: dir xalloc
	$(CC) $(CFLAGS) $(CAT_SRC) $(OBJDIR)/xalloc.o -o $(BUILDDIR)/cat
test_cat: s21_cat
	chmod 777 $(CAT_TEST)
	$(CAT_TEST)
#-----grep-----
grep: dir grep_libs xalloc
	$(CC) $(CFLAGS) $(GREP_SRC) $(OBJDIR)/*.o -o $(BUILDDIR)/grep
grep_libs:
	$(CC) $(CFLAGS) $(GREP_LIBS) -c
	mv *.o $(OBJDIR)
test_grep: s21_grep
	chmod 777 $(GREP_TEST)
	$(GREP_TEST)
#-----common-----
xalloc:
	$(CC) $(CFLAGS) $(COMMONDIR)/xalloc.c -o $(OBJDIR)/xalloc.o -c

dir:
	mkdir -p $(OBJDIR)
	mkdir -p $(BUILDDIR)

clean:
	rm -rf $(OBJDIR) $(BUILDDIR) *.dSYM mem_report.txt *.exe s21_temp.txt temp.txt
