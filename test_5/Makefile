V=1
SOURCE_DIR=.
BUILD_DIR=../build/test_5
include $(N64_INST)/include/n64.mk

all: test_5.z64
.PHONY: all

OBJS = $(BUILD_DIR)/main.o

test_5.z64: N64_ROM_TITLE="my64-tests test_5"
test_5.z64: $(BUILD_DIR)/test_5.dfs

$(BUILD_DIR)/test_5.dfs: $(wildcard filesystem/*)
$(BUILD_DIR)/test_5.elf: $(OBJS)

clean:
	rm -f $(BUILD_DIR)/* *.z64
.PHONY: clean

-include $(wildcard $(BUILD_DIR)/*.d)
