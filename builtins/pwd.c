/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcouchma <kcouchma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 14:33:22 by kcouchma          #+#    #+#             */
/*   Updated: 2024/02/02 17:00:52 by kcouchma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int pwd(t_pipex *pipex)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		printf("%s\n", pipex->pwd_origin);
	else
		printf("%s\n", pwd);
	free(pwd);
	return (0);
}
