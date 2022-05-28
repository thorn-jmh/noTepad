# Make Sure Your ANSI Encoding method
# same as that of src/handlers/bar_callback.c
# and change CFLAGS charset to your encoding method
#
# compilation flags
CC :=gcc
LD :=gcc
CFLAGS:= -w # -finput-charset=UTF-8 -fexec-charset=UTF-8
LDFLAGS:=  -s -lcomctl32 -lgdi32 -lkernel32 -Wl,--subsystem,windows
# file path 
SRC_DIR :=src
HAND_DIR := $(SRC_DIR)/handlers
MOD_DIR :=	$(SRC_DIR)/modules
LIB_DIR :=	$(SRC_DIR)/libgraphics
BUILD_DIR := build
OBJ_DIR :=$(BUILD_DIR)/obj
MOD_DEPS :=$(MOD_DIR)/include/
HAND_DEPS :=$(HAND_DIR)/include/
LIB_DEPS :=$(LIB_DIR)/include/
INC := -I $(MOD_DEPS) -I $(HAND_DEPS) -I $(LIB_DEPS)
# intermediate files
HANDS:=$(wildcard $(HAND_DIR)/*.c)
MODS:=$(wildcard $(MOD_DIR)/*.c)
LIBS:=$(wildcard $(LIB_DIR)/*.c)
HANDS_O:=$(HANDS:$(HAND_DIR)/%.c=$(OBJ_DIR)/%.o)
MODS_O:=$(MODS:$(MOD_DIR)/%.c=$(OBJ_DIR)/%.o)
LIBS_O:=$(LIBS:$(LIB_DIR)/%.c=$(OBJ_DIR)/%.o)
OBJS := $(MODS_O) $(HANDS_O) $(LIBS_O) $(OBJ_DIR)/main.o 
BUILD := noTepad.exe


all: prepare $(BUILD) clean

prepare:
	$(shell if [ ! -e $(BUILD_DIR) ];then mkdir -p $(BUILD_DIR); fi)
	$(shell if [ ! -e $(OBJ_DIR) ];then mkdir -p $(OBJ_DIR); fi)

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c
	$(CC) $(INC) $(CFLAGS) -c $< -o $@

$(MODS_O):$(OBJ_DIR)/%.o:$(MOD_DIR)/%.c
	$(CC) $(INC) $(CFLAGS) -c $< -o $@

$(LIBS_O):$(OBJ_DIR)/%.o:$(LIB_DIR)/%.c
	$(CC) $(INC) $(CFLAGS) -c $< -o $@

$(HANDS_O):$(OBJ_DIR)/%.o:$(HAND_DIR)/%.c
	$(CC) $(INC) $(CFLAGS) -c $< -o $@

$(BUILD): $(OBJS)
	@$(LD) $(INC) $(CFLAGS) $(LDFLAGS) -o $@ $+

.PHONY: all clean

clean:
	@$(shell rm -rf $(BUILD_DIR);)