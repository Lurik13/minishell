/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 14:33:22 by kcouchma          #+#    #+#             */
/*   Updated: 2024/03/01 18:27:37 by lribette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../Pipex/pipex.h"

int	ft_pwd(t_struct *main)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		write(STDOUT_FILENO, main->common.pwd, ft_strlen(main->common.pwd));
		write(STDOUT_FILENO, "\n", ft_strlen(pwd));
		if (errno == MALLOC_ERROR)
			return (errno);
	}
	write(STDOUT_FILENO, pwd, ft_strlen(pwd));
	write(STDOUT_FILENO, "\n", ft_strlen(pwd));
	free(pwd);
	return (EXIT_SUCCESS);
}
