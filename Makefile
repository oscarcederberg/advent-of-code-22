.SILENT:

MAKEFLAGS += --no-print-directory

CC = clang
CFLAGS = -g -Wall
SRC_DIR = src
OUT_DIR = out

.PHONY: all
all:
	@echo "Usage:"
	@echo "		make <day> - Compile and run solutions for the given day (e.g., make 01)"
	@echo "		make clean - Remove all compiled files"

# Dynamic target
.PHONY: $(OUT_DIR)/%/p1 $(OUT_DIR)/%/p2
%:
	@$(MAKE) $(OUT_DIR)/$*/p1.out
	@echo "day $*"
	@echo "part 1 (test):"
	@$(OUT_DIR)/$*/p1.out < $(SRC_DIR)/$*/i
	@echo "part 1:"
	@$(OUT_DIR)/$*/p1.out < $(SRC_DIR)/$*/j
	@$(MAKE) $(OUT_DIR)/$*/p2.out
	@echo "part 2 (test):"
	@$(OUT_DIR)/$*/p2.out < $(SRC_DIR)/$*/i
	@echo "part 2:"
	@$(OUT_DIR)/$*/p2.out < $(SRC_DIR)/$*/j

# Part 1
$(OUT_DIR)/%/p1.out: $(OUT_DIR)/%/p1.o
	$(CC) $(CFLAGS) -o $@ $<

$(OUT_DIR)/%/p1.o: $(SRC_DIR)/%/p1.c
	@mkdir -p $(OUT_DIR)/$*
	$(CC) $(CFLAGS) -c $< -o $@

# Part 2
$(OUT_DIR)/%/p2.out: $(OUT_DIR)/%/p2.o
	$(CC) $(CFLAGS) -o $@ $<

$(OUT_DIR)/%/p2.o: $(SRC_DIR)/%/p2.c
	@mkdir -p $(OUT_DIR)/$*
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
.PHONY: clean
clean:
	rm -rf $(OUT_DIR)
