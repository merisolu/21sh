/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_redir_recursion.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amann <amann@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 13:42:07 by amann             #+#    #+#             */
/*   Updated: 2022/10/21 15:07:03 by amann            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static int	add_redir_out(t_ast *node, t_token **cursor)
{
	node->out_type = ft_strdup((*cursor)->value);
	if (!node->out_type)
		return (0);
	*cursor = (*cursor)->next;
	if (!(*cursor) || (*cursor)->type != TOKEN_WORD)
		return (0);
	node->out_file = ft_strdup((*cursor)->value);
	if (!node->out_file)
		return (0);
	*cursor = (*cursor)->next;
	return (1);
}

static int	add_redir_in(t_ast *node, t_token **cursor)
{
	node->in_type = ft_strdup((*cursor)->value);
	if (!node->in_type)
		return (0);
	*cursor = (*cursor)->next;
	if (!(*cursor) || (*cursor)->type != TOKEN_WORD)
		return (0);
	node->in_file = ft_strdup((*cursor)->value);
	if (!node->in_file)
		return (0);
	*cursor = (*cursor)->next;
	return (1);
}

int	ast_redirect_recursion(t_ast *node, t_token **cursor)
{
	if (*cursor && (*cursor)->type == TOKEN_GT)
	{
		if (!add_redir_out(node, cursor))
			return (0);
	}
	if (*cursor && (*cursor)->type == TOKEN_LT)
	{
		if (!add_redir_in(node, cursor))
			return (0);
	}
	if (*cursor
		&& ((*cursor)->type == TOKEN_GT || (*cursor)->type == TOKEN_LT))
		ast_redirect_recursion(node, cursor);
	return (1);
}
