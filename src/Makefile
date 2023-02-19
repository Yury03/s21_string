CC = gcc
FLAGS = -Wall -Wextra -Werror -std=c11
TST_LIBS = -lcheck #$(shell pkg-config --cflags --libs check)
ifeq ($(shell uname), Linux)
	TST_LIBS += -lpthread -lrt -lsubunit -lm
endif
ifneq (,$(findstring desperos,$(MAKECMDGOALS)))
    DEBUG= -g -DDEBUG -fsanitize=address
else ifneq (,$(findstring fartlouv,$(MAKECMDGOALS)))
	DEBUG= -g -fsanitize=address
else ifneq (,$(findstring sallieam,$(MAKECMDGOALS)))
	DEBUG= -g -fsanitize=address
else ifneq (,$(findstring tiaraurz,$(MAKECMDGOALS)))
	DEBUG= -g -fsanitize=address
endif
LIB_NAME = s21_string

SRCS=$(wildcard s21_*.c)
OBJS=$(SRCS:.c=.o)
SRCS2=$(wildcard tests/*.c)
OBJS2=$(SRCS2:.c=.o)

all: clean test

rebuild: all

clean:
	rm -rf *.o test *.a *.gcno *.gcda *.gcov \
	*.html *.css *.info report main/*.o desperos \
	sallieam fartlouv tiaraurz test.dSYM \
	tests/*.o tests/*/*.o tests/*/*/*.o \
	tests/*/*/*/*.o

clang-format:
	clang-format -n -style=Google *.c *.h tests/*.c tests/*.h

set-style-google:
	clang-format -i -style=Google *.c *.h tests/*.c tests/*.h

%.o: %.c
	$(CC) $(FLAGS) $(DEBUG) -c $< -o $@

$(LIB_NAME).a: $(OBJS)
	ar rc $(LIB_NAME).a $^
	ranlib $(LIB_NAME).a
	rm -rf *.o

test: clean $(LIB_NAME).a
	$(CC) $(FLAGS) -g tests/*.c $(LIB_NAME).a $(TST_LIBS) -o test

test_leak: clean
	$(CC) $(FLAGS) -g -DTEST_LEAK $(SRCS) tests/*.c $(TST_LIBS) -o test
#	$(CC) $(FLAGS) -g -fsanitize=address -DTEST_LEAK tests/*.c $(LIB_NAME).a $(TST_LIBS) -o
test_debug: clean $(LIB_NAME).a
	$(CC) $(FLAGS) -g -DDEBUG tests/*.c $(LIB_NAME).a $(TST_LIBS) -o test


add_coverage_flag:
	$(eval FLAGS += --coverage)

gcov_report: add_coverage_flag test
	./test
	lcov -t “gcov_test” -o test.info --rc lcov_branch_coverage=1 --no-external -c -d .
	genhtml -o report/ test.info --rc lcov_branch_coverage=1
	open ./report/index.html

