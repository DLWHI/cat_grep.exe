COMP = gcc -Wall -Wextra -Werror -std=c11 -g
TESTSHDIR = test_sh
COMMONDIR = ../common

all: s21_cat

s21_cat: xalloc
	$(COMP) s21_cat.c xalloc.o -o s21_cat
xalloc:
	$(COMP) $(COMMONDIR)/xalloc.c -o xalloc.o -c

test: s21_cat
	chmod 777 $(TESTSHDIR)/test_cat.sh
	./$(TESTSHDIR)/test_cat.sh
valcheck: s21_cat
	chmod 777 $(TESTSHDIR)/mem_cat.sh
	./$(TESTSHDIR)/mem_cat.sh
clean:
	rm -rf *.o *.out s21_cat *.dSYM mem_report.txt