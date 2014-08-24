CC = g++

DEBUG = true

CXXFLAGS = -std=c++11 -Wall -Wextra -I include

LDFLAGS =

TESTS = $(addsuffix .test,$(patsubst tests/%,%,$(basename $(shell find tests -name "*.cpp"))))

ifeq ($(DEBUG),true)
	CXXFLAGS += -O0 -g3
else
	CXXFLAGS += -O2 -s
endif

%.test:
	@$(CC) -o $@.out tests/$(basename $@).cpp $(CXXFLAGS) $(LDFLAGS)
	@echo "[Test : " $@ "]"
	@./$@.out
	@echo "[End of test]\n"

tests: $(TESTS)

.PHONY: %.test
