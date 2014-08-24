CC = g++-4.8

DEBUG = true

CXXFLAGS = -std=c++11 -Wall -Wextra -I include

LDFLAGS =

ifeq ($(DEBUG),true)
	CXXFLAGS += -O0 -g3
else
	CXXFLAGS += -O2 -s
endif

%.test:
	$(CC) -o $@.out src/$(basename $@).cpp $(CXXFLAGS) $(LDFLAGS)

.PHONY: %.test
