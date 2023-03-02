CC = gcc
FLAGS = -Wall -Wextra -Werror -std=c11
TST_LIBS = -lcheck
ifeq ($(shell uname), Linux)
	TST_LIBS += -lpthread -lrt -lsubunit -lm
endif

LIB_NAME = s21_string

SRCS=$(wildcard s21_*.c)
OBJS=$(SRCS:.c=.o)

all: clean $(LIB_NAME).a
	$(CC) $(FLAGS) -g tests/*.c $(LIB_NAME).a $(TST_LIBS) -o test
	rm -rf *.o


rebuild: clean all

clean:
	rm -rf *.o test *.a *.gcno *.gcda *.gcov \
	*.html *.css *.info report   test.dSYM \
	tests/*.o

format:
	clang-format -i -style=Google *.c *.h tests/*.c tests/*.h

check:
	clang-format -n -style=Google *.c *.h tests/*.c tests/*.h

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

s21_string.a: $(OBJS)
	ar rc $(LIB_NAME).a $^
	ranlib $(LIB_NAME).a
	rm -rf *.o

test: clean $(LIB_NAME).a
	$(CC) $(FLAGS) -g tests/*.c $(LIB_NAME).a $(TST_LIBS) -o test
	./test

add_coverage_flag:
	$(eval FLAGS += --coverage)

gcov_report: add_coverage_flag test
	./test
	lcov -t “gcov_test” -o test.info --rc lcov_branch_coverage=1 --no-external -c -d .
	genhtml -o report/ test.info --rc lcov_branch_coverage=1
	open ./report/index.html

valgrind:
	make test
	$(CC) -g tests/*.c s21_string.a -o test -lcheck -lpthread -lm -D_GNU_SOURCE -lrt -lsubunit
	valgrind --leak-check=full --show-leak-kinds=all -s ./test
