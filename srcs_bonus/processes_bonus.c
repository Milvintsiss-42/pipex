/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ple-stra <ple-stra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 22:24:34 by ple-stra          #+#    #+#             */
/*   Updated: 2022/08/20 08:13:55 by ple-stra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	exec_command(t_pip *pip, const char *cmd_x_args)
{
	char	**args;
	char	*abs_path;

	if (!*cmd_x_args)
		ft_exit(*pip, ft_fperror(*pip, "", strerror(13)), 1);
	args = ft_split(cmd_x_args, ' ');
	if (!args)
		ft_exit(*pip, ft_perror_errno(*pip), 1);
	abs_path = get_absolute_path(pip, args[0], pip->path);
	if (!abs_path)
	{
		if (pip->s_errno == 2)
			ft_fperror(*pip, args[0], ERR_CMD_NOT_FOUND);
		else
			ft_fperror(*pip, args[0], strerror(pip->s_errno));
		ft_freesplit(args);
		ft_exit(*pip, pip->s_errno, 1);
	}
	else if (execve(abs_path, args, pip->env) == -1)
		ft_exit(*pip, ft_perror_errno(*pip), 1);
}

static void	sets_infile_as_stdin(t_pip *pip)
{
	int	fd_file;

	fd_file = open(pip->infile, O_RDONLY);
	if (fd_file == -1)
		ft_exit(*pip, ft_fperror_errno(*pip, pip->infile), 1);
	if (dup2(fd_file, STDIN_FILENO) == -1)
	{
		close(fd_file);
		ft_exit(*pip, ft_perror_errno(*pip), 1);
	}
	close(fd_file);
}

static void	sets_outfile_as_stdout(t_pip *pip)
{
	int	fd_file;
	int	oflag;

	if (pip->is_heredoc)
		oflag = O_WRONLY | O_CREAT | O_APPEND;
	else
		oflag = O_WRONLY | O_CREAT | O_TRUNC;
	fd_file = open(pip->outfile, oflag, 0644);
	if (fd_file == -1)
		ft_exit(*pip, ft_fperror_errno(*pip, pip->outfile), 1);
	if (dup2(fd_file, STDOUT_FILENO) == -1)
	{
		close(fd_file);
		ft_exit(*pip, ft_perror_errno(*pip), 1);
	}
	close(fd_file);
}

void	child(t_pip *pip, const char *cmd_w_args, int pipein[2], int pipeout[2])
{
	if (cmd_w_args == pip->cmds_w_args[0] && pip->is_heredoc)
		set_here_doc_as_stdin(*pip);
	else if (cmd_w_args == pip->cmds_w_args[0])
		sets_infile_as_stdin(pip);
	else
		sets_pipe_as_stdin(pip, pipein, cmd_w_args == pip->last_cmds_w_args);
	if (cmd_w_args == pip->last_cmds_w_args)
		sets_outfile_as_stdout(pip);
	else
		sets_pipe_as_stdout(pip, pipeout, 1);
	exec_command(pip, cmd_w_args);
}
