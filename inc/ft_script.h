/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_script.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltanenba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/12 02:25:29 by ltanenba          #+#    #+#             */
/*   Updated: 2018/07/13 18:36:50 by ltanenba         ###   ########.fr       */
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

# define BUF_SIZE 256

int				g_master;
int				g_slave;
int				g_file;
int				g_writer_pid;
int				g_shell_pid;
char			*g_fname;
struct termios	g_normal_term;

/*
** Function Declarations:
*/

int				transcribe_content(void);
void			script_sig(int signo);
void			script_exit(void);

int				become_shell(void);
int				become_writer(void);
int				become_operator(void);

#endif
