/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcouchma <kcouchma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 17:30:14 by kcouchma          #+#    #+#             */
/*   Updated: 2024/03/06 14:50:27 by kcouchma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**ft_extract_paths(char **f_envp)
{
	char	**paths;
	int		i;

	paths = NULL;
	i = 0;
	if (f_envp)
	{
		while (f_envp[i])
		{
			if (ft_strncmp("PATH=", f_envp[i], 5) == 0)
				paths = ft_split(f_envp[i] + 5, ':');
			i++;
		}
	}
	if (!paths)
	{
		paths = ft_split(". ", ' ');
		if (!paths)
			return (NULL);
	}
	return (paths);
}

char	*ft_strjoin3(char const *s1, char const *s2, char const *s3)
{
	char	*output;
	size_t	i;

	output = malloc((ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3) + 1));
	if (!output)
		return (NULL);
	i = 0;
	while (output && s1[i])
	{
		output[i] = ((char *)s1)[i];
		i++;
	}
	while (output && *s2)
	{
		output[i++] = *(char *)s2;
		s2++;
	}
	while (output && *s3)
	{
		output[i++] = *(char *)s3;
		s3++;
	}
	output[i] = '\0';
	return (output);
}

void	ft_execve(t_pipex *pipex, t_args *arg, t_struct *main)
{
	char	*cmd_path;
	int		i;

	cmd_path = 0;
	i = 0;
	if (arg->is_builtin)
	{
		builtins_executing(pipex, arg, main);
		return ;
	}
	while (pipex->paths[i])
	{
		if (arg->command_name[0] == '.' || arg->command_name[0] == '/')
			execve(arg->command_name, arg->command_table, main->common.f_envp);
		cmd_path = ft_strjoin3(pipex->paths[i], "/", arg->command_name);
		if (!cmd_path)
			ft_pipex_error(pipex, main, EXIT_FAILURE);
		execve(cmd_path, arg->command_table, main->common.f_envp);
		i++;
		free(cmd_path);
	}
}

void	ft_wait_parent(t_pipex *pipex, int nb_commands)
{
	int	i;

	i = 1;
	waitpid(pipex->pid_last, &pipex->exit_code, 0);
	pipex->exit_code = WEXITSTATUS(pipex->exit_code);
	while (i < nb_commands)
	{
		wait(NULL);
		i++;
	}
}

void	ft_closepipe(t_pipex *pipex)
{
	if (pipex->pipe_fd[0] != -1)
	{
		close(pipex->pipe_fd[0]);
		close(pipex->pipe_fd[1]);
	}
}
