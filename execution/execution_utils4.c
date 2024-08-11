/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maglagal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 14:50:48 by maglagal          #+#    #+#             */
/*   Updated: 2024/08/11 13:28:03 by maglagal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parse_header.h"

int	execute_or(t_token_tree *tree, t_env_vars **head,
	char **cmds, int child)
{
	if (execute_tree(tree->left, head, child) == -1)
	{
		if (execute_tree(tree->right, head, child) == -1)
			return (update_underscore_env(tree->right->token, cmds,
					*tree->head, tree), -1);
		return (update_underscore_env(tree->right->token, cmds,
				*tree->head, tree), 0);
	}
	return (update_underscore_env(tree->left->token, cmds,
			*tree->head, tree), 0);
}

int	execute_and(t_token_tree *tree, t_env_vars **head,
		char **cmds, int child)
{
	if (!execute_tree(tree->left, head, child))
	{
		if (execute_tree(tree->right, head, child) == -1)
			return (update_underscore_env(tree->right->token, cmds,
					*tree->head, tree), -1);
	}
	else
		return (update_underscore_env(tree->left->token, cmds, *tree->head,
				tree), -1);
	return (update_underscore_env(tree->right->token, cmds, *tree->head,
			tree), 0);
}

char	*remove_space_first_last(char *str)
{
	int		index_f;
	int		index_l;
	char	*newstr;
	int		i;

	if (!str)
		return (NULL);
	index_f = count_f_index(str);
	index_l = count_l_index(str);
	newstr = malloc(sizeof(char) * (index_l - index_f) + 2);
	if (!newstr)
		return (NULL);
	i = alloc_newstr_re(str, newstr, index_f, index_l);
	return (newstr[i] = '\0', free(str), newstr);
}

int	count_cmd_redirections(t_token_tree *node)
{
	int	redi;

	redi = 0;
	while (node)
	{
		if (node->type == REDIRECTION_A
			|| node->type == REDIRECTION_I || node->type == REDIRECTION_O)
			redi++;
		else
			return (redi);
		node = node->left;
	}
	return (redi);
}

int	handle_redirection(t_token_tree *tree, char **cmds)
{
	if (count_cmd_redirections(tree) == 2
		&& (tree->left->type == REDIRECTION_O
			|| tree->left->type == REDIRECTION_A)
		&& tree->type == REDIRECTION_I)
	{
		if (execute_redirection(tree->left, cmds) == -1)
			return (-1);
		if (execute_redirection(tree, cmds) == -1)
			return (-1);
	}
	else if ((count_cmd_redirections(tree) == 2)
		&& (tree->left->type == REDIRECTION_I)
		&& (tree->type == REDIRECTION_O || tree->type == REDIRECTION_A))
	{
		if (redirec_edge_case(tree, cmds) == -1)
			return (-1);
	}
	else if (execute_redirection(tree, cmds) == -1)
		return (-1);
	return (0);
}
