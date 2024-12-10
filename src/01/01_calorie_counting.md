# day 1: calorie counting
## implementation
until hitting EOF or an empty line, parse each line as a number and sum them together. when an empty line or newline is found the sum is saved into an array in the same index as the current elf.

## improvements
- no need to save each result into an array. only the top result (or tope three results in part 2) need to be tracked.
- instead of `realloc`:ing each iteration a larger array can be allocated from the start. keep track of the allocated space and only call `realloc` when the count is equal to the allocated space.
- don't like the way empty spaces are handled. still unsure about how to use `scanf`, `fgets`, etc. well.
