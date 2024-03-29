/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 12:10:57 by kcouchma          #+#    #+#             */
/*   Updated: 2024/03/12 19:19:15 by lribette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../Pipex/pipex.h"

static int	_sorted(char **ex)
{
	int	i;

	i = 0;
	while (ex[i] && ex[i + 1])
	{
		if (ft_strncmp(ex[i], ex[i + 1], ft_strlen(ex[i])) > 0)
			return (1);
		i++;
	}
	return (0);
}

static void	_sort_export(char **ex)
{
	int		i;
	char	*temp;

	i = 0;
	temp = NULL;
	while (_sorted(ex))
	{
		while (ex[i] && ex[i + 1])
		{
			if (ft_strcmp(ex[i], ex[i + 1]) > 0)
			{
				temp = ex[i];
				ex[i] = ex[i + 1];
				ex[i + 1] = temp;
				temp = NULL;
			}
			i++;
		}
		i = 0;
	}
}

static void	_print_(char *var)
{
	size_t	j;

	j = ft_find_eq(var);
	write(STDOUT_FILENO, "declare -x ", 11);
	if (j != ft_strlen(var))
	{
		j++;
		write(STDOUT_FILENO, var, j);
		write(STDOUT_FILENO, "\"", 1);
		write(STDOUT_FILENO, var + j, ft_strlen(var) - j);
		write(STDOUT_FILENO, "\"", 1);
	}
	else
		write(STDOUT_FILENO, var, ft_strlen(var));
	write(STDOUT_FILENO, "\n", 1);
}

static int	_print_export(char **f_envp)
{
	int		i;
	char	**export;

	i = -1;
	if (!f_envp)
		return (EXIT_SUCCESS);
	export = malloc(sizeof(char *) * (ft_tablen(f_envp) + 1));
	if (!export)
		return (EXIT_FAILURE);
	while (f_envp[++i])
	{
		export[i] = ft_strdup(f_envp[i]);
		if (!export[i])
			return (free_table(export), EXIT_FAILURE);
	}
	export[i] = NULL;
	_sort_export(export);
	i = -1;
	while (export[++i])
	{
		if (ft_strncmp(export[i], "_=", 2) != 0
			&& ft_strncmp(export[i], "?=", 2) != 0)
			_print_(export[i]);
	}
	return (free_table(export), EXIT_SUCCESS);
}

int	ft_export(t_args *arg, t_struct *main)
{
	int	j;

	j = 1;
	if (!arg->args)
	{
		_print_export(main->common.f_envp);
		if (errno == MALLOC_ERROR)
			return (errno);
	}
	while (arg->command_table[j])
	{
		if (ft_mod_fevnp(arg->command_table[j], &main->common.f_envp) == 1)
		{
			if (errno == MALLOC_ERROR)
				return (errno);
			else
				return (EXIT_FAILURE);
		}
		j++;
	}
	return (EXIT_SUCCESS);
}
