/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_utils3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maglagal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 11:01:23 by maglagal          #+#    #+#             */
/*   Updated: 2024/07/07 11:58:15 by maglagal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parse_header.h"

void	swap_nodes_content(t_env_vars *env1, t_env_vars *env2)
{
	char	*tmpname;
	char	*tmpval;

	tmpname = env2->env_name;
	tmpval = env2->env_val;
	env2->env_name = env1->env_name;
	env2->env_val = env1->env_val;
	env1->env_name = tmpname;
	env1->env_val = tmpval;
}

void    sort_matched_envs(t_env_vars *head, int nbr_matched, int ascii_nbr)
{
	int			index;
	int			i;
	t_env_vars	*tmp_head;

	i = 0;
	index = 0;
	tmp_head = NULL;
	while (head && head->env_name[0] != ascii_nbr)
		head = head->next;
	while (i < nbr_matched)
	{
		tmp_head = head;
		while (tmp_head && tmp_head->next)
		{
			while (tmp_head->env_name[index] == tmp_head->next->env_name[index])
				index++;
			if (tmp_head->env_name[index] > tmp_head->next->env_name[index])
				swap_nodes_content(tmp_head, tmp_head->next);
			tmp_head = tmp_head->next;
			index = 0;
		}
		i++;
	}
}

void    create_sorted_lst(t_env_vars *node, t_env_vars **head)
{
	t_env_vars  *newnode;
	t_env_vars  *prev;

	prev = get_last_node(*head);
	newnode = malloc(sizeof(t_env_vars)); //leaks
	if (!*head)
		*head = newnode;
	if (prev)
		prev->next = newnode;
	newnode->env_name = ft_strdup(node->env_name); //leaks
	newnode->env_val = ft_strdup(node->env_val); //leaks
	newnode->next = NULL;
}

t_env_vars  *display_envs_sorted(t_env_vars *head)
{
	int			matches;
	int         ascii_nbr;
	t_env_vars  *tmp;
	t_env_vars  *s_head;

	matches = 0;
	ascii_nbr = 33;
	s_head = NULL;
	while (ascii_nbr++ <= 127)
	{
		tmp = head;
		while (tmp)
		{
			if (tmp->env_name[0] == ascii_nbr)
			{
				create_sorted_lst(tmp, &s_head);
				matches++;
			}
			tmp = tmp->next;
		}
		if (matches > 1)
			sort_matched_envs(s_head, matches, ascii_nbr);
		matches = 0;
	}
	return (s_head);
}

void    create_env(t_env_vars *node, t_env_vars *head, char *env)
{
	char	**envs;

	envs = ft_split(env, '='); //leaks
	if (!envs)
		return (free(head), exit(1));
	node->env_name = ft_strdup(envs[0]); //leaks
	node->env_val = ft_strdup(envs[1]); //leaks
	node->next = NULL;
	free_cmds(envs);
	free(envs);
	envs = NULL;
	ft_lstadd(&head, node);
}
