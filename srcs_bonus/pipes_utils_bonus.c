/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ple-stra <ple-stra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 08:42:45 by ple-stra          #+#    #+#             */
/*   Updated: 2022/08/18 08:42:47 by ple-stra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	close_pipe(int fds[2])
{
	close(fds[0]);
	close(fds[1]);
}

void	close_all_pipes(t_pip pip)
{
	int	i;

	i = 0;
	while (i < pip.nb_pipes)
	{
		close_pipe(pip.pipes + i * 2);
		i++;
	}
}

int	sets_pipe_as_stdin(t_pip *pip, int fds_pipe[2], int have_to_close)
{
	if (dup2(fds_pipe[0], STDIN_FILENO) == -1)
		ft_exit(*pip, ft_perror_errno(*pip));
	if (have_to_close)
		close_all_pipes(*pip);
}

int	sets_pipe_as_stdout(t_pip *pip, int fds_pipe[2], int have_to_close)
{
	if (dup2(fds_pipe[1], STDOUT_FILENO) == -1)
		ft_exit(*pip, ft_perror_errno(*pip));
	if (have_to_close)
		close_all_pipes(*pip);
}
