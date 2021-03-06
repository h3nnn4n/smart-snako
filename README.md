# Smart Snako

[![gperftools](https://github.com/h3nnn4n/smart-snako/actions/workflows/heap-check.yml/badge.svg)](https://github.com/h3nnn4n/smart-snako/actions/workflows/heap-check.yml)
[![valgrind](https://github.com/h3nnn4n/smart-snako/actions/workflows/valgrind.yml/badge.svg)](https://github.com/h3nnn4n/smart-snako/actions/workflows/valgrind.yml)
[![Clang ASAN check](https://github.com/h3nnn4n/smart-snako/actions/workflows/clang-asan.yml/badge.svg)](https://github.com/h3nnn4n/smart-snako/actions/workflows/clang-asan.yml)
[![clang-format](https://github.com/h3nnn4n/smart-snako/actions/workflows/clang-format-check.yml/badge.svg)](https://github.com/h3nnn4n/smart-snako/actions/workflows/clang-format-check.yml)
[![clang-tidy](https://github.com/h3nnn4n/smart-snako/actions/workflows/clang-tidy.yml/badge.svg)](https://github.com/h3nnn4n/smart-snako/actions/workflows/clang-tidy.yml)
[![cppcheck](https://github.com/h3nnn4n/smart-snako/actions/workflows/cppcheck.yml/badge.svg)](https://github.com/h3nnn4n/smart-snako/actions/workflows/cppcheck.yml)
[![cpplint](https://github.com/h3nnn4n/smart-snako/actions/workflows/cpplint.yml/badge.svg)](https://github.com/h3nnn4n/smart-snako/actions/workflows/cpplint.yml)
[![Smoke Test](https://github.com/h3nnn4n/smart-snako/actions/workflows/run.yml/badge.svg)](https://github.com/h3nnn4n/smart-snako/actions/workflows/run.yml)
[![Unit Tests](https://github.com/h3nnn4n/smart-snako/actions/workflows/tests.yml/badge.svg)](https://github.com/h3nnn4n/smart-snako/actions/workflows/tests.yml)

This is a basic snake implementation, with several built in agents. The following agents are available:

- `random`: Moves in a random direction, but never in a direction where it
  would kill itself. Unless there is no option.

- `manhattan`: Moves in a straight line to the cherry. If it doesn't have line
  of vision it moves randomly until it dies or estabilishes line of sight
  again.

- `raw_hamilton`: Follows a pre-calculated path which visits all cells exactly
  once. If the grid size is odd numbered in the rows and columns (e.g. 5x5 or
  7x11) a halmitonian cycle won't exit. In this case a cycle is generated with
  exacly one cell left out. If the cherry spawns in this cell, an alternative
  cycle is used.

- `dfs`: When a new cherry spawns, runs a depth first search to find a path to
  the next cherry. If a path isn't found, the agent picks a random direction.

- `ida_dfs`: At every step, runs an IDA DFS search to find a path to the
  cherry. If no path is found, the agent picks a random direction.

- `smart_snako`: Uses a halmitonian cycle to get to the cherry in a safe way.
  However, at each game step, it tries to do small changes to the cycle in
  order to reduce the number of cells between the snake head and the cherry.

# References

So cool references to look into:

- [Code Bullet's Video](https://www.youtube.com/watch?v=tjQIO1rqTBE)
- [Alpha Phoenix's Video](https://www.youtube.com/watch?v=TOpBcfbAgPg)

They inspired me into researching this further.

# Usage

Clone this repo and it's submodules and build using `make`. `--verbose` shows
up what is happening. `--width` and `--height` can be used to customize the
board size. Some statistics are logged into `stats/`.

Use `--agent` to set an agent to run. So far the following are available:
`random`, `manhattan`, `raw_hamilton`, `dfs`, `ida_dfs` and `smart_snako`.

# License

All the contents of this repository are released under a MIT No Attribution
License, unless otherwise noted (such as the files under `deps/`) or some code
reuse from other places. See [LICENSE](LICENSE) for more details.
