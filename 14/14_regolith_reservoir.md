# day 14: regolith reservoir
## implementation
big 1D array instead of 2D with helper functions to get and set. keep track of bounding box to ease printing and tracking of min/max x and y.

## improvements
- array size might be overkill.
- `from_fill_to` isn't very nice. bug-prone.
- `update_bounds` might be more readable and nicer with `min`/`max`-functions instead. less bug-prone too.
- probably structural ways to speed up the algorithm, especially in part 2.