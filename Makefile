BIN=webpage
SRCS=main.c config.c page.c lexer.c eval.c
INC=.
LIB=curl
FLAGS=-Wall -Wextra -Og -g -ggdb -fvar-tracking

all: $(BIN)

$(BIN) : $(SRCS)
	gcc $(FLAGS) -o $(BIN) $(SRCS) -I$(INC) -l$(LIB)

run : $(BIN)
	./$(BIN) $(input)

check :
	cppcheck --enable=all --suppress=missingIncludeSystem -I$(INC) .
	flawfinder .

clean :
	rm -f $(BIN) *.o

.PHONY : run check clean
