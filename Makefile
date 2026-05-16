CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
TARGET = parafilter
OBJS = main.o search.o strings.o pipes.o files.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

main.o: main.c search.h strings.h
	$(CC) $(CFLAGS) -c main.c -o main.o

search.o: search.c search.h
	$(CC) $(CFLAGS) -c search.c -o search.o

strings.o: strings.c strings.h
	$(CC) $(CFLAGS) -c strings.c -o strings.o

pipes.o: pipes.c pipes.h
	$(CC) $(CFLAGS) -c pipes.c -o pipes.o

files.o: files.c files.h
	$(CC) $(CFLAGS) -c files.c -o files.o

clean:
	rm -f $(OBJS) $(TARGET)

cleano:
	rm -f $(OBJS)
