# comp & comp flags
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra
CFLAGS_GCOV = $(CFLAGS) -fprofile-arcs -ftest-coverage

# Add flags for shared library compilation
SHARED_FLAGS = -fPIC -shared

# for diff linking on MacOS and Linux
UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
    LDFLAGS := -lcheck -pthread -lcheck_pic -lrt -lm -lsubunit
    LDFLAGS_GCOV := $(LDFLAGS) -fprofile-arcs --coverage
    LIBLINK := -L. -l:s21_decimal.a
else ifeq ($(UNAME), Darwin)  # MacOS
    LDFLAGS := -lcheck
    LDFLAGS_GCOV := $(LDFLDS) -fprofile-arcs --coverage
    LIBLINK := s21_decimal.a
else
    $(error Unsupported platform: $(UNAME))
endif

# source files/dirs
SRCDIR = .
LIB_OBJDIR = lib_obj
TEST_OBJDIR = tests_obj

# List all source files including those in subdirectories
SRC = $(SRCDIR)/decimal.c $(SRCDIR)/binary.c \
			$(SRCDIR)/uint256.c \
      $(wildcard $(SRCDIR)/comparison/*.c) \
      $(wildcard $(SRCDIR)/convertors/*.c) \
      $(wildcard $(SRCDIR)/other_functions/*.c)

# List all object files that will be created in lib_obj
LIB_OBJ = $(patsubst $(SRCDIR)/%.c, $(LIB_OBJDIR)/%.o, $(SRC))
LIB = s21_decimal.a

# for testing & gcov report
TEST_DIR = tests
TEST_SRC = $(wildcard $(TEST_DIR)/test_s21_*.c)
TEST_OBJ = $(patsubst $(TEST_DIR)/%.c, $(TEST_OBJDIR)/%.o, $(TEST_SRC))

# default target
all: $(LIB)

# creating directories for objects
$(LIB_OBJDIR):
	mkdir -p $(LIB_OBJDIR)

$(TEST_OBJDIR):
	mkdir -p $(TEST_OBJDIR)

# building static lib from obj files
$(LIB): $(LIB_OBJ)
	ar rcs $@ $(LIB_OBJDIR)/*.o


# library object files (all object files go into lib_obj directory)
$(LIB_OBJDIR)/%.o: $(SRCDIR)/%.c | $(LIB_OBJDIR)
	$(CC) $(CFLAGS) -g -c $< -o $(LIB_OBJDIR)/$(notdir $@)


# test object files
$(TEST_OBJDIR)/%.o: $(TEST_DIR)/%.c | $(TEST_OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

shared: $(LIB_OBJDIR)
	$(foreach src, $(SRC), $(CC) $(CFLAGS) $(SHARED_FLAGS) -c $(src) -o $(LIB_OBJDIR)/$(notdir $(src:.c=.o));)
	$(CC) $(SHARED_FLAGS) -o s21_decimal.so $(LIB_OBJDIR)/*.o

test: clean $(LIB) $(TEST_OBJ)
	$(CC) $(CFLAGS) $(SHARED_FLAGS) -c test_s21_test_lib_builder.c -o test_s21_test_lib_builder.o
	$(CC) $(SHARED_FLAGS) -o s21_decimal.so test_s21_test_lib_builder.o $(LIB_OBJDIR)/*.o
	$(CC) $(CFLAGS) -o s21_decimal_test.exe $(TEST_OBJDIR)/*.o $(LDFLAGS) $(LIBLINK)
	./s21_decimal_test.exe || true
	python3 ./tests/p.py || true
	rm -f test_s21_test_lib_builder.o s21_decimal.so

# gcov_report target
gcov_report: clean
	mkdir -p $(LIB_OBJDIR)
	$(foreach src,$(SRC),$(CC) $(CFLAGS_GCOV) -fPIC -c $(src) -o $(LIB_OBJDIR)/$(notdir $(src:.c=.o));)
	ar rcs $(LIB) $(LIB_OBJDIR)/*.o

	mkdir -p $(TEST_OBJDIR)
	$(foreach test,$(TEST_SRC),$(CC) $(CFLAGS_GCOV) -c $(test) -o $(TEST_OBJDIR)/$(notdir $(test:.c=.o));)

	$(CC) $(CFLAGS_GCOV) -o s21_decimal_test.exe $(TEST_OBJDIR)/*.o $(LDFLAGS_GCOV) $(LIBLINK)

	$(CC) $(CFLAGS_GCOV) $(SHARED_FLAGS) -o s21_decimal.so test_s21_test_lib_builder.c $(LIB_OBJDIR)/*.o

	./s21_decimal_test.exe || true
	python3 ./tests/p.py || true

	lcov -t "s21_decimal" -o coverage.info -c -d .
	genhtml -o report coverage.info
	@echo "\033[32m\t\t                      GCOV REPORT IS DONE                        \033[0m"
	@echo "\033[32m\t\t          report generated in ./report/index.html                \033[0m"
	rm -f *.gcda *.gcno *.o coverage.info

# clean target
clean:
	rm -rf *.o $(LIB_OBJDIR) $(TEST_OBJDIR) $(LIB) s21_decimal_test.exe *.gcda *.gcno ./report coverage.info *.so tests/*.gcno

.PHONY: all clean test gcov_report shared
