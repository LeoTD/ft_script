#include "ft_script.h"

static int		transcribe_content(void)
{
	g_writer_pid = fork();
	if (g_writer_pid < 0)
		;//ERROR
	if (g_writer_pid == 0)
	{
		g_shell_pid = fork();
		if (g_shell_pid < 0)
			;//ERROR
		if (g_shell_pid == 0)
			return (become_shell());
		else
			return (become_writer());
	}
	return (become_operator());
}

static int		st_makeraw(struct termios *t)
{
	t->c_iflag &= ~(IGNBRK | BRKINT | PARMRK | 
			ISTRIP | INLCR | IGNCR | ICRNL | IXON);
	t->c_oflag &= ~(OPOST);
	t->c_cflag &= ~(CSIZE | PARENB);
	t->c_cflag |= CS8;
	t->c_lflag &= ~(ICANON | ECHO | ECHONL | IEXTEN | ISIG);
//	t->c_cc[VMIN] = 1;
//	t->c_cc[VTIME] = 0;
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
		if (pid == g_shell_pid)
			done = 1;
	if (done)
		script_exit();
}

void			script_exit(void)
{
	time_t				tstamp;

	if (g_writer_pid == 0)
	{
		tstamp = time(NULL);
		ft_putstr_fd("Transcript ends: ", g_file);
		ft_putstr_fd(ctime(&tstamp), g_file);
		ft_putstr_fd("\n", g_file);
		close(g_master);
		close(g_file);
	}
	else
	{
		ft_putstr_fd("Script complete. Stored in: ", STDOUT_FILENO);
		ft_putstr_fd(g_fname, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
	}
	exit(0);
}

int			main(int argc, char **argv, char **env)
{
	struct termios		custom_term;
	struct termios		normal_term;
	struct winsize		win;

	g_env = env;
	if (argc > 1)
		g_fname = argv[1];
	else
		g_fname = ft_strdup("typescript");
	if ((g_file = open(g_fname, O_WRONLY | O_CREAT)) < 0)
		;//ERROR
	tcgetattr(STDIN_FILENO, &normal_term);
	tcgetattr(STDIN_FILENO, &custom_term);
	ioctl(STDIN_FILENO, TIOCGWINSZ, &win);

	/* This is not a system call */
	if (openpty(&g_master, &g_slave, NULL, &normal_term, &win) == -1)
		;//ERROR

	ft_putstr_fd("Script begins here:\n", STDOUT_FILENO);
	st_makeraw(&custom_term);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &custom_term);

	signal(SIGCHLD, script_sig);

	if (transcribe_content())
		return (-1);//ERROR
	free(g_fname);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &normal_term);
	script_exit();
	return (-1);
}
