/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_single_cmd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcouchma <kcouchma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 14:46:48 by kcouchma          #+#    #+#             */
/*   Updated: 2024/02/23 11:01:38 by kcouchma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
//compiled from the ft_first/last_cmd functions, can extract these sections
//to separate in/out functions to be called by both ft_first/last_cmd and single
//command functions, leaving only the pipe redirections in the 
//ft_first/last_cmd functions.

void	ft_single_cmd(t_pipex *pipex, t_args *child_args, t_struct *main)
{
	ft_inputs(pipex, child_args, main);
	ft_outputs(pipex, child_args, main);
	ft_execve(pipex, child_args, main);
	ft_command_fail(pipex, child_args, main);
}
