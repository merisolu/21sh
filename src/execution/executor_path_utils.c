/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_path_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jumanner <jumanner@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 11:27:48 by jumanner          #+#    #+#             */
/*   Updated: 2022/12/01 12:01:57 by jumanner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

/*
 * Checks whether or not the given path is OK for execution. Returns an integer
 * that can be used as a return value (0 on success > 0 on failure, with
 * specific values for specific errors).
 */

int	check_path_validity(char *path)
{
	if (ft_is_dir(path))
		return (print_error(RETURN_NO_ACCESS, ETEMPLATE_SHELL_NAMED,
				path, ERR_IS_DIR));
	else if (path[ft_strlen(path) - 1] == '/')
		return (print_error(RETURN_NO_ACCESS, ETEMPLATE_SHELL_NAMED,
				path, ERR_IS_NOT_DIR));
	else if (!ft_path_has_valid_end(path))
		return (print_error(RETURN_COMMAND_NOT_FOUND, ETEMPLATE_SHELL_NAMED,
				path, ERR_NO_SUCH_FILE_OR_DIR));
	else if (access(path, X_OK) == -1)
		return (print_error(RETURN_NO_ACCESS, ETEMPLATE_SHELL_NAMED,
				path, ERR_NO_PERMISSION));
	return (0);
}

/*
 * Attempts to find a binary with the given name from PATH in env. Prints
 * ERR_COM_NOT_FOUND error message to stderr if the command could not be found.
 */

int	find_from_path(char *name, char *const *env, char **result)
{
	int		return_value;

	return_value = bin_env_find(name, env, result);
	if (return_value == 0)
		return (print_error(RETURN_COMMAND_NOT_FOUND, ETEMPLATE_SHELL_NAMED,
				name, ERR_COM_NOT_FOUND));
	return (return_value);
}