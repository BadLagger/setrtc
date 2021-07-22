CC ?= gcc
EXE = setrtc
SRC = main.c
OBJ = $(SRC:.c=.o)
CFLAGS =-c -Wall

all: $(EXE)

$(EXE): $(OBJ) 
	$(CC) $(OBJ) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXE)