/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ple-stra <ple-stra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/29 15:44:27 by ple-stra          #+#    #+#             */
/*   Updated: 2022/08/25 12:10:15 by ple-stra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	init_pip_data(t_pip *pip, int argc, char const **argv, char **env)
{
	pip->is_heredoc = 0;
	if (argc == 0 || argv[0] == 0)
	{
		pip->exec_name = "";
		return ;
	}
	pip->exec_name = get_exec_basename(argv[0]);
	if (argc < 5)
		return ;
	pip->env = env;
	pip->path = get_path_from_env(env);
	pip->is_heredoc = ft_strncmp(argv[1], "here_doc", 9) == 0;
	pip->limiter = argv[2];
	pip->infile = argv[1];
	pip->outfile = argv[argc - 1];
	pip->pipes = 0;
	pip->nb_pipes = argc - 4 - pip->is_heredoc;
	pip->pids = 0;
	pip->cmds_w_args = argv + pip->is_heredoc + 2;
	pip->last_cmds_w_args = argv[pip->is_heredoc + 2 + pip->nb_pipes];
}

static void	create_pids_arr(t_pip *pip)
{
	pip->pids = malloc(sizeof(pid_t) * (pip->nb_pipes + 1));
	if (!pip->pids)
		ft_exit(*pip, ft_perror_errno(*pip), 0);
}

static void	create_childs(t_pip *pip)
{
	int	i;
	int	*pipein;
	int	*pipeout;

	i = 0;
	while (i <= pip->nb_pipes)
	{
		pip->pids[i] = fork();
		if (pip->pids[i] < 0)
			ft_exit(*pip, ft_perror_errno(*pip), 0);
		if (pip->pids[i] == 0)
		{
			if (i != 0)
				pipein = pip->pipes + (i - 1) * 2;
			else
				pipein = 0;
			if (i != pip->nb_pipes)
				pipeout = pip->pipes + i * 2;
			else
				pipeout = 0;
			child(pip, pip->cmds_w_args[i], pipein, pipeout);
			break ;
		}
		i++;
	}
}

static void	wait_childs(t_pip *pip)
{
	int		i;
	int		status_code;

	i = 0;
	while (i < pip->nb_pipes)
		waitpid(pip->pids[i++], NULL, 0);
	waitpid(pip->pids[i], &status_code, 0);
	if (WIFEXITED(status_code))
		ft_exit(*pip, WEXITSTATUS(status_code), 0);
	ft_exit(*pip, 0, 0);
}

int	main(int argc, char const **argv, char **env)
{
	t_pip	pip;

	init_pip_data(&pip, argc, argv, env);
	if (argc < (5 + pip.is_heredoc))
		return (ft_perror(pip, ERR_WRG_NB_ARG));
	create_pipes(&pip);
	create_pids_arr(&pip);
	create_childs(&pip);
	close_all_pipes(pip);
	wait_childs(&pip);
	return (0);
}
