
BUILD_DIR	?= .
OBJ_DIR		?= ./build/obj
SRC_DIR		?= ./src

OUT			 = exe.out
OUT_PATH	 = $(addprefix $(BUILD_DIR)/, $(OUT))

SRCSPATH	:= $(shell find $(SRC_DIR) -name *.c)
SRCDIRS 	:= $(shell find . -name '*.c' -exec dirname {} \; | uniq)
OBJDIRS		:= $(patsubst $(SRC_DIR)%, $(OBJ_DIR)%, $(SRCDIRS))
SRCS 		:= $(patsubst $(SRC_DIR)/%, %, $(SRCSPATH))
OBJS 		:= $(SRCS:%.c=$(OBJ_DIR)/%.o)
CC	 		 = gcc

CFLAGS 		 = -O3 -Wall -W -pedantic -I$(SRC_DIR)

LFLAGS_MACOS = 
LFLAGS_LINUX = 

OS 			:= $(shell uname)

all: $(OUT)

$(OUT): buildrepo $(OBJS)
ifeq ($(OS), Darwin)
	$(CC) $(OBJS) -o $(OUT_PATH) $(LFLAGS_MACOS)
else
	$(CC) $(OBJS) -o $(OUT_PATH) $(LFLAGS_LINUX)
endif


$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.c $(SRC_DIR)/%.h
	$(CC) -o $@ -c $< $(CFLAGS)

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.c
	$(CC) -o $@ -c $< $(CFLAGS)


buildrepo:
	$(call make-repo)

clean:
	rm -f $(OBJS)

cleanall:
	rm -f $(OBJS) $(OUT_PATH)

define make-repo
        for dir in $(OBJDIRS); \
        do \
                mkdir -p $$dir; \
        done
endef
