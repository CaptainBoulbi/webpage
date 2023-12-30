SRC=main.c
BIN=webpage
LIB=ssl
FLAGS=-Wall -Wextra

all: $(BIN)

$(BIN) : $(SRC)
	gcc $(FLAGS) -o $(BIN) $(SRC) -l$(LIB)

run : $(BIN)
	./$(BIN)

clean :
	rm $(BIN)

.PHONY : run clean
