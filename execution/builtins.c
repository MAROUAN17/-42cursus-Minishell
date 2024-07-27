/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oait-laa <oait-laa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 15:44:16 by maglagal          #+#    #+#             */
/*   Updated: 2024/07/27 09:51:37 by oait-laa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parse_header.h"

int	cd_command(char *path, t_env_vars *head)
{
	if (!path || !ft_strcmp(path, "~"))
	{
		if (home_case(head) == -1)
			return (-1);
	}
	else if (!ft_strcmp(path, "-"))
	{
		if (oldpwd_case(head) == -1)
			return (-1);
	}
	else
		return (changing_current_directory(path, head));
	return (0);
}

void	pwd_command(void)
{
	char	buff[PATH_MAX];

	if (!getcwd(buff, sizeof(buff)) && errno != ENOENT)
		return (print_err(strerror(errno), NULL, NULL));
	ft_putstr_fd(buff, 1);
	write(1, "\n", 1);
}

void	echo_command(t_token_tree *tree, char **cmds)
{
	int		i;
	int		new_line;

	i = 1;
	new_line = 1;
	if (print_echo_content(cmds, i, new_line) == 0)
		return (ft_close(cmds, tree->head, tree), exit(1));
}

int	export_command(char **cmds, t_env_vars **head, t_token_tree *tree)
{
	int	nbr_envs;

	if (!cmds[1])
	{
		export_without_arguments(*head, cmds, tree);
		return (0);
	}
	nbr_envs = count_env_vars(cmds);
	if (add_env_var(cmds, nbr_envs, head, tree) == -1)
		return (-1);
	return (0);
}

int	unset_command(char **cmds, t_token_tree *tree)
{
	int	i;

	i = 1;
	while (cmds[i])
	{
		if (delete_env(cmds[i], tree, cmds) == -1)
			return (-1);
		i++;
	}
	return (0);
}
