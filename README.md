*This project has been created as part of the 42 curriculum by alejanr2.*

# get_next_line

## Description
get_next_line is a C function that reads a file descriptor line by line. It keeps leftover data between calls to return complete lines efficiently, even with large files.

## Instructions
- Build (normal):
  - `cc -Wall -Wextra -Werror get_next_line.c get_next_line_utils.c -D BUFFER_SIZE=42`
- Build (bonus):
  - `cc -Wall -Wextra -Werror get_next_line_bonus.c get_next_line_utils_bonus.c -D BUFFER_SIZE=42`
- Run example (normal):
  - `./a.out path/to/file`
- Run example (bonus):
  - `./a.out path/to/file`

## How it works
- Normal version:
  - A single static buffer (`stash`) keeps leftover data between calls.
  - The function reads from the fd into a temporary buffer and appends to `stash`.
  - When a `\n` is found (or EOF), it builds the line to return.
  - The remainder after the line stays in `stash` for the next call.
  - When there is nothing left to return, it returns `NULL`.
- Bonus version:
  - Uses one `stash` per file descriptor (an array indexed by `fd`).
  - This allows interleaved reads from multiple files without losing state.
  - The read/return flow is the same as the normal version for each `fd`.

## Resources
- 42 subject: get_next_line
- `man 2 read`
- `man 3 malloc`
