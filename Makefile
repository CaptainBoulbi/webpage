SRC=main.c
BIN=webpage
LIB=curl
FLAGS=-Wall -Wextra -Og -g

all: $(BIN)

$(BIN) : $(SRC)
	gcc $(FLAGS) -o $(BIN) $(SRC) -l$(LIB)

run : $(BIN)
	./$(BIN) $(input)

clean :
	rm $(BIN)

.PHONY : run clean
