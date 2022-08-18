/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ple-stra <ple-stra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/29 15:44:27 by ple-stra          #+#    #+#             */
/*   Updated: 2022/08/18 07:04:38 by ple-stra         ###   ########.fr       */
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
	pip->pipes = 0;
	pip->nb_pipes = argc - 4 - pip->is_heredoc;
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

static void	close_all_pipes(t_pip *pip)
{
	int	i;

	i = 0;
	while (i < pip->nb_pipes)
	{
		close_pipe(pip->pipes + i * 2);
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
		child_1(&pip, argv[1], argv[2], pip.pipes);
	pid[1] = fork();
	if (pid[1] == 0)
		child_2(&pip, argv[4], argv[3], pip.pipes);
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
