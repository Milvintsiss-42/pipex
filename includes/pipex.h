/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ple-stra <ple-stra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/29 15:45:28 by ple-stra          #+#    #+#             */
/*   Updated: 2022/08/18 05:53:02 by ple-stra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft.h"
# include <unistd.h>
# include <stdio.h>
# include <sys/errno.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <string.h>

# define ERR_WRG_NB_ARG			"Incorrect number of arguments"
# define ERR_CMD_NOT_FOUND		"Command not found"
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
	int			s_errno;
}	t_pip;

int			ft_perror(t_pip pip, const char *error_str);
int			ft_fperror(t_pip pip, const char *filename, const char *error_str);
int			ft_perror_errno(t_pip pip);
int			ft_fperror_errno(t_pip pip, const char *filename);

void		ft_exit(t_pip pip, int err_num);

char		*get_path_from_env(char	**env);
char		*get_absolute_path(t_pip *pip, const char *file, char *path);
const char	*get_exec_basename(const char *path);

void		child_1(t_pip *pip, const char *infile, const char *cmd,
				int fds_pipe[2]);
void		child_2(t_pip *pip, const char *outfile, const char *cmd,
				int fds_pipe[2]);
void		close_pipe(int fds[2]);

#endif