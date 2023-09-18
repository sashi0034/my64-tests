V=1
SOURCE_DIR=.
BUILD_DIR=../build/test_2
include $(N64_INST)/include/n64.mk

all: test_2.z64
.PHONY: all

OBJS = $(BUILD_DIR)/main.o $(BUILD_DIR)/rsp_process.o

test_2.z64: N64_ROM_TITLE="my64-tests test_2"

$(BUILD_DIR)/test_2.elf: $(OBJS)

clean:
	rm -f $(BUILD_DIR)/* *.z64
.PHONY: clean

-include $(wildcard $(BUILD_DIR)/*.d)
