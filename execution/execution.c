/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maglagal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 12:32:52 by maglagal          #+#    #+#             */
/*   Updated: 2024/08/12 15:06:53 by maglagal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parse_header.h"

int	exit_cases(t_token_tree *tree, char **cmds)
{
	int		i;

	i = 0;
	if (!contains_only_spaces(cmds[1]))
		return (print_err("minishell: exit: ", cmds[1],
			": numeric argument required\n"), exit(255), -1);
	while (cmds[++i])
	{
		cmds[i] = remove_space_first_last(cmds[i]);
		if (!cmds[i] && errno == ENOMEM)
			return (ft_close(cmds, tree->head, tree), exit(1), -1);
	}
	return (0);
}

int	check_expand(t_token_tree *tree, char **str)
{
	if (has_vars(*str))
	{
		*str = split_quotes(*str, tree);
		if (!*str)
			return (ft_close(NULL, tree->head, tree),
				exit(1), -1);
		return (0);
	}
	else
	{
		*str = ignore_quotes(str);
		if (!*str)
			return (ft_close(NULL, tree->head, tree),
				exit(1), -1);
	}
	return (1);
}

int	execute_cmds_with_operators_heredoc(t_token_tree *tree, t_env_vars **head,
	char **cmds, int child)
{
	if (!ft_strcmp(tree->token, "&&"))
		return (execute_and(tree, head, cmds, child));
	else if (!ft_strcmp(tree->token, "||"))
		return (execute_or(tree, head, cmds, child));
	else if (!ft_strcmp(tree->token, "|"))
		execute_pipe(tree);
	else if (tree->type == HEREDOC)
	{
		execute_heredoc(tree);
		return (update_underscore_env(tree->left->token, cmds,
				*tree->head, tree), 0);
	}
	return (update_underscore_env("", cmds, *tree->head, tree), 0);
}

int	execute_one_command(t_token_tree *tree, char **cmds, int child)
{
	int		n;
	int		flag;
	char	*old;

	n = 0;
	while (cmds[n])
	{
		old = ft_strdup(cmds[n]);
		if (!old)
			return (ft_close(cmds, tree->head, tree), exit(1), -1);
		flag = set_flag(cmds, n);
		check_expand(tree, &cmds[n]);
		if (flag == 1)
			if_must_split(&cmds, &n, tree, old);
		else
			free(old);
		if (!cmds[n])
			break ;
		n++;
	}
	if (cmds[0] == NULL)
		return (free_2d_array(cmds), 0);
	if (cmds && exec_command(tree, cmds, child) == -1)
		return (free_2d_array(cmds), -1);
	return (free_2d_array(cmds), 0);
}

int	execute_tree(t_token_tree *tree, t_env_vars **head, int child)
{
	char	**cmds;

	check_for_wildcard(tree);
	cmds = ft_split_qt(tree->token, ' ');
	if (!cmds && errno == ENOMEM)
		return (ft_close(NULL, tree->head, tree), exit(1), -1);
	if (tree->type == REDIRECTION_I || tree->type == REDIRECTION_O
		|| tree->type == REDIRECTION_A)
	{
		if (execute_redirection(tree, cmds) == -1)
			return (free_2d_array(cmds), -1);
	}
	else if (!tree->right && !tree->left)
		return (execute_one_command(tree, cmds, child));
	else if (tree->type == OPERATOR_T || tree->type == HEREDOC)
	{
		if (execute_cmds_with_operators_heredoc(tree, head, cmds, child) == -1)
			return (free_2d_array(cmds), -1);
	}
	wait(NULL);
	free_2d_array(cmds);
	return (0);
}
