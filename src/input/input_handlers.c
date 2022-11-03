/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jumanner <jumanner@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 12:29:22 by jumanner          #+#    #+#             */
/*   Updated: 2022/11/03 13:41:46 by jumanner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"

// TODO: This needs to be moved elsewhere
/*

	tokens = tokenize(state);
	if (!tokens)
		return (INPUT_READ_ERROR);
	if (state->input[ft_strlen(state->input) - 1] != '\\' && !state->in_quotes)
	{
		state->cursor = 0;
		token_list_free(&tokens);
		return (INPUT_NEWLINE_FOUND);
	}
	token_list_free(&tokens);
	return (INPUT_NO_NEWLINE_FOUND);

*/

static t_input_result	handle_newline(t_input_context *ctx)
{
	(void)ctx;
	return (INPUT_NEWLINE_FOUND);
}

static t_input_result	handle_delete_char(t_input_context *ctx)
{
	if (ctx->cursor == 0)
		return (INPUT_NO_NEWLINE_FOUND);
	ft_strcpy(ctx->input + ctx->cursor - 1, ctx->input + ctx->cursor);
	ft_bzero(ctx->input + ft_strlen(ctx->input),
		INPUT_MAX_SIZE - ft_strlen(ctx->input));
	ctx->cursor--;
	return (INPUT_NO_NEWLINE_FOUND);
}

// TODO: Missing
	// {TAB, &autocomplete},
	// {CTRL_D, &ctrl_d},
t_input_result	handle_key(char *buf, t_input_context *ctx)
{
	size_t								i;
	static const t_key_handler_dispatch	dispatch_table[] = {
	{RETURN_KEY, &handle_newline},
	{BACKSPACE, &handle_delete_char},
	{CTRL_K, &cut_from_cursor},
	{CTRL_U, &cut_to_cursor},
	{CTRL_W, &cut_word},
	{CTRL_Y, &paste},
	{0, NULL}
	};

	i = 0;
	while (dispatch_table[i].run != NULL)
	{
		if (ft_strnequ(dispatch_table[i].activator, buf,
				ft_strlen(dispatch_table[i].activator)))
			return (dispatch_table[i].run(ctx));
		i++;
	}
	return (INPUT_NO_NEWLINE_FOUND);
}
