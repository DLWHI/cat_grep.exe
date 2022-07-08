COMP = gcc -Wall -Wextra -Werror -std=c11 -g

COMMONDIR = ./common
OBJDIR = ./obj

CAT_SRC:= ./cat/s21_cat.c
CAT_TEST:= ./cat/test_cat.sh

GREP_SRC:= ./grep/s21_grep.c
GREP_LIBS:= ./grep/libs/*.c
GREP_TEST:= ./grep/test_grep.sh

all: s21_grep
#-----cat-----
s21_cat: xalloc
	$(COMP) $(CAT_SRC) $(OBJDIR)/xalloc.o -o s21_cat
test_cat: s21_cat
	chmod 777 $(CAT_TEST)
	$(CAT_TEST)
#-----grep-----
s21_grep: s21_grep_libs xalloc
	$(COMP) $(GREP_SRC) $(OBJDIR)/*.o -o s21_grep -lregex
s21_grep_libs:
	$(COMP) $(GREP_LIBS) -c
	mv *.o $(OBJDIR)
test_grep: s21_grep
	chmod 777 $(GREP_TEST)
	$(GREP_TEST)
#-----common-----
xalloc:
	$(COMP) $(COMMONDIR)/xalloc.c -o $(OBJDIR)/xalloc.o -c

clean:
	rm -rf $(OBJDIR)/*.o *.out s21_grep *.dSYM mem_report.txt *.exe s21_temp.txt temp.txt