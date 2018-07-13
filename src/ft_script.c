

int			main(int argc, char **argv)
{
	int					master;
	int					slave;
	int					file
	int					rc;
	char				fname[] = "afile";
	struct termios		custom_term, normal_term;
	struct winsize		win;

	if ((file = open(fname, O_WRONLY | O_CREAT)) == NULL)
		;//ERROR
	tcgetattr(STDIN_FILENO, &normal_term);
	tcgetattr(STDIN_FILENO, &custom_term);
	ioctl(STDIN_FILENO, TIOCGWINSZ, &win);

	/* This is not a system call */
	if (openpty(&master, &slave, NULL, &normal_term, &win) == -1)
		;//ERROR

	printf("Script begins here:\n");
	st_makeraw(&custom);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &custom);

	signal(SIGCHLD, script_finish);

	transcribe_content(master, slave, file);

	return (0);
}

int				transcribe_content(int master, int slave, int file)
{
	int					writer_pid;
	int					shell_pid;

	writer_pid = fork();
	if (writer_pid < 0)
		;//ERROR
	if (writer_pid == 0)
	{
		shell_pid = fork();
		if (shell_pid < 0)
			;//ERROR
		if (shell_pid == 0)
			become_shell(master, slave, file);
		else
			become_writer(master, slave, file);
	}
	become_operator(master, slave, file);
	return (0);
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

void			script_finish(int signo)
{
	printf("script_finish() called!\n");
}
