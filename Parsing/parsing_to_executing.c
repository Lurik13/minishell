/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_to_executing.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lribette <lribette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 10:56:04 by lribette          #+#    #+#             */
/*   Updated: 2024/02/15 17:38:38 by lribette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../minishell.h"

void	init_arg(t_args *cmd, t_struct *main, int start, int end)
{
	int	s;

	s = start;
	cmd->command_name = NULL;
	while (s < end)
	{
		if (main->parse.types[s] == COMMAND)
		{
			cmd->command_name = ft_strdup(main->parse.argv[s]);
			break ;
		}
		s++;
	}
	cmd->flags = fill_type(OPTION, main, start, end);
	cmd->input_redirs = fill_type(INPUT_REDIR, main, start, end);
	cmd->input_files = fill_type(INPUT_FILE, main, start, end);
	cmd->output_redirs = fill_type(OUTPUT_REDIR, main, start, end);
	cmd->output_files = fill_type(OUTPUT_FILE, main, start, end);
	fill_strings(cmd, main, start, end);
	cmd->command_table = fill_table(main, start, end);
	cmd->is_builtin = is_builtin(cmd->command_name);
	//int		nb_of_inputs;
	//int		nb_of_outputs;
}

t_args	*ft_structnew(t_struct *main, int start, int end)
{
	t_args	*add2list;

	add2list = malloc(sizeof(t_args));
	if (add2list)
	{
		init_arg(add2list, main, start, end);
		add2list->next = NULL;
	}
	return (add2list);
}

t_args	*parsing_to_executing(t_struct *main)
{
	t_args	*cmd;
	t_args	*temp;
	int		i;
	int		start;
	int		end;

	i = 0;
	end = 0;
	cmd = NULL;
	printf("number_of_commands = %d\n", main->parse.number_of_commands);
	while (i < main->parse.number_of_commands)
	{
		if (main->parse.types[end] == PIPE)
			end++;
		start = end;
		while (end < main->parse.argc && main->parse.types[end] != PIPE)
			end++;
		temp = ft_structnew(main, start, end);
		if (!temp)
			return (NULL);
		if (i != 0)
			temp->next = cmd;
		cmd = temp;
		i++;
	}
	// n'oublie pas de free la liste chainee!
	return (cmd);
}
