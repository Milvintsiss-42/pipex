/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ple-stra <ple-stra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 08:12:57 by ple-stra          #+#    #+#             */
/*   Updated: 2022/08/25 12:11:17 by ple-stra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "libft.h"
# include <unistd.h>
# include <stdio.h>
# include <sys/errno.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <string.h>

// ERR_CSM errors are custom errors that need arguments and to be printed with
// ft_print_fd.
// Others errors can be printed with perror.
# define ERR_WRG_NB_ARG			"Incorrect number of arguments"
# define ERR_CMD_NOT_FOUND		"Command not found"
# define ERR_CSM_EOF_IN_HERE_DOC	"%s: warning: here-document at line %d\
 delimited by end-of-file (wanted `%s')\n"
# define ERR_INV_EXT			"Invalid file extension"
# define ERR_INV_FILE			"Invalid file"
# define ERRNO_CANT_OPEN_FILE	"Cannot open file"
# define ERRNO_INSUFFICIENT_MEM	"Insufficient memory"
# define ERR_UNKNOWN			"Unknown error"

typedef struct s_pip
{
	const char	*exec_name;
	char		**env;
	char		*path;
	char const	**cmds_w_args;
	char const	*last_cmds_w_args;
	const char	*infile;
	const char	*outfile;
	int			is_heredoc;
	const char	*limiter;
	int			*pipes;
	int			nb_pipes;
	pid_t		*pids;
}	t_pip;

int			ft_perror(t_pip pip, const char *error_str);
int			ft_fperror(t_pip pip, const char *filename, const char *error_str);
int			ft_perror_errno(t_pip pip);
int			ft_fperror_errno(t_pip pip, const char *filename);

void		ft_exit(t_pip pip, int err_num, int is_child);

void		close_pipe(int fds[2]);
void		close_all_pipes(t_pip pip);
void		create_pipes(t_pip *pip);
void		set_here_doc_as_stdin(t_pip pip);
void		sets_pipe_as_stdin(t_pip *pip, int fds_pipe[2], int have_to_close);
void		sets_pipe_as_stdout(t_pip *pip, int fds_pipe[2], int have_to_close);

char		*get_path_from_env(char	**env);
char		*get_absolute_path(t_pip *pip, const char *file, char *path,
				int *s_errno);
const char	*get_exec_basename(const char *path);

void		child(t_pip *pip, const char *cmd_w_args, int pipein[2],
				int pipeout[2]);

#endif