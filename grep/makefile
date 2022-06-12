COMP = gcc -Wall -Wextra -Werror -std=c11 -g
TESTSHDIR = test_sh
COMMONDIR = ../common

all: s21_grep

s21_grep: s21_storage s21_list xalloc
	$(COMP) s21_grep.c s21_storage.o s21_list.o xalloc.o -o s21_grep

s21_storage:
	$(COMP) s21_storage.c -o s21_storage.o -c
s21_list:
	$(COMP) s21_list.c -o s21_list.o -c
xalloc:
	$(COMP) $(COMMONDIR)/xalloc.c -o xalloc.o -c
test: s21_grep
	chmod 777 $(TESTSHDIR)/test_grep.sh
	./$(TESTSHDIR)/test_grep.sh
valcheck: s21_grep
	chmod 777 $(TESTSHDIR)/mem_grep.sh
	./$(TESTSHDIR)/mem_grep.sh
clean:
	rm -rf *.o *.out s21_grep *.dSYM mem_report.txt