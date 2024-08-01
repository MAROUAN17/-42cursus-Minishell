/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maglagal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 14:50:42 by oait-laa          #+#    #+#             */
/*   Updated: 2024/08/01 10:48:10 by maglagal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_header.h"

void	*free_alloc(char **bigstr, int l)
{
	while (l > 0)
	{
		l--;
		free(bigstr[l]);
	}
	free(bigstr);
	return (NULL);
}

void	null_input(t_env_vars *head)
{
	if (errno == ENOMEM)
		readline_allocation_failure(head);
	else
		eof_pressed(&head);
}

int	tokenize_and_build_execute_tree(char *input, t_env_vars **head,
	char **envp)
{
	t_token_array	*token_array;
	t_token_tree	*ast_tree;
	t_stack			postfix_stack;

	token_array = tokenizer(&input, *head);
	if (!token_array)
		return (-1);
	postfix_stack = shunting_yard(token_array);
	ast_tree = build_tree(&postfix_stack, envp, head);
	ast_tree->head = head;
	execute_tree(ast_tree, ast_tree->head, 1);
	free_tree(ast_tree);
	g_is_heredoc[0] = 0;
	g_is_heredoc[1] = 0;
	return (0);
}

t_env_vars	*initialize_main_variables(char **envp)
{
	t_env_vars	*head;
	t_env_vars	*oldpwd;

	g_is_heredoc[0] = 0;
	g_is_heredoc[1] = 0;
	head = create_lst(envp);
	handle_shlvl(head);
	oldpwd = search_for_env_var(&head, "OLDPWD");
	if (oldpwd)
	{
		free(oldpwd->env_val);
		oldpwd->env_val = NULL;
	}
	define_signals();
	return (head);
}

int	main(int argc, char **argv, char **envp)
{
	t_env_vars			*head;
	char				*input;

	(void)argc;
	(void)argv;
	head = initialize_main_variables(envp);
	rl_catch_signals = 0;
	while (1)
	{
		input = readline("Minishell$ ");
		if (input == NULL)
			null_input(head);
		if (input[0] == '\0' || syntax_error_check(head, input) == -1)
		{
			free(input);
			continue ;
		}
		add_history(input);
		if (g_is_heredoc[1] == 1)
			define_exit_status(head, "1");
		if (tokenize_and_build_execute_tree(input, &head, envp) == -1)
			continue ;
	}
	return (0);
}
