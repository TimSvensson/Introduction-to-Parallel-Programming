SRC	= src
OBJ	= obj
INC	= inc

FILES	= integrate

SRC_F	= $(FILES:%=$(SRC)/%.c)
OBJ_F	= $(FILES:%=$(OBJ)/%.o)
INC_F	= $(FILES:%=$(INC)/%.h)

CC	= gcc

OUT	= integrate

CCFLAGS	= -Wall -std=c11 -ggdb -Wextra -Werror -Wfatal-errors

$(OUT):$(OBJ_F)
	@echo "## MAKE" $(OUT)
	@$(CC) $(CCFLAGS) -o $@ $^

$(OBJ_F):$(SRC_F) $(INC_F)
	@echo "## MAKE" $(OUT_F)
	@$(CC) $(CCFLAGS) -c -o $@ $<

test:
	@echo "## MAKE TEST ##"
	@echo "All src files:" $(SRC_F)
	@echo "All obj files:" $(OBJ_F)
	@echo "All inc files:" $(INC_F)
	@echo "##"
