#include "parse_header.h"

int exec_normal_commands(t_token_tree *tree, char **envp)
{
    int                 status;
    pid_t               pid;
    char                **cmds;

    status = 0;
    cmds = ft_split(tree->token, ' ');
    pid = fork();
    if (pid == -1)
        write(2, "fork() failed!!\n", 17); //fail
    else if (pid == 0)
    {    
        if (exec_command(cmds, envp) == -1)
            exit(1);
        else
            exit(0);
    }
    wait(&status);
    if (status == 1)
        return (-1);
    return (0);
}

int     execute(t_token_tree *tree, char **envp)
{
    if (tree->type == OPERATOR_T)
    {
        if (tree->left->type == CMD_T && tree->right->type == CMD_T)
        {
            if (!ft_strcmp(tree->token, "&&"))
            {
                if (!exec_normal_commands(tree->left, envp))
                    exec_normal_commands(tree->right, envp);
                else
                    return (-1);
            }
            else if (!ft_strcmp(tree->token, "||"))
            {
                if (exec_normal_commands(tree->left, envp) == -1)
                {
                    if (exec_normal_commands(tree->right, envp) == -1)
                        return (-1);
                }
            }
            else if (!ft_strcmp(tree->token, "|"))
                execute_pipe(envp, tree->left, tree->right);
        }
        else
            execute(tree->left, envp);
    }
    else if (tree->left)
        execute(tree->left, envp);
    else
    {    
        if (exec_normal_commands(tree, envp) == -1)
            return (-1);
    }
    return (0);
}
