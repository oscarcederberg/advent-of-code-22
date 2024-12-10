# day 03: rucksack reorganization
## implementation
i don't remember this one too well.

## improvements
- don't like the use of the `skip`-variable. restructure to avoid it.
- no need to transform the buffer before comparison, do it afterwards (like in part 2).
- move out the parsing from main into it's own function. might make removing `skip` easier.
- handle empty lines better.