/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ple-stra <ple-stra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/29 15:44:27 by ple-stra          #+#    #+#             */
/*   Updated: 2022/08/18 08:46:32 by ple-stra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	init_pip_data(t_pip *pip, int argc, char const **argv, char **env)
{
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
	pip->cmds_w_args = argv + pip->is_heredoc + 2;
	pip->last_cmds_w_args = argv[pip->is_heredoc + 2 + pip->nb_pipes];
	pip->s_errno = 0;
}

static void	create_pipes(t_pip *pip)
{
	int	i;

	pip->pipes = malloc(sizeof(int) * pip->nb_pipes * 2);
	if (!pip->pipes)
		ft_exit(*pip, ft_perror_errno(*pip));
	i = 0;
	while (i < pip->nb_pipes)
	{
		if (pipe(pip->pipes + i * 2) == -1)
			ft_exit(*pip, ft_perror_errno(*pip));
		i++;
	}
}

int	main(int argc, char const **argv, char **env)
{
	t_pip	pip;
	int		pid[2];
	int		status_code;

	init_pip_data(&pip, argc, argv, env);
	if (argc < (5 + pip.is_heredoc))
		return (ft_perror(pip, ERR_WRG_NB_ARG));
	create_pipes(&pip);
	pid[0] = fork();
	if (pid[0] == 0)
		child_1(&pip, pip.infile, pip.cmds_w_args[0], pip.pipes);
	pid[1] = fork();
	if (pid[1] == 0)
		child_2(&pip, pip.outfile, pip.cmds_w_args[1], pip.pipes);
	close_all_pipes(&pip);
	if (pid[0] == -1 || pid[1] == -1)
		return (ft_perror_errno(pip));
	waitpid(pid[0], NULL, 0);
	waitpid(pid[1], &status_code, 0);
	if (WIFEXITED(status_code))
		ft_exit(pip, WEXITSTATUS(status_code));
	ft_exit(pip, 0);
	return (0);
}
