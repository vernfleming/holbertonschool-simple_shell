#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "version.h"

extern char **environ;

/**
 * split_line - tokenizes the input line into an array of words
 * @line: input command line
 * Return: NULL-terminated array of words (tokens)
 */

char **split_line(char *line)
{
	int bufsize = 64, i = 0;
	char **tokens = malloc(bufsize * sizeof(char *));
	char *token;

	if (!tokens)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, " \t\r\n");
	while (token != NULL)
	{
		tokens[i++] = token;

		if (i >= bufsize)
		{
			bufsize += 64;
			tokens = realloc(tokens, bufsize * sizeof(char *));
			if (!tokens)
			{
				perror("realloc");
				exit(EXIT_FAILURE);
			}
		}
		token = strtok(NULL, " \t\r\n");
	}
	tokens[i] = NULL;
	return (tokens);
}

/**
 * find_command - checks if command exists
 * @cmd: shell command name
 * Return: full path to command
 */

char *find_command(char *cmd)
{
  char *path = NULL, *path_copy, *token, *full_path;
	size_t len;
	int i;

	if (strchr(cmd, '/') != NULL && access(cmd, X_OK) == 0)
		return (strdup(cmd));

	for (i = 0; environ[i]; i++)
	{
		if (strncmp(environ[i], "PATH=", 5) == 0)
		{
			path = environ[i] + 5;
			break;
		}
	}

	if (!path || path[0] == '\0')
		return (NULL);

	path_copy = strdup(path);
	if (!path_copy)
		return (NULL);

	token = strtok(path_copy, ":");
		while (token)
		{
			len = strlen(token) + strlen(cmd) + 2;
			full_path = malloc(len);
			if (!full_path)
				{
					free(path_copy);
					return (NULL);
				}
			snprintf(full_path, len, "%s/%s", token, cmd);
			if (access(full_path, X_OK) == 0)
			{
				free(path_copy);
				return (full_path);
			}
			free(full_path);
			token = strtok(NULL, ":");
		}
	free(path_copy);
        return(NULL);
}

/**
 * main - Simple UNIX shell 0.3 with arguments & PATH handling
 *
 * Return: 0 on success
 */

int main(void)
{
  char *line = NULL, *command_path;
    size_t len = 0;
    ssize_t nread;
    pid_t pid;
    int status, last_status = 0;
    char **args;

    while (1)
    {
        if (isatty(STDIN_FILENO))
            write(STDOUT_FILENO, "#cisfun$ ", 9);

        nread = getline(&line, &len, stdin);
        if (nread == -1)
            break;
        

        args = split_line(line);
        if (args[0] == NULL)
        {
            free(args);
            continue;
        }

        if (strcmp(args[0], "exit") == 0)
        {
            free(args);
            free(line);
            exit(last_status); 
        }

        if (strcmp(args[0], "env") == 0)
        
        {
            char **env = environ;
            while (*env)
            {
                write(STDOUT_FILENO, *env, strlen(*env));
                write(STDOUT_FILENO, "\n", 1);
                env++;
            }
            free(args);
            continue;
        }

        command_path = find_command(args[0]);
        if (!command_path)
          {
            fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
            free(args);
            last_status = 127;
            if (!isatty(STDIN_FILENO))
              {
            free(line);
            exit(last_status);
              }
            continue;
          }

        pid = fork();
        if (pid == 0)
        {

            if (execve(command_path, args, environ) == -1)
            {
                perror("./hsh");
                exit(EXIT_FAILURE);
            }
        }
        else if (pid > 0)
        {
            wait(&status);
            if (WIFEXITED(status)) {
                last_status = WEXITSTATUS(status); 
            }
        }
        else
        {
            perror("fork");
            free(command_path);
            free(args);
            free(line);
            exit(EXIT_FAILURE);
        }
        free(command_path);
        free(args);
    }
    free(line);
    return (last_status); 
}
