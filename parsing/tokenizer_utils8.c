/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils8.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oait-laa <oait-laa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 19:49:28 by oait-laa          #+#    #+#             */
/*   Updated: 2024/08/12 16:05:29 by oait-laa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parse_header.h"

int	join_extra_with_redi(char **holder, int i)
{
	char	*extra;
	char	*tmp;

	extra = ft_split_first(holder[i + 1]);
	if (!extra)
		return (free_2d_array(holder), exit(1), 0);
	if (holder[i - 1][ft_strlen(holder[i - 1]) - 1] != ' ' && i - 1 > 0)
	{
		tmp = holder[i - 1];
		holder[i - 1] = ft_strjoin(tmp, " ");
		if (!holder[i - 1])
			return (free(extra),
				free_2d_array(holder), exit(1), 0);
		free(tmp);
	}
	tmp = holder[i - 1];
	holder[i - 1] = ft_strjoin(tmp, extra);
	if (!holder[i - 1])
		return (free(extra),
			free_2d_array(holder), exit(1), 0);
	free(tmp);
	free(extra);
	return (0);
}

char	*get_second_cmd(char *holder)
{
	int	i;

	i = 0;
	while (holder[i])
	{
		if (holder[i + 1] && holder[i] == ' ' && holder[i + 1] != ' '
			&& is_inside_quotes(holder, i) == 0)
			return (holder + i + 1);
		i++;
	}
	return (holder);
}

int	alloc_str_no_qt(int j, t_token_tree *tree, char **tmp, char nxt)
{
	char	*w_tmp;

	if (!tmp[j])
		return (0);
	if (has_vars(tmp[j]))
	{
		w_tmp = ft_strdup(tmp[j]);
		tmp[j] = ignore_quotes(&tmp[j]);
		if (!w_tmp || !tmp[j])
			return (free(w_tmp), 0);
		if (is_inside_quotes(w_tmp, 0) == 0 && !if_must_add(j, tmp, nxt))
			return (free(w_tmp), 0);
		tmp[j] = expand_vars(tmp[j], *tree->head, is_inside_quotes(w_tmp, 0));
		if (!tmp[j])
			return (free(w_tmp), 0);
		if (!recheck_wilcard(tmp, j, w_tmp, tree))
			return (0);
	}
	else
	{
		tmp[j] = ignore_quotes(&tmp[j]);
		if (!tmp[j])
			return (0);
	}
	return (1);
}

void	remove_empty_space(char *str)
{
	int	len;

	len = ft_strlen(str) - 1;
	while (len >= 0 && *(str + len) == ' ')
	{
		*(str + len) = '\0';
		len--;
	}
}
