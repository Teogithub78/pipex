#include <stdio.h>
#include "../libft/libft.h"
#include "../inc/pipex.h"
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>

int	check_access_program(char **chemins, char **args)
{
	int	i;
	char *str;
	int	retour;

	i = 0;
	str = ft_strjoin(chemins[i], args[0]);
	while (chemins[i])
	{
		retour = access(str, X_OK);
		free(str);
		if (retour == 0)
			return (i);
		i++;
		if (chemins[i] == NULL)
			return (i - 1);
		str = ft_strjoin(chemins[i], args[0]);
	}
	if (str != NULL)
		free(str);
	return(ERROR);
}

int		path_table(char ***chemins, char **envp)
{
	while(*envp)
	{
		if (ft_strncmp(*envp, "PATH", 4) == 0)
		{
			*chemins = ft_split_plus((*envp) + 5, ':');
			if (chemins == NULL)
				return (ERROR);
			return (0);
		}
		envp++;
	}
	return (ERROR);
}

//Il faut initialiser et open un fichier

int	first_child(char ***args, char **chemins, char **envp, int good_path, int *fd)
{
		int	file_fd_input;
		char	*str;
		int	i;

		file_fd_input = open(args[0][0], O_RDONLY);
		if (file_fd_input == -1)
			exit(0);
		dup2(file_fd_input, 0);
		close(file_fd_input);
		i = 1;
		str = ft_strjoin(chemins[good_path], args[1][0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		i = execve(str, args[i], envp);
		exit(0);

		return (ERROR);
}

int	last_child(char ***args, char **chemins, char **envp, int *fd)
{
	int	file_fd_output;
	char	*str;
	int	i;

	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	close(fd[1]);
	file_fd_output = open(args[3][0], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	dup2(file_fd_output, STDOUT_FILENO);
	i = check_access_program(chemins, args[2]);
	str = ft_strjoin(chemins[i], args[2][0]);
	i = execve(str, args[2], envp);
	exit(0);
}


int	split_args(char ****args, char **av, int ac)
{
	int	i;

	i = 0;
	*args = malloc(sizeof(char **) * ac);
	if (args == NULL)
		return (ERROR);
	while (i < ac - 1)
	{
		(*args)[i] = ft_split(av[i + 1], ' ');
		i++;
	}
	(*args)[i] = NULL;
	return (0);
}



int	pipex(char ***args, char **chemins, char **envp, int good_path)
{
	int	fd[2];
	int pid[2];

	pipe(fd);
	pid[0] = fork();
	if (pid[0] == 0)
	{
		first_child(args, chemins, envp, good_path, fd);
	}
	pid[1] = fork();

	if (pid[1] == 0)
	{
		last_child(args, chemins, envp, fd);
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(pid[0], NULL, 0);
	waitpid(pid[1], NULL, 0);
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	char **chemins;
	char ***args;
	int	i;

	//if (ac != 5)
	//	return (0);
	i = 1;
	chemins = NULL;

	if (split_args(&args, av, ac) == ERROR)
		return (ERROR);

	if (path_table(&chemins, envp) == ERROR)
	{
		free(chemins);
		return (ERROR);
	}

	while (chemins[i])
	{
		printf("CHEMINS ==");
		printf("%s\n", chemins[i]);
		i++;
	}
	i = 0;
	int j = 0;
	while (args[i])
	{
		printf("ARG |%d| == %s\n", i, args[i][j]);
		j++;
		if (args[i][j] == NULL)
		{
			i++;
			j = 0;
		}
	}
	i = check_access_program(chemins, args[1]);
	if (i == ERROR)
	{
	//	perror(NULL);
		printf("ERROR\n");
	}
	else
		printf("SUCCESS\n");
	pipex(args, chemins, envp, i);
	printf("prout");
	i = 0;
	while (args[i])
	{
		free_tabs(args[i]);
		i++;
	}
	free(args);
	free_tabs(chemins);
	return (0);
}


/*
ACCESS == int access(const char* PATHNAME, int MODE);
	- inc <unistd.h>
	- vérifie si le processus qui appelle access peut acceder au fichier PATHNAME
	- MODE == F_OK (check si f existe), R_OK/W_OK/X_OK (check si ficher writable)
	- verif faite avec le user UID ou GID, pas l'ID du programme


EXECVE ==  int execve(const char *pathname, char *const argv[], char *const envp[]);
	- execute le prog de "pathname"
	-

Pipe is used to pass output to another program or utility.

Redirect is used to pass output to either a file or stream.


listes de trucs pas compris :
	# Dans fork:
		- Semaphore (2 semop)
		- pending signal (man 2 sigpending)
		- memory locks (2 mlock)
		-directory change notification ( F_NOTIFY dans man 2 fcntl)
		- (man 2 clone)
		-

	# Dans execve


SUJET :
	- Faire un programme appelé pipex
	- qui prend 2 filename, un "d'entrée" et un de sortie
	- et des commandes/fonctions a executer avec leur flags
	- reliées par des tuyaux
	-

PROBLEMES potentiels (que je sais pas resoudre la tt de suite, liste non exhaustive):
	- recup les arguments
	- messages d'erreurs
	- le truc des PID
	- on doit attendre les process ?

PLAN :
	1 == PARSER ARGUMENTS
		1.

	2 == CHECKER ARGUMENTS
		1. files existent et/ou executablent
			a. tester avec access chaque argument/path possible pour les binaires
		2. commandes existent et/ou executablent
		3

	3 == LANCER FONCTIONS DEMANDER + fork + pipe
		1.la cest flou
		2.


*/
// int main(int ac, char **av)
// {
// 	pipe(av[1], av[2]);
// 	return (0);
// }

// #include <sys/types.h>
// #include <sys/wait.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <string.h>

// int   main(int argc, char *argv[])
// {
// 	int pipefd[2];
// 	pid_t cpid;
// 	char buf;

// 	if (argc != 2)
// 	{
// 		fprintf(stderr, "Usage: %s <string>\n", argv[0]);
// 		exit(EXIT_FAILURE);
// 	}

// 	if (pipe(pipefd) == -1)
// 	{
// 		perror("pipe");
// 		exit(EXIT_FAILURE);
// 	}

//    cpid = fork();
//    if (cpid == -1) {
//    perror("fork");
//    exit(EXIT_FAILURE);
//    }

//    if (cpid == 0) {/* Child reads from pipe */
//    close(pipefd[1]);  /* Close unused write end */

//    while (read(pipefd[0], &buf, 1) > 0)
//    write(STDOUT_FILENO, &buf, 1);

//    write(STDOUT_FILENO, "\n", 1);
//    close(pipefd[0]);
//    _exit(EXIT_SUCCESS);

//    } else {/* Parent writes argv[1] to pipe */
//    close(pipefd[0]);  /* Close unused read end */
//    write(pipefd[1], argv[1], strlen(argv[1]));
//    close(pipefd[1]);  /* Reader will see EOF */
//    wait(NULL);/* Wait for child */
//    exit(EXIT_SUCCESS);
//    }
// }


