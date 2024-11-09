SRC=main.c
CC=sdcc
FAMILY=pic14
PROC=16f886
PORT=ttyUSB0

all: $(SRC:.c=.hex)

$(SRC:.c=.hex): $(SRC)
	$(CC) --use-non-free -m$(FAMILY) -p$(PROC) $^

clean:
	rm -f $(SRC:.c=.asm) $(SRC:.c=.cod) $(SRC:.c=.hex) $(SRC:.c=.lst) $(SRC:.c=.o)

upload: all
	zeppp -c /dev/${PORT} -wait 2000 -i ${SRC:.c=.hex} -p 

.PHONY: all clean