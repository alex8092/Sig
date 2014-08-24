CC = g++

DEBUG = true

CXXFLAGS = -std=c++11 -Wall -Wextra -I include

LDFLAGS =

ifeq ($(DEBUG),true)
	CXXFLAGS += -O0 -g3
else
	CXXFLAGS += -O2 -s
endif

%.test:
	$(CC) -o $@.out test/$(basename $@).cpp $(CXXFLAGS) $(LDFLAGS)

.PHONY: %.test
