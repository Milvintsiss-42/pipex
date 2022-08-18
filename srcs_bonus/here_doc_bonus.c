/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ple-stra <ple-stra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 11:00:57 by ple-stra          #+#    #+#             */
/*   Updated: 2022/08/18 11:10:48 by ple-stra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	create_here_doc(t_pip *pip)
{
	int		fds_pipe[2];
	char	*line;

	if (pipe(fds_pipe) == -1)
		ft_exit(*pip, ft_perror_errno(*pip), 1);
	
	line = get_next_line(STDIN_FILENO);
	while (line)
	{
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
}
