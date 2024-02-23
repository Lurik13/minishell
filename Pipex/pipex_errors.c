/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcouchma <kcouchma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 17:37:25 by kcouchma          #+#    #+#             */
/*   Updated: 2024/02/23 11:03:02 by kcouchma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// void	ft_free_pipex(t_pipex *pipex)
// {
// 	// if (pipex->infile)
// 	// 	free(pipex->infile);
// 	// if (pipex->outfile)
// 	// 	free(pipex->outfile);
// 	ft_freetable(pipex->paths);
// }

void	ft_freetable(char **table)
{
	int	i;

	i = 0;
	if (!table)
		return ;
	while (table[i])
	{
		free(table[i]);
		i++;
	}
	free(table);
}

void	ft_command_fail(t_pipex *pipex, t_args *child_args, t_struct *main)
{
	char	*msg;

	if (!child_args->command_name)
		write(STDERR_FILENO, "finishell: command not found: ''\n", 33);
	else
	{
		msg = ft_strjoin3(
				"\x1b[38;2;255;0;0;1mfinishell 🤬: command not found: ",
				child_args->command_name, "\e[0m\n");
		if (!msg)//may need to set malloc error here
			write(STDERR_FILENO, "finishell: command not found\n", 29);
		else
		{
			write(STDERR_FILENO, msg, ft_strlen(msg));
			free(msg);
		}
	}
	ft_freetable(pipex->paths);
	ft_structclear(&main->args_list);
	free_envp(main->common.envp);
	exit(FILENOTFOUND);
}

int	ft_byedoc(t_pipex *pipex, t_args *child_args, int exit_code)
{
	char	*msg;

	g_signal = 0;
	pipex->exit_code = EXIT_FAILURE;
	msg = ft_strjoin3
		("\x1b[38;2;255;0;0;1mfinishell 🤬: warning: here-doc wanted `",
			child_args->input_files[0], "'\n\e[0m");
	if (!msg)//may need to set malloc error here
		pipex->exit_code = EXIT_FAILURE; //Need to change to fatal error
		// write(STDERR_FILENO,
		// 	"\x1b[38;2;255;0;0;1mfinishell 🤬: warning: here-doc wanted \n",
		// 	25);
	write(STDERR_FILENO, msg, ft_strlen(msg));
	free(msg);
	return (exit_code);
}

int	ft_pipex_error(t_pipex *pipex, t_struct *main, int exit_code)
{
	ft_freetable(pipex->paths);
	if (pipex->pid == 0)
	{
		ft_structclear(&main->args_list);
		free_envp(main->common.envp);
		exit(exit_code);
	}
	return (unlink_hds());
	// return (EXIT_FAILURE);
}

int	unlink_hds(void)
{
	int		i;
	char	*filename;
	char	*string_i;

	i = 0;
	filename = NULL;
	string_i = NULL;
	while (i < 1024)
	{
		string_i = ft_itoa(i);
		if (!string_i)
			return (EXIT_FAILURE);
		filename = ft_strjoin("./Pipex/temp_", string_i);
		free(string_i);
		unlink(filename);
		free(filename);
		i++;
	}
	return (0);
}
