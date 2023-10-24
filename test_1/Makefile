V = 1
# BUILD_DIRの中に..を書くくらいならMakefileをトップに持ってくる方がよいと思う
BUILD_DIR = ../build/test_1
include $(N64_INST)/include/n64.mk # ???

TARGET := test_1.z64
N64_ROM_TITLE := "my64-tests test_1"

DEPS_FLAGS = -MMD -MP
CFLAGS := -Wall -Wextra $(DEPS_FLAGS)
CXXFLAGS := -Wall -Wextra $(DEPS_FLAGS)
LDFLAGS := -ldragon

ELF := $(TARGET:%.z64=$(BUILD_DIR)/%.elf)
SRCS := $(wildcard *.c)
OBJS := $(SRCS:%.c=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:%.o=%.d)

.PHONY: all
all: $(TARGET)

$(TARGET): $(ELF)
	some-command-to-make-z64-from-elf $< -o $@

$(ELF): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJS): $(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(DEPS): %.d: %.o
-include $(DEPS)

$(OBJS) $(DEPS) $(ELF): | $(BUILD_DIR)
$(BUILD_DIR):
	mkdir -p $@

.PHONY: clean
clean:
	$(RM) $(OBJS) $(DEPS) $(TARGET)
