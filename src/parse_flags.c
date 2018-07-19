/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_flags.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltanenba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/16 21:05:46 by ltanenba          #+#    #+#             */
/*   Updated: 2018/07/16 21:05:48 by ltanenba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_script.h"

# include <stdio.h>

int					usage(void)
{
	ft_putendl("usage: ft_script [-aq] [-F pipe [command ...]] <or> [file [command ...]]");
	return (0);
}

static void			st_apply_flag(int c)
{
	if (c == 'a')
		g_flags |= a_FLAG;
	if (c == 'q')
		g_flags |= q_FLAG;
	if (c == 'F')
		g_flags |= F_FLAG;
}

int					parse_flags(int ac, char **av)
{
	int			c;

	g_flags = 0;
	if (ac > 1)
		while ((c = ft_getopt(ac, av, "aqF")) != -1)
			st_apply_flag(c);
	if (av[g_opt_index] != NULL)
	{
		g_fname = av[g_opt_index];
		if (av[g_opt_index + 1])
		{
			g_flags |= CMD_FLAG;
			g_cmd_args = av + g_opt_index + 1;
		}
	}
	else
		g_fname = "typescript";
	if (g_opt_error == FT_GETOPT_INVALID_CHAR)
		return (-1);
	return (0);
}
