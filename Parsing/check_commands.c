/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 12:12:01 by lribette          #+#    #+#             */
/*   Updated: 2024/02/08 19:06:08 by lribette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

static void	_file(t_parsing *parse, int i)
{
	if (parse->types[i - 1] == REDIRECTION)
		printf("Type = %d\n", parse->types[i]);
	if (i && parse->types[i - 1] == REDIRECTION
		&& parse->types[i] != SEPARATOR)
		{
			parse->types[i] = R_FILE;
			parse->nb_of_redir++;
		}
}

static int	_not_separator(t_parsing *parse, int i)
{
	if (parse->argv[i][0] != '-' || i == 0)
	{
		parse->types[i] = COMMAND;
		i++;
	}
	while (i < parse->argc && parse->types[i] != SEPARATOR)
	{
		if (i && parse->types[i - 1] == PIPE)
			parse->types[i] = COMMAND;
		else if (parse->argv[i][0] == '-')
		{
			parse->nb_of_flags++;
			parse->types[i] = OPTION;
			_file(parse, i);
		}
		else
			parse->types[i] = ARGUMENT;
		i++;
	}
	return (i);
}

static int	_separator(t_parsing *parse, int i)
{
	if (!ft_strcmp(parse->argv[i], "|"))
	{
		parse->number_of_commands++;
		parse->types[i] = PIPE;
	}
	else if (!ft_strcmp(parse->argv[i], "="))
		parse->types[i] = EQUAL;
	else
		parse->types[i] = REDIRECTION;
	return (i + 1);
}

int	check_commands(t_parsing *parse)
{
	int	i;

	i = 0;
	while (i < parse->argc)
	{
		if (parse->types[i] != SEPARATOR)
			i = _not_separator(parse, i);
		if (parse->types[i] == SEPARATOR)
			i = _separator(parse, i);
	}

	// a prendre avec des pincettes !!!
	if (parse->types[parse->argc - 1] == REDIRECTION)
	{
		printf("%sbash: syntax error near unexpected token `newline'%s\n", 
			RED, RESET);
		//ft_free_parse(parse);
		return (0);
		//exit(EXIT_FAILURE);
	}
	return (0);
}
