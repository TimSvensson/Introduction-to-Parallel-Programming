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

CCFLAGS	= -Wall -std=c11 -ggdb -Wextra -Werror -Wfatal-errors -pthread

run_test:$(OUTPUT)
	@echo "####"
	@echo "####\tMAKE\tRUNNING TESTS"
	@echo "####"
	$(OUT)/$(OUTPUT).out 2 7
#	$(OUT)/$(OUTPUT).out 1 10
#	$(OUT)/$(OUTPUT).out 1 100
#	$(OUT)/$(OUTPUT).out 1 1000
	@echo "####"
	@echo "####\tMAKE\tENDING TESTS"
	@echo "####"

$(OUTPUT):$(OBJ_F)
	@echo "####"
	@echo "####\tMAKE\tCREATING" $(OUTPUT)
	@echo "####"
	$(CC) $(CCFLAGS) -o $(OUT)/$@.out $^ -lm
	@echo "####"
	@echo "####\tMAKE\tCREATING" $(OUTPUT)
	@echo "####"

$(OBJ_F):$(SRC_F) $(INC_F)
	$(CC) $(CCFLAGS) -c -o $@ $< -lm

test:
	@echo "## MAKE TEST ##"
	@echo "All src files:" $(SRC_F)
	@echo "All obj files:" $(OBJ_F)
	@echo "All inc files:" $(INC_F)
	@echo "##"
