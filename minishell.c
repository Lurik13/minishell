/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcouchma <kcouchma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 09:13:20 by lribette          #+#    #+#             */
/*   Updated: 2024/03/06 09:02:45 by kcouchma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "./Pipex/pipex.h"

void	sigint_handler(int signal)
{
	if (signal == SIGINT)
	{
		g_signal = EXIT_SIGINT;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 1);
		rl_redisplay();
	}
}

char	**finishell_env(char **envp)
{
	char	**f_envp;
	int		i;

	f_envp = NULL;
	i = 0;
	if (!envp)
		return (NULL);
	while (envp[i])
		i++;
	if (!i)
		return (NULL);
	f_envp = ft_calloc(i + 2, sizeof(char *));
	if (!f_envp)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		f_envp[i] = ft_strdup(envp[i]);
		if (errno == MALLOC_ERROR)
			return (free_table(f_envp), NULL);
		i++;
	}
	f_envp[i] = ft_strdup("?=0");
	return (f_envp);
}

volatile int			g_signal;

static void	_finishell_core(t_struct *main, char *input)
{
	if (parsing(main, input) == EXIT_SUCCESS)
	{
		if (!main->parse.error)
		{
			ft_free_parsing(&main->parse);
			main->exit_code = executing(main);
		}
		ft_structclear(&main->args_list);
	}
}

static int	_ft_loop(t_struct *main)
{
	char		*input;

	main->exit_code = 0;
	input = readline(GREEN"finishell 🤯 > "RESET);
	if (g_signal)
	{
		main->common.f_envp = ch_exit_code(g_signal, main->common.f_envp);
		g_signal = 0;
	}
	if (!input)
	{
		write(1, "exit\n", 5);
		return (1);
	}
	_finishell_core(main, input);
	if (errno == MALLOC_ERROR)
	{
		main->exit_code = errno;
		printf ("\x1b[38;2;136;87;66;1mfinishell 💩: malloc failure\n");
		return (errno);
	}
	main->common.f_envp = ch_exit_code(main->exit_code, main->common.f_envp);
	return (EXIT_SUCCESS);
}

int	main(int argc, char **argv, char **envp)
{
	t_struct	main;

	g_signal = 0;
	(void)argv;
	if (argc != 1)
	{
		printf("Just write \x1b[38;2;200;100;0;1m./minishell\e[0m\n");
		exit(EXIT_FAILURE);
	}
	main.common.f_envp = finishell_env(envp);
	main.common.pwd = getcwd(NULL, 0);
	while (1)
	{
		errno = 0;
		signal(SIGINT, &sigint_handler);
		signal(SIGQUIT, SIG_IGN);
		if (_ft_loop(&main))
			break ;
	}
	rl_clear_history();
	free_table(main.common.f_envp);
	free(main.common.pwd);
	return (main.exit_code);
}
