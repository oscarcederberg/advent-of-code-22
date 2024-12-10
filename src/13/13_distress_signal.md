# day 03: distress signal
## implementation
parse packets into `struct value_t` that can either be a list or a value. during comparison, if the other is a number, encapsulate it into a list. this permanetly modifies the packet for future comparisons in part 2, but that's luckily OK. in part 2, feed the compare function into `qsort`, which in turn sorts the array of packets.

## improvements
- no need to sort the entire array, just compare the divider packets with each packet and see how many pass to find it's index.
- part 1 is absolutely possible to solve during parsing.
- the compare function is very ugly. not sure how to handle different list depths well.
- `value_t` is quite ugly, should be renamed to something that makes more sense. perhaps use unions?