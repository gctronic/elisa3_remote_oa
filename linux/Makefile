TARGET = main
CC = gcc
ifeq ($(shell uname), Darwin)
CFLAGS = -O2 -I../../pc-side-elisa3-library -L/opt/local/lib/ -L../../pc-side-elisa3-library/linux/ -lelisa3 -lm -lusb-1.0 -lpthread -lncurses
else
CFLAGS = -O2 -I../../pc-side-elisa3-library -L/usr/lib/i386-linux-gnu/ -L../../pc-side-elisa3-library/linux/ -lelisa3 -lm -lusb-1.0 -lpthread -lncurses
endif

SRC = ../main.c
OBJS = 

all: $(OBJS)
		$(CC) $(SRC)  $(OBJS) $(CFLAGS) -o $(TARGET)

clean:
		rm -f $(TARGET)
		rm *.o

