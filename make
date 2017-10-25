CC=g++
CFLAGS=-lstd++ -c -w

oss: oss.o
	$(CC) oss.o -o oss

oss.o: oss.cpp
	$(CC) $(CFLAGS) oss.cpp

.PHONY: clean

clean:
	rm -f *.o
	rm -f oss
