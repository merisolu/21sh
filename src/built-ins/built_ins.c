/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jumanner <jumanner@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 12:18:43 by jumanner          #+#    #+#             */
/*   Updated: 2022/11/02 13:48:22 by amann            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_ins.h"

static const t_cmd_dispatch	*get_dispatch(void)
{
	static const t_cmd_dispatch	dispatch_table[] = {
	{"cd", &cmd_cd},
	{"echo", &cmd_echo},
	{"env", &cmd_env},
	{"setenv", &cmd_setenv},
	{"unsetenv", &cmd_unsetenv},
	{"exit", &cmd_exit},
	{"", NULL}
	};

	return (&(dispatch_table[0]));
}

char	*built_in_search(const char *partial_name)
{
	const t_cmd_dispatch	*dispatch_table;
	size_t					i;

	dispatch_table = get_dispatch();
	i = 0;
	while (dispatch_table[i].run != NULL)
	{
		if (ft_strnequ(
				partial_name, dispatch_table[i].name, ft_strlen(partial_name)))
			return (ft_strdup((char *)dispatch_table[i].name));
		i++;
	}
	return (NULL);
}

t_cmd	*built_in_get(const char *name)
{
	const t_cmd_dispatch	*dispatch_table;
	size_t					i;

	dispatch_table = get_dispatch();
	i = 0;
	while (dispatch_table[i].run != NULL)
	{
		if (ft_strequ(dispatch_table[i].name, name))
			return (dispatch_table[i].run);
		i++;
	}
	return (NULL);
}

/*
 * Runs the given built_in. Forks if necessary. Returning -1 when not forking
 * is intentional to avoid calling waitpid() in execute_tree_list(). It should
 * not cause issues with error handling (at least right now).
 */
pid_t	built_in_run(t_cmd cmd, char *const *args, t_state *state, \
	t_ast_execution *ast)
{
	pid_t	result;
	int		should_fork;
	t_pipes	*pipes;

	pipes = ast->pipes;
	should_fork = !(pipes->read[0] == -1 && pipes->read[1] == -1
			&& pipes->write[0] == -1 && pipes->write[1] == -1);
	if (should_fork)
	{
		result = start_fork(ast);
		if (result == 0)
			exit(cmd(args, state));
		else
			return (result);
	}
	else
	{
		if (ast->node->right
			&& !handle_redirects(ast->node->right, ast->redirect))
			exit(1);
	}
	set_return_value(cmd(args, state), state);
	return (-1);
}