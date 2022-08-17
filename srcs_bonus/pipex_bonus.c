/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ple-stra <ple-stra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/29 15:44:27 by ple-stra          #+#    #+#             */
/*   Updated: 2022/08/17 20:06:05 by ple-stra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	init_pip_data(t_pip *pip, char const **argv, char **env)
{
	pip->exec_name = get_exec_basename(argv[0]);
	pip->env = env;
	pip->path = get_path_from_env(env);
	pip->s_errno = 0;
}

int	main(int argc, char const **argv, char **env)
{
	t_pip	pip;
	int		pid[2];
	int		fds_pipe[2];
	int		status_code;

	init_pip_data(&pip, argv, env);
	if (argc != 5)
		return (ft_perror(pip, ERR_WRG_NB_ARG));
	if (pipe(fds_pipe) == -1)
		return (ft_perror_errno(pip));
	pid[0] = fork();
	if (pid[0] == 0)
		child_1(&pip, argv[1], argv[2], fds_pipe);
	pid[1] = fork();
	if (pid[1] == 0)
		child_2(&pip, argv[4], argv[3], fds_pipe);
	close_pipe(fds_pipe);
	if (pid[0] == -1 || pid[1] == -1)
		return (ft_perror_errno(pip));
	waitpid(pid[0], NULL, 0);
	waitpid(pid[1], &status_code, 0);
	if (WIFEXITED(status_code))
		ft_exit(pip, WEXITSTATUS(status_code));
	ft_exit(pip, 0);
	return (0);
}
