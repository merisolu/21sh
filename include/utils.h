/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jumanner <jumanner@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 11:32:20 by jumanner          #+#    #+#             */
/*   Updated: 2022/11/01 16:15:59 by amann            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

/* Includes */

# include <sys/ioctl.h>
# include <termcap.h>

# include "libft.h"
# include "general.h"

/* Files */

/* error.c */
int		print_error(char *message, int return_value);
int		print_named_error(char *name, char *message, int return_value);
t_ast	*print_error_ast(char *message, t_ast *return_value);

/* return_value.c */
int		get_return_value_from_status(int status);
void	set_return_value(int return_value, t_state *state);

/* utils.c */
void	update_window_size(t_state *state);
void	clear_input(t_state *state, int newline);
void	*var_copy(void *var);

/* utils_input.c */
size_t	input_get_row_count(t_state *state, size_t index);
void	input_get_line_properties(t_state *state, size_t index, size_t *start, \
		size_t *length);

#endif
