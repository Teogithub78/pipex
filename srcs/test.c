#include <stdio.h>
#include "../libft/libft.h"
#include "../inc/pipex.h"
#include <fcntl.h>




char	**path_table(char **chemins, char **envp)
{
	while(*envp)
	{
		if (ft_strncmp(*envp, "PATH", 4) == 0)
		{
			chemins = ft_split_plus((*envp) + 5, ':');
			if (chemins == NULL)
				return (NULL);
			return (chemins);
		}
		envp++;
	}
	return (chemins);
}

int		parser_flags(char **av)
{
	ft_split(av[2], ' ');
}

int	main(int ac, char **av, char **envp)
{
	char **chemins;
	int	i;
	int	fd;

	i = 0;
	(void)av;
	chemins = NULL;
	(void)ac;
	fd = open(av[1], O_RDONLY);
	if (fd == ERROR)
	{
		perror(av[1]);
		return (0);
	}
	chemins = path_table(chemins, envp);
	while (chemins[i])
	{
		printf("CHEMINS ==");
		printf("%s\n", chemins[i]);
		i++;
	}

	free_tabs(chemins);
	return (0);
}


/*
ACCESS == int access(const char* PATHNAME, int MODE);
	-inc <unistd.h>
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


