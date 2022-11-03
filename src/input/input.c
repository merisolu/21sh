/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jumanner <jumanner@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 12:42:30 by jumanner          #+#    #+#             */
/*   Updated: 2022/11/03 13:39:23 by jumanner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"

extern int	g_last_signal;

static void	append_input(t_input_context *ctx, char character)
{
	if (ft_strlen(ctx->input) == INPUT_MAX_SIZE)
	{
		ft_putstr(tgetstr("bl", NULL));
		return ;
	}
	ft_memmove(ctx->input + ctx->cursor + 1,
		ctx->input + ctx->cursor,
		ft_strlen(ctx->input - ctx->cursor));
	ctx->input[ctx->cursor] = character;
	ctx->cursor++;
}

t_input_result	get_line(t_input_context *ctx)
{
	int		read_count;
	char	buf[BUF_SIZE + 1];
	int		i;

	ft_bzero(&buf, BUF_SIZE + 1);
	read_count = read(STDIN_FILENO, &buf, BUF_SIZE);
	if (read_count == 0)
		return (INPUT_NOTHING_READ);
	i = 0;
	while (i < read_count)
	{
		i += check_movement(buf + i, ctx);
		if (i >= BUF_SIZE)
			break ;
		if (handle_key(buf + i, ctx) == INPUT_NEWLINE_FOUND)
			return (INPUT_NEWLINE_FOUND);
		else if (ft_isprint(buf[i]) || buf[i] == '\n')
			append_input(ctx, buf[i]);
		i++;
	}
	return (INPUT_NO_NEWLINE_FOUND);
}
