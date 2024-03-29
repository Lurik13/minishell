/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_type.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 10:53:33 by lribette          #+#    #+#             */
/*   Updated: 2024/03/05 09:15:21 by lribette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

static int	_count_type(int type, t_struct *main, int start, int end)
{
	int	counter;

	counter = 0;
	while (start != end)
	{
		if (main->parse.types[start] == type)
			counter++;
		start++;
	}
	return (counter);
}

char	**fill_type(int type, t_struct *main, int start, int end)
{
	char	**tab;
	int		nb_of_this_type;
	int		i;

	nb_of_this_type = _count_type(type, main, start, end);
	if (!nb_of_this_type)
		return (NULL);
	tab = ft_calloc((nb_of_this_type) + 1, sizeof(char *));
	if (!tab)
		return (NULL);
	i = 0;
	while (start != end)
	{
		if (main->parse.types[start] == type)
		{
			tab[i] = ft_strdup(main->parse.argv[start]);
			if (errno == MALLOC_ERROR)
				return (free_table(tab), NULL);
			i++;
		}
		start++;
	}
	tab[i] = NULL;
	return (tab);
}

static int	_not_redirections(t_struct *main, int start, int end)
{
	int	counter;

	counter = 0;
	while (start != end)
	{
		if (main->parse.types[start] != INPUT_REDIR
			&& main->parse.types[start] != INPUT_FILE
			&& main->parse.types[start] != OUTPUT_REDIR
			&& main->parse.types[start] != OUTPUT_FILE)
			counter++;
		start++;
	}
	return (counter);
}

static char	**_dup_table(t_struct *main, int start, int end, char **tab)
{
	int	i;

	i = 0;
	while (start != end)
	{
		if (main->parse.types[start] != INPUT_REDIR
			&& main->parse.types[start] != INPUT_FILE
			&& main->parse.types[start] != OUTPUT_REDIR
			&& main->parse.types[start] != OUTPUT_FILE)
		{
			tab[i] = ft_strdup(main->parse.argv[start]);
			if (errno == MALLOC_ERROR)
				return (free_table(tab), NULL);
			i++;
		}
		start++;
	}
	tab[i] = NULL;
	return (tab);
}

char	**fill_table(t_struct *main, int start, int end)
{
	char	**tab;
	int		nb_of_this_type;

	nb_of_this_type = _not_redirections(main, start, end);
	if (!nb_of_this_type)
		return (NULL);
	tab = ft_calloc((nb_of_this_type) + 1, sizeof(char *));
	if (!tab)
		return (NULL);
	tab = _dup_table(main, start, end, tab);
	return (tab);
}
