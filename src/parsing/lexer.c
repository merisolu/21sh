/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jumanner <jumanner@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 11:32:06 by jumanner          #+#    #+#             */
/*   Updated: 2022/11/16 17:49:16 by amann            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

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

static bool	check_quotes(char c, t_tokenizer *t)
{
	if ((c == '\'' || c == '\"') && !(t->in_quotes))
	{
		t->in_quotes = true;
		t->quote_type = c;
		return (true);
	}
	else if (c == t->quote_type && t->in_quotes)
	{
		t->in_quotes = false;
		return (true);
	}
	return (true);
}

static int	skip_whitespace(char *line)
{
	int	i;

	i = 0;
	while (line[i] && get_token_type(line[i], false) == TOKEN_WHITESPACE)
		i++;
	return (i);
}

static bool	tokenize_init(t_tokenizer *t, char *line)
{
	t->in_quotes = false;
	t->quote_type = '\0';
	t->buff_idx = 0;
	t->buff = ft_strnew(sizeof(char) * (ft_strlen(line) + 1));
	if (!(t->buff))
		return (false);
	return (true);
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

static void	t_loop(char *lc, t_tokenizer *t, t_token_type *type, t_token **r)
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

t_token	*tokenize(char *input, t_tokenizer *tokenizer)
{
	t_token			*result;
	t_token_type	type;
	int				i;

	if (!tokenize_init(tokenizer, input))
		return (NULL);
	result = NULL;
	i = skip_whitespace(input);
	type = get_token_type(input[i], false);
	while (input[i])
	{
		check_quotes(input[i], tokenizer);
		t_loop(input + i, tokenizer, &type, &result);
		i++;
	}
	token_add(&result, type, ft_strdup(tokenizer->buff));
	free(tokenizer->buff);
	return (result);
}
