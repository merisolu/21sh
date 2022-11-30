/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_setenv.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jumanner <jumanner@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/01 15:04:07 by jumanner          #+#    #+#             */
/*   Updated: 2022/11/30 14:07:17 by jumanner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_ins.h"

int	cmd_setenv(char *const *args, t_state *state)
{
	char	*name;
	char	*value;

	name = args[1];
	if (!name)
	{
		env_print_all(state->env);
		return (0);
	}
	if (!(ft_isalpha(name[0]) || name[0] == '_'))
		return (print_error(1, "21sh: setenv: %s\n",
				"Variable name must begin with a letter."));
	value = args[2];
	if (!value)
	{
		value = ft_strchr(name, '=');
		if (!value)
			return (0);
		value += 1;
		name[ft_dstchr(name, '=', ft_strlen(name))] = '\0';
	}
	return (!env_set(name, value, &(state->env)));
}
