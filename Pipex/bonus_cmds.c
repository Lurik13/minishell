/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_cmds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 11:28:50 by kcouchma          #+#    #+#             */
/*   Updated: 2024/02/09 15:02:02 by lribette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_inputs(t_pipex *pipex)
{
	if (pipex->heredoc == 1) //will need to change this to if ">>"
		pipex->infile_fd = open("/tmp/temp", O_RDONLY);
	else
		pipex->infile_fd = open(pipex->infile, O_RDONLY);
	if (pipex->infile_fd == -1)
	{
		write(STDERR_FILENO, "pipex : open failed: input\n", 27);
		ft_freetable(pipex->child_args);
		ft_freetable(pipex->paths);
		free(pipex->pwd_origin);
		exit(EXIT_FAILURE);
	}
	if (dup2(pipex->infile_fd, STDIN_FILENO) == -1)
	{
		write(STDERR_FILENO, "pipex : dup2 failed: inputs\n", 28);
		ft_dup2_fail(pipex);
	}
}

void	ft_outputs(t_pipex *pipex)
{
	int	out_fd;

	if (pipex->heredoc == 1)
		out_fd = open(pipex->outfile, O_WRONLY | O_APPEND | O_CREAT, 0644);
	else
		out_fd = open(pipex->outfile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (out_fd == -1)
	{
		write(STDERR_FILENO, "pipex : open failed: output\n", 28);
		ft_freetable(pipex->child_args);
		ft_freetable(pipex->paths);
		free(pipex->pwd_origin);
		exit(EXIT_FAILURE);
	}
	if (dup2(out_fd, STDOUT_FILENO) == -1)
	{
		write(STDERR_FILENO, "pipex : dup2 failed: outputs\n", 29);
		ft_dup2_fail(pipex);
	}
}

void	ft_bonus_last_cmd(t_pipex *pipex)
{
	close(pipex->pipe_fd[1]);
	if (dup2(pipex->pipe_fd[0], STDIN_FILENO) == -1)
	{
		write(STDERR_FILENO, "pipex : dup2 failed: last_cmd\n", 30);
		ft_dup2_fail(pipex);
	}
	close(pipex->pipe_fd[0]);
	ft_outputs(pipex);
}

void	ft_bonus_first_cmd(t_pipex *pipex)
{
	close(pipex->pipe_fd[0]);
	if (dup2(pipex->temp_fd_out, STDOUT_FILENO) == -1)
	{
		write(STDERR_FILENO, "pipex : dup2 failed: first_cmd\n", 30);
		ft_dup2_fail(pipex);
	}
	close(pipex->temp_fd_out);
	ft_inputs(pipex);
}

void	ft_bonus_mid_cmd(t_pipex *pipex)
{
	close(pipex->pipe_fd[1]);
	if (dup2(pipex->temp_fd_out, STDOUT_FILENO) == -1)
	{
		write(STDERR_FILENO, "pipex : dup2 failed: mid_cmd\n", 30);
		ft_dup2_fail(pipex);
	}
	close(pipex->temp_fd_out);
	if (dup2(pipex->pipe_fd[0], STDIN_FILENO) == -1)
	{
		write(STDERR_FILENO, "pipex : dup2 failed: mid_cmd\n", 30);
		ft_dup2_fail(pipex);
	}
	close(pipex->pipe_fd[0]);
}

void	ft_bonus_forkchild(t_pipex *pipex, int i)
{
	pipex->pid = fork();
	if (pipex->pid == -1)
	{
		write(STDERR_FILENO, "pipex : fork failed\n", 20);
		ft_freetable(pipex->paths);
		free(pipex->pwd_origin);
		exit(EXIT_FAILURE);
	}
	if (pipex->pid == 0)
	{
		pipex->child_args = ft_split
			(pipex->args[pipex->commands + 1 + pipex->heredoc - i], ' ');
		if (!pipex->child_args)
			ft_parse_fail(pipex);
		if (i == 0)
			ft_bonus_last_cmd(pipex);
		else if (i == pipex->commands - 1)
			ft_bonus_first_cmd(pipex);
		else if (i > 0 && i < (pipex->commands - 1))
			ft_bonus_mid_cmd(pipex);
		if (!pipex->child_args[0])
			ft_command_fail(pipex);
		ft_execve(pipex);
		ft_command_fail(pipex);
	}
	if (pipex->pid != 0)
		if (pipex->temp_fd_out != -1)
			close(pipex->temp_fd_out);
	if (pipex->pid != 0)
		pipex->temp_fd_out = pipex->pipe_fd[1];
}

void	ft_wait_parent(t_pipex *pipex)
{
	int	i;

	i = 1;
	waitpid(pipex->pid_last, &pipex->exit_code, 0);
	//will need to converts to 1/0 from EXIT_SUCCESS/FAILURE:
	pipex->exit_code = WEXITSTATUS(pipex->exit_code);
	while (i < pipex->commands)
	{
		wait(NULL);
		i++;
	}
}
