/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ple-stra <ple-stra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 11:00:57 by ple-stra          #+#    #+#             */
/*   Updated: 2022/08/22 13:13:15 by ple-stra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static int	is_end_of_input(t_pip pip, char *line, size_t len)
{
	int	is_end_of_input;

	*(line + len - 1) = 0;
	is_end_of_input = ((*(pip.limiter) != 0 ^ len == 1)
			&& ft_strncmp(line, pip.limiter, len - 1) == 0);
	*(line + len - 1) = '\n';
	return (is_end_of_input);
}

static void	clean_and_handle_errors(t_pip pip, char *line, int lines_count)
{
	free(line);
	if (errno != 0 && errno != 22)
		ft_exit(pip, ft_perror_errno(pip), 0);
	if (!line)
		ft_printf_fd(STDERR_FILENO, ERR_CSM_EOF_IN_HERE_DOC,
			pip.exec_name, lines_count, pip.limiter);
}

static void	redirect_pipe_to_stdin(t_pip pip, int fds_pipe[2])
{
	close(fds_pipe[1]);
	if (dup2(fds_pipe[0], STDIN_FILENO) == -1)
		ft_exit(pip, ft_perror_errno(pip), 0);
	close(fds_pipe[0]);
}

// lines_count return numbers of lines wrotted to stdin as we do not have access
// 	to command history

// The pipe for here_doc is closed by ft_exit, this has to be done in an other
//  way for minishell

// When adapting this for minishell, careful with readline() that does not
// return the \n like get_next_line()
void	set_here_doc_as_stdin(t_pip pip)
{
	int		fds_pipe[2];
	char	*line;
	size_t	len;
	int		lines_count;

	lines_count = 0;
	if (pipe(fds_pipe) == -1)
		ft_exit(pip, ft_perror_errno(pip), 0);
	while (1)
	{
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		len = ft_strlen(line);
		if (is_end_of_input(pip, line, len))
			break ;
		write(fds_pipe[1], line, len);
		lines_count++;
		free(line);
	}
	clean_and_handle_errors(pip, line, lines_count);
	redirect_pipe_to_stdin(pip, fds_pipe);
}
