#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
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
 * main - Simple UNIX shell 0.2 with arguments
 *
 * Return: 0 on success
 */

int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	pid_t pid;
	int status;
	char **args;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "#cisfun$ ", 9);

		nread = getline(&line, &len, stdin);
		if (nread == -1)
		{
			free(line);
			break;
		}

		args = split_line(line);
		if (args[0] == NULL)
		{
			free(args);
			continue;
		}

		pid = fork();
		if (pid == 0)
		{

			if (execve(line, argv, environ) == -1)
			{
				perror("./shell");
				exit(EXIT_FAILURE);
			}
		}
		else if (pid > 0)
		{
			wait(&status);
		}
		else
		{
			perror("fork");
			free(args);
			free(line);
			exit(EXIT_FAILURE);
		}

		free(args);
	}

	return (0);
}
