SAN = -fsanitize=undefined
# SAN = -fsanitize=address

CFLAGS += $(SAN)
CFLAGS += -Wall -Wextra -O2 -g 

CXXFLAGS += $(SAN)
CXXFLAGS += -std=c++14 -Llib -lgtest -pthread -g 

all: lru.o lru_test.o

lru.o: lru.c lru.h
	gcc -c $(CFLAGS) $<
	
lru_test.o: lru_test.cc lru.o
	g++ $(CXXFLAGS) $^ -o $@

clean:
	rm *.o