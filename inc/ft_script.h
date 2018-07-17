/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_script.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltanenba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/12 02:25:29 by ltanenba          #+#    #+#             */
/*   Updated: 2018/07/15 19:05:11 by ltanenba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SCRIPT_H
# define FT_SCRIPT_H

# include "libft.h"
# include <time.h>
# include <unistd.h>
# include <termios.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/time.h>

# define BUF_SIZE 256

int				g_master;
int				g_slave;
int				g_file;
int				g_writer_pid;
int				g_shell_pid;
char			*g_fname;
struct termios	g_normal_term;
int				g_flags;
char			**g_cmd_args;

typedef enum	e_scr_flags
{
	a_FLAG = 1,
	F_FLAG = 2,
	q_FLAG = 4,
	CMD_FLAG = 8
}				t_scr_flags;

/*
** Function Declarations:
*/

int				transcribe_content(void);
void			script_sig(int signo);
void			script_exit(void);

int				become_shell(void);
int				become_writer(void);
int				become_operator(void);

int				usage(void);
int				parse_flags(int ac, char **av);

#endif
