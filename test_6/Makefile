V=1
SOURCE_DIR=.
BUILD_DIR=../build/test_6
include $(N64_INST)/include/n64.mk

all: test_6.z64
.PHONY: all

OBJS = $(BUILD_DIR)/main.o

test_6.z64: N64_ROM_TITLE="my64-tests test_6"

$(BUILD_DIR)/test_6.elf: $(OBJS)

clean:
	rm -f $(BUILD_DIR)/* *.z64
.PHONY: clean

-include $(wildcard $(BUILD_DIR)/*.d)
