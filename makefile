CC = gcc
CFLAGS = -Wall $(addprefix -I, $(shell find src -type d))
SRC = main.c $(shell find src -name '*.c')
TARGET = prog

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)