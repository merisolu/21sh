/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jumanner <jumanner@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 11:32:04 by jumanner          #+#    #+#             */
/*   Updated: 2022/11/15 17:42:17 by amann            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

/* Includes */

# include "libft.h"
# include "general.h"
# include "utils.h"
# include "parsing.h"
# include "expansion.h"

/* Types */

typedef struct s_token_dispatch
{
	char				symbol;
	t_token_type		token;
}	t_token_dispatch;

typedef int	t_parse_function(t_token **cursor, t_state *state, char **result);

/* Files */

/* ast_add_args.c */
int		allocate_args_array(char ***res, t_token **cursor);

/* ast_cleanse_args.c */
void	ast_cleanse_ws(t_token **list);

/* ast_fd_agg_format_check.c */
int		ast_fd_agg_format_check(t_token **cursor);

/* ast_free.c */
void	ast_free(t_ast ***tree_list);
void	ast_free_recursion(t_ast *node);

/* ast_parse_expansions.c */
int		ast_parse_expansions(t_ast *root, t_state *state);

/* ast_pipe_sequence.c */
int		ast_pipe_sequence(t_token **cursor, t_ast **node);

/* ast_redirect_recursion.c */
int		ast_redirect_control(t_ast *node, t_token **cursor);

/* ast_retokenize.c */
t_token	*ast_retokenize(char *line);

/* ast_simple_command.c */
int		ast_simple_command(t_token **cursor, t_ast **node);

/* grammar.c */
t_ast	**construct_ast_list(t_token *cursor);

#endif
