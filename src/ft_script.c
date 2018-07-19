/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_script.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltanenba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/13 18:44:08 by ltanenba          #+#    #+#             */
/*   Updated: 2018/07/16 21:03:49 by ltanenba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_script.h"

static int		st_makeraw(struct termios *t)
{
	t->c_iflag &= ~(IGNBRK | BRKINT | PARMRK |
			ISTRIP | INLCR | IGNCR | ICRNL | IXON);
	t->c_oflag &= ~(OPOST);
	t->c_cflag &= ~(CSIZE | PARENB);
	t->c_cflag |= CS8;
	t->c_lflag &= ~(ICANON | ECHO | ECHONL | IEXTEN | ISIG);
	t->c_cc[VMIN] = 1;
	t->c_cc[VTIME] = 0;
	return (0);
}

void			script_sig(int signo)
{
	int					status;
	int					done;
	int					pid;

	signo = 0;
	done = 0;
	while ((pid = wait3(&status, WNOHANG, 0)) > 0)
		if (pid == g_writer_pid)
			done = 1;
	if (done)
		script_exit();
}

void			script_exit(void)
{
	struct timeval		tstamp;

	if (g_shell_pid)
	{
		gettimeofday(&tstamp, NULL);
		if (!(g_flags & q_FLAG))
		{
			ft_putstr_fd("Transcript ends: ", g_file);
			ft_putendl_fd(ctime(&tstamp.tv_sec), g_file);
		}
		close(g_master);
		close(g_file);
	}
	else
	{
		ioctl(STDIN_FILENO, TIOCSETAF, &g_normal_term);
		if (!(g_flags & q_FLAG))
		{
			ft_putstr_fd("Script complete. Stored in: ", STDOUT_FILENO);
			ft_putendl_fd(g_fname, STDOUT_FILENO);
		}
	}
	_exit(0);
}

void			create_master_slave_pair(void)
{
	struct winsize		win;
	char				slave_name[256];

	ioctl(STDIN_FILENO, TIOCGWINSZ, &win);
	g_master = open("/dev/ptmx", O_RDWR);
	ioctl(g_master, TIOCPTYGRANT, 0);
	ioctl(g_master, TIOCPTYUNLK, 0);
	ioctl(g_master, TIOCPTYGNAME, slave_name);
	g_slave = open(slave_name, O_RDWR);
	ioctl(g_slave, TIOCSETAF, &g_normal_term);
	ioctl(g_slave, TIOCGWINSZ, &win);
}

int				main(int argc, char **argv, char **env)
{
	struct termios		custom_term;

	g_environ = env;
	if (parse_flags(argc, argv))
		return (usage());
	if ((g_file = open(g_fname, (g_flags & a_FLAG ? O_APPEND : O_TRUNC) |
			O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0)
		return (-1);
	ioctl(STDIN_FILENO, TIOCGETA, &g_normal_term);
	create_master_slave_pair();
	custom_term = g_normal_term;
	st_makeraw(&custom_term);
	ioctl(STDIN_FILENO, TIOCSETAF, &custom_term);
	signal(SIGCHLD, script_sig);
	if (!(g_flags & q_FLAG))
		ft_putstr_fd("Script begins here:\n\r", STDOUT_FILENO);
	if (transcribe_content())
		return (-1);
	script_exit();
	return (-1);
}
