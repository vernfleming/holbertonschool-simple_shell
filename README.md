# hsh - Simple Shell

**hsh** is a simple Unix command line interpreter, or shell, written in C.  
It is designed to mimic the behavior of `/bin/sh`, supporting both interactive and non-interactive modes.

---

## Features

- Interactive shell with prompt (`#cisfun$`)
- Non-interactive mode using pipes or redirection
- Command execution using `execve`
- Search and execution using the system `PATH`
- Built-in commands:
  - `exit`: Exit the shell with the status of the last executed command
  - `env`: Print the current environment variables
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

```
$ ./hsh
#cisfun$ ls
file1 file2
#cisfun$ env
USER=username
HOME=/home/username
PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin
#cisfun$ exit
```

### Non-Interactive Mode

```
$ echo "ls -l" | ./hsh
-rw-r--r-- 1 user user 1234 date file1
```

### Example Error Handling

```
$ ./hsh
#cisfun$ nonexistentcommand
./hsh: 1: nonexistentcommand: not found
```

## Exit Status

The shell returns the following exit status codes:
- **0**: Success
- **127**: Command not found
- **Other values**: As returned by the executed command

## Signal Handling

The shell passes most signals to its child processes. Ctrl+C (SIGINT) will terminate the current foreground process but not the shell itself.

---

## Limitations

This shell is for educational purposes and does not support advanced features such as:
- Pipes (`|`)
- Redirection (`>`, `<`)
- Chaining commands using `&&`, `||`
- Environment variable assignment or substitution

---

## Man Page

A manual page is provided with the shell. To view it, run:
```
man ./man_1_simple_shell
```

---

## Project Structure

- simple_shell.c - Main loop and entry point, includes splitting input and command execution
- version.h - Defines version information and header includes
- README.md - Project Documentation
- man_1_simple_shell - Man page
- AUTHORS - Lists all individuals having contributed content to the repository

---

## Authors

- **Jarryd Barrah**
- **Isaac Dillon**
- **Jin Liu**

Written for Holberton School.

---

## License

Feel free to do with this as you please.