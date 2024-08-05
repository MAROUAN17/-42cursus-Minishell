/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_utils7.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oait-laa <oait-laa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 10:25:01 by oait-laa          #+#    #+#             */
/*   Updated: 2024/08/05 15:12:28 by oait-laa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parse_header.h"

int	recheck_wilcard(char **tmp, int j, char *w_tmp, t_token_tree *tree)
{
	if (has_wildcard(w_tmp) || (has_wildcard(tmp[j])
			&& !is_inside_quotes(w_tmp, 0)))
	{
		free(w_tmp);
		if (handle_wildcard(&tmp[j], "", *tree->head) == 0
			&& errno == ENOMEM)
			return (0);
	}
	else
		free(w_tmp);
	return (1);
}

int	has_vars_in_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && is_inside_quotes(str, i) == 0)
			return (1);
		i++;
	}
	return (0);
}
