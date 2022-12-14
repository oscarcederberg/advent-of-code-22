# day 02: rock paper scissors
## implementation
scan each character until EOF, skipping any that are not A-B or X-Y. update total by calculating the points based on the input.

### part 1
check if player won, tied, or lost and give points based on result and choice taken.

### part 2
give points based on what choice is needed for a specific outcome, and the outcome of the match.

## improvements
- even if `update_total` looks "cooler" in part 2, it is less readable than in part 1.
- too naive, doesn't care about input being wrong.
- if-else might be a bit prettier in this case.
