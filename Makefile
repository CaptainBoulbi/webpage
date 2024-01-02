BIN=webpage
SRCS=main.c config.c page.c lexer.c
INC=.
LIB=curl
FLAGS=-Wall -Wextra -Og -g -ggdb -fvar-tracking

all: $(BIN)

$(BIN) : $(SRCS)
	gcc $(FLAGS) -o $(BIN) $(SRCS) -I$(INC) -l$(LIB)

run : $(BIN)
	./$(BIN) $(input)

clean :
	rm -f $(BIN) *.o

.PHONY : run clean
