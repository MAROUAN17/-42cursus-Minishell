/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_failure.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oait-laa <oait-laa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 13:32:17 by maglagal          #+#    #+#             */
/*   Updated: 2024/08/10 14:31:10 by oait-laa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parse_header.h"

void	print_err(char *string1, char *string2, char *string3)
{
	ft_putstr_fd(string1, 2);
	ft_putstr_fd(string2, 2);
	ft_putstr_fd(string3, 2);
}

int	check_ambiguous_without_quotes(char *old_filename, t_token_tree *tree)
{
	char	**cmds;

	cmds = NULL;
	if (has_quotes(tree->token, '\'') && has_quotes(tree->token, '\"'))
	{
		switch_tabs_to_spaces_nq(tree->token);
		cmds = ft_split(tree->token, ' ');
		if (!cmds)
			return (print_err(strerror(errno), "\n", NULL), ft_close(NULL,
					tree->head, tree), free(old_filename), exit(1), -1);
		if (count_2d_array_elements(cmds) > 1)
			return (free_2d_array(cmds),
				ambiguous_redirect_error(old_filename));
		free_2d_array(cmds);
	}
	return (0);
}

int	ambiguous_redirect_error(char *filename)
{
	print_err("minishell: ", filename, ": ambiguous redirect\n");
	free(filename);
	return (-1);
}

void	define_when_to_set(t_env_vars *env, char *to_set,
	char **cmds, t_token_tree *tree)
{
	char	**cmds_token;

	cmds_token = NULL;
	free(env->env_val);
	if (to_set && to_set[0] == '\0')
	{
		env->env_val = ft_strdup(to_set);
		if (!env->env_val && errno == ENOMEM)
			return (ft_close(cmds, tree->head, tree),
				exit(1));
		return ;
	}
	cmds_token = ft_split(to_set, ' ');
	if (!cmds_token && errno == ENOMEM)
		return (ft_close(cmds, tree->head, tree), exit(1));
	env->env_val = ft_strdup(cmds_token[count_2d_array_elements(
				cmds_token) - 1]);
	if (!env->env_val && errno == ENOMEM)
		return (ft_close(cmds, tree->head, tree),
			free_2d_array(cmds_token), exit(1));
	free_2d_array(cmds_token);
}

void	update_underscore_env(char *to_set, char **cmds, t_env_vars *head,
			t_token_tree *tree)
{
	t_env_vars	*env;

	env = search_for_env(&head, "_");
	if (env && ft_strcmp(env->env_val, "-n") && to_set)
		define_when_to_set(env, to_set, cmds, tree);
	else if (env && ft_strcmp(env->env_val, "-n") && !to_set)
	{
		free(env->env_val);
		env->env_val = ft_strdup(cmds[count_2d_array_elements(cmds) - 1]);
		if (!env->env_val && errno == ENOMEM)
			return (ft_close(cmds, tree->head, tree), exit(1));
	}
	else if (env && !ft_strcmp(env->env_val, "-n"))
	{
		free(env->env_val);
		env->env_val = ft_strdup("");
	}
}
