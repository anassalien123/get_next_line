*This project has been created as part of the 42 curriculum by <ancheab>.*

# get_next_line

## Description
`get_next_line` is a 42-school project that implements a function able to read from a file descriptor and return **one line at a time** (including the trailing `\n` when it exists).

Goal:
- Implement `char *get_next_line(int fd);`
- Each call returns the **next** line from `fd` until EOF.
- Handle lines of arbitrary length using a fixed `BUFFER_SIZE`.
- Provide a **bonus** version that supports **multiple file descriptors simultaneously**.

Project deliverables in this repository:
- Mandatory: `get_next_line.c`, `get_next_line_utils.c`, `get_next_line.h`
- Bonus: `get_next_line_bonus.c`, `get_next_line_utils_bonus.c`, `get_next_line_bonus.h`

---

## Instructions

### 1) Compile (mandatory)
```sh
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c -o gnl
```

### 2) Compile (bonus)
```sh
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c -o gnl_bonus
```

### 3) Example usage
Create a small tester (example `main.c`):
```c
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "get_next_line.h"

int main(void)
{
    int fd = open("file.txt", O_RDONLY);
    char *line;

    if (fd < 0)
        return (1);

    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}
```

Then compile:
```sh
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 main.c get_next_line.c get_next_line_utils.c -o run
./run
```

Notes:
- `BUFFER_SIZE` can be changed at compile time with `-D BUFFER_SIZE=<N>`.
- Returned lines are heap-allocated: the caller must `free()` them.
- If an error occurs, `get_next_line` returns `NULL`.

---

## Algorithm (detailed explanation + justification)

### Overview
The core challenge is that `read(fd, buffer, BUFFER_SIZE)` returns raw chunks that:
- may contain **part** of a line,
- may contain **multiple** lines,
- and lines can be longer than `BUFFER_SIZE`.

So the algorithm must:
1. Accumulate data across reads until a full line is available.
2. Return exactly one line per call.
3. Keep leftover data for the next call.

This implementation uses a **static stash** (`line`) that persists between calls.

---

### Mandatory version (single FD)
File: `get_next_line.c`

#### Data model
- `static char *line;` acts as a persistent “stash” containing unread data.
- Each call:
  - ensures `line` contains at least one complete line (or EOF),
  - extracts the next line to return,
  - updates `line` to keep only the remainder.

#### Step-by-step flow
1) **Read and append until newline or EOF** (`get_line`)
- Allocate `buffer` of size `BUFFER_SIZE + 1`.
- While there is no `\n` in the stash and `read()` still returns bytes:
  - `read()` into `buffer`
  - `buffer[bytes_number] = '\0'`
  - append `buffer` to stash using `ft_strjoin`
- This is necessary because we cannot predict line boundaries relative to `BUFFER_SIZE`.

2) **Extract the next line** (`the_next_line`)
- Find the end of the line:
  - scan until `\n` or `\0`
- Allocate a new string sized to include `\n` if present.
- Copy characters into the newly allocated `next_line`.
- This produces the function return value.

3) **Keep the remainder for the next call** (`get_new_line`)
- Locate the first `\n`.
- If none exists: free stash and return `NULL` (no remainder).
- Otherwise allocate a new string containing everything after `\n`.
- Free old stash and return the new remainder.

4) **Cleanup empty remainder**
- If remainder exists but is an empty string, free it and set to `NULL`.

#### Justification
This approach is selected because it is:
- **Correct with arbitrary line length** (keeps reading until line boundary).
- **Memory-safe with clear ownership**:
  - stash is owned internally (static),
  - returned line is owned by the caller.
- **Simple and standard** for the project constraints:
  - uses only `read`, dynamic allocation, and minimal helper functions.

Time complexity per call:
- Worst-case can be O(n) or more due to repeated concatenation (`ft_strjoin`) as the stash grows (re-copying).
- For 42 project constraints, this is commonly accepted, and the code remains small and understandable.
(An optimization would be using a linked list or chunked buffer to avoid repeated full copies, but adds complexity.)

---

### Bonus version (multiple FDs)
File: `get_next_line_bonus.c`

#### Key difference
Instead of a single stash:
```c
static char *line[OPEN_MAX];
```

- Each `fd` uses `line[fd]` as its independent stash.
- This allows interleaving calls like:
  - `get_next_line(fd1)`, `get_next_line(fd2)`, `get_next_line(fd1)`, etc.
without corrupting state.

#### Justification
This is the simplest way to associate persistent state with each descriptor:
- O(1) access by index
- no complex mapping structure needed
- aligns with the typical 42 bonus requirements

---

## Resources
### Classic references
- Linux `read(2)` manual: https://man7.org/linux/man-pages/man2/read.2.html
- C dynamic memory allocation (`malloc`, `free`): https://man7.org/linux/man-pages/man3/malloc.3.html
- About file descriptors (UNIX): https://stackoverflow.com/questions/5256599/what-are-file-descriptors-explained-in-simple-terms
- 42 project subject (official PDF provided by 42 intranet): (add your intranet link or keep as internal reference)

### How AI was used
AI usage (this README):
- Used to **draft and structure** the `README.md` to meet the required sections:
  - Description, Instructions, Resources, and a detailed algorithm explanation/justification.
- No AI-generated code was added to the C source files in this repository as part of this README creation.
