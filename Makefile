CFLAGS=-Wall -Werror -Wextra
CC=gcc

s21_grep:
	$(CC) s21_grep.c $(CFLAGS) -o s21_grep

rebuild:
	rm -rf s21_grep
	$(CC) s21_grep.c $(CFLAGS) -o s21_grep

clean:
	rm -rf s21_grep

style:
	clang-format -style=Google -n *.c *.h
	clang-format -style=Google -i *.c *.h


.PHONY: s21_grep