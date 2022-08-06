/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ple-stra <ple-stra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 21:27:15 by ple-stra          #+#    #+#             */
/*   Updated: 2022/08/06 18:58:29 by ple-stra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ft_perror(t_pip pip, const char *error_str)
{
	ft_putstr_fd(pip.exec_name, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(error_str, 2);
	ft_putstr_fd("\n", 2);
	return (1);
}

int	ft_fperror(t_pip pip, const char *filename, const char *error_str)
{
	ft_putstr_fd(pip.exec_name, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(filename, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(error_str, 2);
	ft_putstr_fd("\n", 2);
	return (1);
}

int	ft_perror_errno(t_pip pip)
{
	if (errno == 0)
		return (ft_perror(pip, ERR_UNKNOWN));
	perror(pip.exec_name);
	return (errno);
}

int	ft_fperror_errno(t_pip pip, const char *filename)
{
	if (errno == 0)
		return (ft_fperror(pip, filename, ERR_UNKNOWN));
	ft_putstr_fd(pip.exec_name, 2);
	ft_putstr_fd(": ", 2);
	perror(filename);
	return (errno);
}

void	ft_exit(t_pip pip, int err_num)
{
	(void)pip;
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	exit(err_num);
}
