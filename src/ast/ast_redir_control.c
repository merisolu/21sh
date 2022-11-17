/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_redir_recursion.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jumanner <jumanner@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 13:42:07 by amann             #+#    #+#             */
/*   Updated: 2022/11/16 13:58:16 by amann            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"

static bool	add_redir_out(t_ast *node, t_token *cursor)
{
	if (node->out_type)
		ft_strdel(&(node->out_type));
	node->out_type = ft_strdup(cursor->value);
	if (!node->out_type)
		return (print_bool_error(ERR_MALLOC_FAIL, false));
	cursor = cursor->next;
	if (node->out_file)
		ft_strdel(&(node->out_file));
	node->out_file = ft_strdup(cursor->value);
	if (!node->out_file)
		return (print_bool_error(ERR_MALLOC_FAIL, false));
	cursor = cursor->next;
	return (true);
}

static bool	add_redir_in(t_ast *node, t_token **cursor)
{
	if (node->in_type)
		ft_strdel(&(node->in_type));
	node->in_type = ft_strdup((*cursor)->value);
	if (!node->in_type)
		return (print_bool_error(ERR_MALLOC_FAIL, false));
	*cursor = (*cursor)->next;
	if (!(*cursor) || (*cursor)->type != TOKEN_WORD)
		return (print_bool_error(ERR_SYNTAX, false));
	if (node->in_file)
		ft_strdel(&(node->in_file));
	node->in_file = ft_strdup((*cursor)->value);
	if (!node->in_file)
		return (print_bool_error(ERR_MALLOC_FAIL, false));
	*cursor = (*cursor)->next;
	return (true);
}

static void	add_fd_agg(t_ast *node, t_token **cursor)
{
	t_token	*reset;

	reset = *cursor;
	node->aggregation = true;
	if (read_token(cursor, TOKEN_GT | TOKEN_LT, reset))
	{
		if ((*cursor)->value[0] == '>')
			node->agg_from = STDOUT_FILENO;
		if ((*cursor)->value[0] == '<')
			node->agg_from = STDIN_FILENO;
	}
	else
	{
		node->agg_from = ft_atoi((*cursor)->value);
		eat_token(cursor, TOKEN_WORD, reset);
	}
	eat_token(cursor, TOKEN_GT | TOKEN_LT, reset);
	if ((*cursor)->value[0] == '-')
		node->agg_close = true;
	else
		node->agg_to = ft_atoi((*cursor)->value);
	eat_token(cursor, TOKEN_WORD, reset);
}

bool	ast_redirect_control(t_ast *node, t_token **cursor)
{
	t_token	*reset;

	if (ast_fd_agg_format_check(cursor))
		add_fd_agg(node, cursor);
	reset = *cursor;
	if (eat_token(cursor, TOKEN_GT, reset)
		&& eat_token(cursor, TOKEN_WORD, reset))
	{
		if (!add_redir_out(node, reset))
			return (false);
	}
	else if (eat_token(cursor, TOKEN_GT, reset))
		return (print_bool_syntax_error(ERR_SYNTAX, reset, false));
	reset = *cursor;
	if (eat_token(cursor, TOKEN_LT, reset)
		&& eat_token(cursor, TOKEN_WORD, reset))
	{
		if (!add_redir_in(node, &reset))
			return (false);
	}
	else if (eat_token(cursor, TOKEN_LT, reset))
		return (print_bool_syntax_error(ERR_SYNTAX, reset, false));
	return (true);
}
