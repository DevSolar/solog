CC ?= gcc
CXX ?= g++
CFLAGS ?= -Wall -Wextra -Wpedantic -Werror -std=c99 -g
CXXFLAGS ?= -Wall -Wextra -Wpedantic -Werror -std=c++11 -g
LDFLAGS ?=

EXEEXT ?=
ifeq ($(OS),Windows_NT)
    EXEEXT := .exe
else ifneq ($(findstring mingw,$(CC)$(CXX)),)
    EXEEXT := .exe
else ifneq ($(findstring w64,$(CC)$(CXX)),)
    EXEEXT := .exe
else ifneq ($(findstring win32,$(CC)$(CXX)),)
    EXEEXT := .exe
endif

ifneq ($(EXEEXT),)
    LDFLAGS += -static-libgcc -static-libstdc++
endif

OBJS = test_main.o \
       test_basic.o \
       test_level_feature.o \
       test_color_feature.o \
       test_multi_tu_main.o \
       test_multi_tu_sub.o \
       test_cpp.o \
       test_format.o \
       test_custom_alloc.o \
       test_location_feature.o \
       test_time_feature.o

TARGET_BASE = test_runner
TARGET = $(TARGET_BASE)$(EXEEXT)

.PHONY: all check clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJS) -o $@

%.o: %.c ../solog.h test_helper.h greatest.h
	$(CC) $(CFLAGS) -c $< -o $@

test_cpp.o: test_cpp.cpp ../solog.h test_helper.h greatest.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

check: $(TARGET)
	@echo "=== Running unified test runner ==="
	@./$(TARGET)

clean:
	rm -f $(TARGET_BASE) $(TARGET_BASE).exe $(TARGET) *.o *.out
