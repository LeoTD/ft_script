#include "ft_script.h"

int				become_shell(void)
{
	char	*av[2];

	av[0] = ft_getenv("SHELL");
	close(g_master);
	close(g_file);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDOUT_FILENO);
	dup(g_slave);
	dup(g_slave);
	dup(g_slave);
	execve(av[0], av, g_env);
	ft_putstr("Exec failed!\n");
	return (-1);
}

int				become_writer(void)
{
	time_t		tstamp;
	char		buf[BUF_SIZE];
	int			r;

	close(STDIN_FILENO);
	close(g_slave);
	tstamp = time(NULL);
	ft_putstr_fd("Transcript begins: ", g_file);
	ft_putstr_fd(ctime(&tstamp), g_file);
	ft_putstr_fd("\n", g_file);
	while ((r = read(g_master, buf, BUF_SIZE)) > 0)
	{
		write(STDOUT_FILENO, buf, r);
		write(g_file, buf, r);
	}
	return (0);
}

int				become_operator(void)
{
	char		buf[BUF_SIZE];
	int			r;

	while ((r = read(STDIN_FILENO, buf, BUF_SIZE)) > 0)
		write(g_master, buf, r);
	return (0);
}
