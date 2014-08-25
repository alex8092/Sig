CC = g++

DEBUG = true

CXXFLAGS = -std=c++11 -Wall -Wextra -I include

LDFLAGS = -lpthread

ifeq ($(DEBUG),true)
	CXXFLAGS += -O0 -g3
else
	CXXFLAGS += -O2 -s
endif

all:
	$(eval TESTS := $(shell find tests/ -name "*.cpp" | sed -e 's/^tests\///g' | sed -e 's/.cpp$$//g'))
	@echo "Try ..."
	@for CMD in $(TESTS); do echo make $$CMD.test; done
	@echo "Or 'make tests' for launching all tests"


%.test:
	@$(CC) -o $@.out tests/$(basename $@).cpp $(CXXFLAGS) $(LDFLAGS)
	@echo "[Test : " $@ "]"
	@./$@.out
	@echo "[End of test]\n"

tests: basic.test switcher.test timer.test
.PHONY: %.test
