CC = gcc
CFLAGS = -Wall -Wextra -std=c99 
EXECUTABLE = algorithm
all: $(EXECUTABLE) 
$(EXECUTABLE): GA.c
	$(CC) $(CFLAGS) -o $(EXECUTABLE) GA.c 
clean:
	rm -f $(EXECUTABLE)

