/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oait-laa <oait-laa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 11:00:27 by oait-laa          #+#    #+#             */
/*   Updated: 2024/05/11 14:11:47 by oait-laa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_header.h"

int is_op(char *input)
{
	if (ft_strncmp("&&", input, 2) == 0 || ft_strncmp("||", input, 2) == 0
		|| ft_strncmp(">>", input, 2) == 0 || ft_strncmp("<<", input, 2) == 0
		|| *input == '|' || *input == '(' || *input == ')' || *input == '<'
		|| *input == '>')
		return (1);
	return (0);
}

int	is_inside_quotes(char const *s, int i)
{
	int	inside;

	inside = 0;
	while (i >= 0)
	{
		if (s[i] == '\'' || s[i] == '\"')
			inside = !inside;
		i--;
	}
	return (inside);
}

void skip_op(int *i, char *input)
{
	if (input[*i] && (ft_strncmp("&&", input + *i, 2) == 0
		|| ft_strncmp("||", input + *i, 2) == 0
		|| ft_strncmp(">>", input + *i, 2) == 0
		|| ft_strncmp("<<", input + *i, 2) == 0))
		*i += 2;
	else if (input[*i] && (input[*i] == '|' 
		|| input[*i] == '(' || input[*i] == ')' || input[*i] == '<'
		|| input[*i] == '>'))
		(*i)++;
}

int 	count_cmds(char *input)
{
	int i;
	int	count;

	i = 0;
	count = 0;
	while (input[i])
	{
		if (is_op(input + i))
			skip_op(&i, input);
		else {
			if (input[i] == ' ')
			{
				i++;
				continue;
			}
			while (input[i] != '\0' && (!is_op(input + i)
				|| (is_op(input + i) && is_inside_quotes(input, i))))
				i++;
		}
		count++;
	}
	return (count);
}

int count_token_len(char *input)
{
	int i;
	int	count;

	i = 0;
	count = 0;
	while (input[i] && (!is_op(input + i)
		|| (is_op(input + i) && is_inside_quotes(input, i))))
	{
		count++;
		i++;
	}
	return (count);
}