/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amann <amann@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 11:32:06 by jumanner          #+#    #+#             */
/*   Updated: 2022/10/19 11:44:52 by amann            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
 * New process for merging quote handling and tokenisation.
 * Each character of the input string is added to a buffer until the token
 * type changes. The buffer is then added as a token to the token list.
 * We then bzero the buffer and start adding to it again, repeating the process
 * until we reach the end of the input string.
 *
 * In each iteration we check quotes. If we hit an opening or closing quote,
 * we flip the in_quotes variable and skip to the next character as these
 * should not form part of our WORD token. While we are 'in_quotes' (i.e. that
 * variable is TRUE), all characters in the string are interpreted literally -
 * so pipes and semicolons form part of a WORD, for example.
 *
 * If in_quotes is still TRUE at the end of the input string, we can enter
 * multi-line input mode.
 */

static int		check_quotes(char c, t_tokenizer *t)
{
	if ((c == '\'' || c == '\"') && !(t->in_quotes))
	{
		t->in_quotes = TRUE;
		t->quote_type = c;
		return (1);
	}
	else if (c == t->quote_type && t->in_quotes)
	{
		t->in_quotes = FALSE;
		return (1);
	}
	return (0);
}

static int	skip_whitespace(char *line)
{
	int i;

	i = 0;
	while (line[i] && get_token_type(line[i], FALSE) == TOKEN_WHITESPACE)
		i++;
	return (i);
}

static int	tokenize_init(t_tokenizer *t, char *line)
{
	t->in_quotes = FALSE;
	t->quote_type = '\0';
	t->buff_idx = 0;
	t->buff = ft_strnew(sizeof(char) * (ft_strlen(line) + 1));
	if (!(t->buff))
		return (0);
	return (1);
}

/*
 * While we are dealing with the same type of token, we keep appending chars
 * to the buffer.
 *
 * When we hit a token that differs, we append the buffer to our token list
 * and clear the buffer. The current token is then added to the start of the
 * buffer and the type is reset to its type.
 *
 * Exceptional case is that we are looking at an & when expecting a > or <
 * This is needed in situations when we are dealing with fd aggregation
 */

//TODO add a process for handling empty strings (i.e. echo "" hello <- should result in 3 word tokens.)
static void t_loop(char *lc, t_tokenizer *t, t_token_type *type, t_token **r)
{
	if (get_token_type(*lc, t->in_quotes) != *type
		&& !((*type == TOKEN_LT || *type == TOKEN_GT)
		&& get_token_type(*lc, t->in_quotes) == TOKEN_AMPERSAND))
	{
			token_add(r, *type, ft_strdup(t->buff));
			ft_bzero(t->buff, ft_strlen(t->buff) + 1);
			*type = get_token_type(*lc, t->in_quotes);
			t->buff_idx = 0;
	}
	(t->buff)[t->buff_idx] = *lc;
	(t->buff_idx)++;
}

t_token	*tokenize(char *line)
{
	t_token			*result;
	t_token_type	type;
	t_tokenizer		t;
	int				i;

	if (!tokenize_init(&t, line))
		return (NULL);
	result = NULL;
	i = skip_whitespace(line);
	type = get_token_type(line[i], FALSE);
	while (line[i])
	{
		if (!check_quotes(line[i], &t))
			t_loop(line + i, &t, &type, &result);
		i++;
	}
	token_add(&result, type, ft_strdup(t.buff));
	free(t.buff);
/*
	t_token *temp = result;
	ft_putendl("########## TOKENS ##########");
	while (temp)
	{
		ft_printf("type = %d || value = %s\n", temp->type, temp->value);
		temp = temp->next;
	}
*/
	return (result);
}
