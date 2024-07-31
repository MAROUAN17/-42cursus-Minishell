/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maglagal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 09:29:37 by maglagal          #+#    #+#             */
/*   Updated: 2024/07/31 15:24:00 by maglagal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parse_header.h"

void	delete_env_head(t_env_vars *tmp, t_env_vars **head)
{
	if (tmp->next)
		*head = tmp->next;
	else
		*head = NULL;
	free_node(tmp);
}

void	delete_env_inside(t_env_vars *tmp, char *cmd,
	char **cmds, t_token_tree *tree)
{
	while (tmp && tmp->next && ft_strcmp(tmp->next->env_name, cmd))
		tmp = tmp->next;
	if (tmp && tmp->next && tmp->next->next)
		replace_nodes_content(tmp->next, tmp->next->next, tree, cmds);
	else if (tmp && tmp->next && !tmp->next->next)
	{
		free_node(tmp->next);
		tmp->next = NULL;
	}
}

void	switch_tabs_to_spaces(char *string)
{
	int	i;

	i = 0;
	while (string[i])
	{
		if (string[i] == '\t')
			string[i] = ' ';
		i++;
	}
}

int	init_wildcard(char **str, int i, char **res, char ***sep_str)
{
	*res = ft_strdup("");
	if (!*res)
		return (0);
	*sep_str = ft_split_qt(str[i], '*');
	if (!*sep_str)
		return (free(*res), 0);
	return (1);
}

char	**linkedlist_to_2d_array(t_env_vars *head)
{
	char		**envs_arr;
	t_env_vars	*tmp;
	char		*with_equal;
	char		*str_to_add;
	int			i;

	tmp = NULL;
	i = 0;
	tmp = head;
	envs_arr = (char **)malloc(sizeof(char *) * (count_linkedlist_size(head) + 1));
	while (tmp)
	{
		with_equal = ft_strjoin("=", tmp->env_val);
		str_to_add = ft_strjoin(tmp->env_name, with_equal);
		free(with_equal);
		// printf("string > %s\n", str_to_add);
		envs_arr[i] = str_to_add;
		// printf("i > %d\n", i);
		tmp = tmp->next;
		i++;
	}
	envs_arr[i] = NULL;
	int j = 0;
	while (envs_arr[j])
	{
		printf("env -> %s\n", envs_arr[j]);
		j++;
	}
	return (envs_arr);
}
