/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_header.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maglagal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 14:51:08 by oait-laa          #+#    #+#             */
/*   Updated: 2024/05/22 19:32:04 by maglagal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef PARSE_HEADER_H
#define PARSE_HEADER_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>

typedef enum e_t_type
{
	OPERATOR_T,
	REDIRECTION_T,
	PARETHESIS_O,
	PARETHESIS_C,
	CMD_T
} t_t_type;

typedef struct s_token_array
{
	char *token;
	t_t_type type;
} t_token_array;

typedef struct s_stack
{
	t_token_array *token;
	int	head;
} t_stack;

typedef struct	s_env_vars {
	char *env_name;
	char *env_val;
	struct s_env_vars *next;
}				t_env_vars;

typedef struct s_token_tree
{
	int id;
	int cmd_count;
	char *token;
	t_t_type type;
	struct s_token_tree *left;
	struct s_token_tree *right;
} t_token_tree;


char	*get_next_line(int fd);
char	*ft_strdup(const char *s1);
char	*ft_strtrim(char const *s1, char const *set);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
size_t	ft_strlen(const char *s);
char	**ft_split(char const *s, char c);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strchr(const char *s, int c);
char	*ft_strjoin(char *s1, char *s2);
int		len(char **s);
void	write_error(char *str);
char	*ft_strtrim(char const *s1, char const *set);
int		check_cmd(char *str);
int		cd_command(char *path);
char	*remove__quotes(char *str);
void 	print_tree(t_token_tree *root, int level);
t_token_tree *create_node(char *token, t_t_type type);
t_token_array *tokenizer(char *input);
void	*free_alloc(char **bigstr, int l);
int		is_inside_quotes(char const *s, int i);
int		pwd_command();
int		echo_command(char *string);
t_env_vars  *export_command(char **tokens, t_env_vars *head, char **envp);
int		is_op(char *input);
int		count_token_len(char *input);
char	*get_token(char **input, int num, char *str);
int 	count_cmds(char *input);
int		handle_tokens(char **input, char *input_cpy, char **holder, int i);
int		handle_cmd(char **input, char *input_cpy, char **holder, int i);
t_stack	shunting_yard(t_token_array *tokens);
int		count_array(t_token_array *tokens);
t_token_tree	*build_tree(t_stack *stack);
int		count_env_vars(char **tokens);
void    print_env_variable(char *env_name, t_env_vars *head);
void    unset_command(t_env_vars **head, char *cmd);
void    env_command(t_env_vars *env_vars, char **envp);
char	**ft_split_one(char const *s, char c);
t_t_type	set_token_type(char *token);
int     execute(t_token_tree *tree, char **envp);
void	execute_pipe(char **envp, t_token_tree *left, t_token_tree *right);
int		exec_command(char **cmds, char **envp);
int		exec_normal_commands(t_token_tree *tree, char **envp);
int		scan_syntax(char **holder, int j);
int		unclosed_var(char *str, char c);
t_t_type	set_token_type(char *token);
void 	free_tree(t_token_tree *root);
char 	*wildcard(char *str);
int		has_wildcard(char *str);
void    execute_tree(t_token_tree *tree, char **envp);
void    execute_redirection(char *cmd, char *file_name, char **envp);
int		is_string(char *str);
void    export_without_arguments(t_env_vars *head, char **envp);
t_env_vars  *add_env_var(t_env_vars *last_env, char **tokens, int nbr_envs, t_env_vars *head);
t_env_vars *get_last_node(t_env_vars *head);
t_env_vars  *lst_add_element(char *token, t_env_vars *last_env, t_env_vars *head, int i);
void    display_envs_sorted(char **envp);

#endif