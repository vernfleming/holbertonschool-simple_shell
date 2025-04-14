# hsh - Simple Shell

**hsh** is a simple Unix command line interpreter, or shell, written in C.  
It is designed to mimic the behavior of `/bin/sh`, supporting both interactive and non-interactive modes.

---

## Features

- Interactive shell with prompt (`$`)
- Non-interactive mode using pipes or redirection
- Command execution using `execve`
- Search and execution using the system `PATH`
- Built-in `exit` command to quit the shell
- Accurate handling of return values and error messages (just like `/bin/sh`)
- Process creation using `fork`
- Proper memory management (no leaks)

---

## How It Works

1. **Input** is read using `getline()`
2. **Tokenization** is performed with `strtok()`
3. The command is searched in the directories listed in the `PATH`
4. If found, the shell **forks a child** process and uses `execve()` to execute the command
5. The **parent process waits** for the child to finish using `wait()`
6. The shell loops until the user exits or an EOF is reached

---

## Compilation

```bash
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
```

---

## Usage

### Interactive Mode

```$ ./hsh
($) ls
file1 file2
($) exit
```

### Non-Interactive Mode

```$ echo "ls -l" | ./hsh
-rw-r--r-- 1 user user 1234 date file1
```

### Example Error Handling

```$ ./hsh
($) nonexistentcommand
./hsh: 1: nonexistentcommand: not found
```

### Allowed Functions and System Calls

- access, chdir, close, execve, exit, _exit, fflush, fork, free, getcwd, getline, getpid
- isatty, kill, malloc, open, opendir, perror, printf, fprintf, vfprintf, sprintf
- putchar, read, readdir, signal, stat, lstat, fstat, strtok, wait, waitpid, wait3, wait4, write
- All functions from string.h

---

## Project Structure

- main.c - Main loop and entry point
- prompt.c - Handles displaying and reading input
- tokenize.c - Splits command strings into arguments
- executor.c - Handles command execution
- path.c - Resolves full path using the PATH variable
- utils.c - Helper functions for string and memory ops
- builtins.c - Handles built-in commands like exit
- README.md - Project documentation

---

## License

Feel free to do with this as you please.