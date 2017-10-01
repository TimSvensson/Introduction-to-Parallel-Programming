SRC	= src
OBJ	= obj
INC	= inc
OUT	= out

FILES	= integrate

SRC_F	= $(FILES:%=$(SRC)/%.c)
OBJ_F	= $(FILES:%=$(OBJ)/%.o)
INC_F	= $(FILES:%=$(INC)/%.h)

CC	= gcc

OUTPUT	= integrate

CCFLAGS	= -Wall -std=c11 -ggdb -Wextra -Werror -Wfatal-errors

$(OUTPUT):$(OBJ_F)
	@echo "## MAKE" $(OUTPUT)
	@$(CC) $(CCFLAGS) -lm -o $(OUT)/$@.out $^

$(OBJ_F):$(SRC_F) $(INC_F)
	@echo "## MAKE" $(OUT_F)
	@$(CC) $(CCFLAGS) -lm -c -o $@ $<

test:
	@echo "## MAKE TEST ##"
	@echo "All src files:" $(SRC_F)
	@echo "All obj files:" $(OBJ_F)
	@echo "All inc files:" $(INC_F)
	@echo "##"
