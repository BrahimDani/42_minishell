*This project has been created as part of the 42 curriculum by kadrouin, brdani.*

# Minishell

## Description
Minishell is a small, Unix-like shell implemented in C. The goal of the project is to understand how a command-line interpreter works by building a simplified shell that can parse user input, expand environment variables, manage processes, handle pipelines and redirections, and provide a set of built-in commands.

## Instructions

### Build
- Run `make` at the repository root.

### Run
- Execute the binary: `./minishell`.

### Clean
- `make clean` removes object files.
- `make fclean` removes object files and the binary.
- `make re` rebuilds everything.

## Resources
- GNU Bash Reference Manual: https://www.gnu.org/software/bash/manual/
- POSIX Shell Command Language: https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html
- Advanced Programming in the UNIX Environment (Stevens & Rago)
- man pages: `man sh`, `man bash`, `man execve`, `man fork`, `man pipe`, `man dup2`, `man wait`, `man signal`

### AI usage
- Used for drafting and organizing README structure and wording.
- Used for targeted research on shell behavior and POSIX references.
- No AI-generated code was integrated into the source files.
