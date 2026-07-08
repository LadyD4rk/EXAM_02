/* 
TODO: pipe, fork, dup2, execvp, close, exit

! Pipe
Funciona basicamente como o próprio nome já diz. É um
“cano” que faz dois fd se comunicarem entre si de maneira
unilateral, o fd[1] sempre será o que escrevemos e o fd[0]
o que lê a informação recebida em fd[1].

? Fork
Cria um processo simultâneo (filho) com os mesmos dados
do original (pai) e trabalha de maneira independente dele.
O fork() faz com que o PID do pai retorne o do filho
que seria um número positivo e o filho teria o PID em 0.
Se em algum momento isso der errado o PID vai ser um valor 
negativo.

* dup2
Cria uma conexão entre os dois files, dando a direção
e como ele deve ser visto pelo terminal. Mais resumidamente
muda a entrada ou saída padrão para ler/escrever de outro
lugar, mas o processo nem percebe.

! execvp

Recebe como primeiro argumento o comando que precisa ser
procurado na env, depois os argumentos como parametros de
entrada. Substitui o código e dados fornecidos pelo novo
programa. O filho que o Fork fez continua existindo mas
passa a rodar o “comando” passado. Se der algum erro ele
retorna -1. Mas se ele der certo, ele não retorna (o programa
antigo deixa de existir naquele processo).
Ps: O execve precisa do PATH e da localização do comando, o 
execvp sozinho procura no PATH. É como se o execvp chamasse
o execve dentro dele.
*/

//Allowed functions: pipe, fork, dup2, execvp, close, exit

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include "algo.h"

int ft_popen(const char *file, char *const argv[], char type)
{
	pid_t	pid;
	int		fd[2];

	if (!file || !argv || (type != 'r' && type != 'w'))
		return(-1);
	if (pipe(fd) == -1)
		return (-1);
	pid = fork();
	if (pid < 0)
	{
		close(fd[0]);
		close(fd[1]);
		return (-1);
	}
	else if (pid > 0)
	{
		if (type == 'r')
		{
			if (dup2(fd[1], STDOUT_FILENO) == -1)
			{
				close(fd[0]);
				close(fd[1]);
				exit(1);
			}
		}
		if (type == 'w')
		{
			if (dup2(fd[0], STDIN_FILENO))
			{
				close(fd[0]);
				close(fd[1]);
				exit(1);
			}
		}
		close(fd[0]);
		close(fd[1]);
		execvp(file, argv);
		exit(1);
	}
	if (type == 'r')
	{
		close(fd[1]);
		return (fd[0]);
	}
	if (type == 'w')
	{
		close(fd[0]);
		return (fd[1]);
	}
	return (0);
}

int main(void)
{
    int  fd;
    char c;

    fd = ft_popen("ls", (char *const[]){"ls", NULL}, 'w');
    if (fd < 0)
	{
		printf("e");
        return (1);
	}
    while (read(fd, &c, 1) > 0)
        write(1, &c, 1);
    close(fd);
    return (0);
}
