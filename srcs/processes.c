/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ple-stra <ple-stra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 22:24:34 by ple-stra          #+#    #+#             */
/*   Updated: 2022/08/18 09:13:08 by ple-stra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	exec_command(t_pip *pip, const char *cmd_x_args)
{
	char	**args;
	char	*abs_path;

	if (!*cmd_x_args)
		ft_exit(*pip, ft_fperror(*pip, "", strerror(13)));
	args = ft_split(cmd_x_args, ' ');
	if (!args)
		ft_exit(*pip, ft_perror_errno(*pip));
	abs_path = get_absolute_path(pip, args[0], pip->path);
	if (!abs_path)
	{
		if (pip->s_errno == 2)
			ft_fperror(*pip, args[0], ERR_CMD_NOT_FOUND);
		else
			ft_fperror(*pip, args[0], strerror(pip->s_errno));
		ft_freesplit(args);
		ft_exit(*pip, pip->s_errno);
	}
	if (abs_path)
		if (execve(abs_path, args, pip->env) == -1)
			ft_exit(*pip, ft_perror_errno(*pip));
}

void	child_1(t_pip *pip, const char *infile, const char *cmd,
	int fds_pipe[2])
{
	int	fd_file;

	if (dup2(fds_pipe[1], STDOUT_FILENO) == -1)
	{
		close_pipe(fds_pipe);
		ft_exit(*pip, ft_perror_errno(*pip));
	}
	close_pipe(fds_pipe);
	fd_file = open(infile, O_RDONLY);
	if (fd_file == -1)
		ft_exit(*pip, ft_fperror_errno(*pip, infile));
	if (dup2(fd_file, STDIN_FILENO) == -1)
	{
		close(fd_file);
		ft_exit(*pip, ft_perror_errno(*pip));
	}
	close(fd_file);
	exec_command(pip, cmd);
}

void	child_2(t_pip *pip, const char *outfile, const char *cmd,
	int fds_pipe[2])
{
	int	fd_file;

	if (dup2(fds_pipe[0], STDIN_FILENO) == -1)
	{
		close_pipe(fds_pipe);
		ft_exit(*pip, ft_perror_errno(*pip));
	}
	close_pipe(fds_pipe);
	fd_file = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_file == -1)
		ft_exit(*pip, ft_fperror_errno(*pip, outfile));
	if (dup2(fd_file, STDOUT_FILENO) == -1)
	{
		close(fd_file);
		ft_exit(*pip, ft_perror_errno(*pip));
	}
	close(fd_file);
	exec_command(pip, cmd);
}

void	close_pipe(int fds[2])
{
	close(fds[0]);
	close(fds[1]);
}
