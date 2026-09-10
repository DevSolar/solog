CC ?= gcc
CXX ?= g++
CFLAGS ?= -Wall -Wextra -Wpedantic -Werror -std=c99 -g
CXXFLAGS ?= -Wall -Wextra -Wpedantic -Werror -std=c++11 -g

TEST_BINS = test_basic test_level_feature test_multi_tu test_cpp test_format

.PHONY: all check clean

all: $(TEST_BINS)

test_basic: test_basic.c ../solog.h test_helper.h
	$(CC) $(CFLAGS) test_basic.c -o $@

test_level_feature: test_level_feature.c ../solog.h test_helper.h
	$(CC) $(CFLAGS) test_level_feature.c -o $@

test_multi_tu: test_multi_tu_main.c test_multi_tu_sub.c ../solog.h test_multi_tu_sub.h test_helper.h
	$(CC) $(CFLAGS) test_multi_tu_main.c test_multi_tu_sub.c -o $@

test_cpp: test_cpp.cpp ../solog.h test_helper.h
	$(CXX) $(CXXFLAGS) test_cpp.cpp -o $@

test_format: test_format.c ../solog.h test_helper.h
	$(CC) $(CFLAGS) test_format.c -o $@

check: $(TEST_BINS)
	@echo "=== Running test suite ==="
	@./test_basic
	@./test_level_feature
	@./test_multi_tu
	@./test_cpp
	@./test_format
	@echo "=== All tests passed successfully ==="

clean:
	rm -f $(TEST_BINS) *.out *.o
