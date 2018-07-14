/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   roles.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltanenba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/13 18:47:06 by ltanenba          #+#    #+#             */
/*   Updated: 2018/07/13 18:47:56 by ltanenba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_script.h"

int				become_shell(void)
{
	char	*av[2];

	av[0] = ft_getenv("SHELL");
	av[1] = NULL;
	setsid();
	ioctl(g_slave, TIOCSCTTY, NULL);
	close(g_master);
	close(g_file);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	dup(g_slave);
	dup(g_slave);
	dup(g_slave);
	execve(av[0], av, g_environ);
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

int				transcribe_content(void)
{
	g_writer_pid = fork();
	if (g_writer_pid < 0)
		return (-1);
	if (g_writer_pid == 0)
	{
		g_shell_pid = fork();
		g_writer_pid = g_shell_pid;
		if (g_shell_pid < 0)
			return (-1);
		if (g_shell_pid == 0)
			return (become_shell());
		else
			return (become_writer());
	}
	return (become_operator());
}
