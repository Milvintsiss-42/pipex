/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ple-stra <ple-stra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 23:13:56 by ple-stra          #+#    #+#             */
/*   Updated: 2022/08/25 11:59:17 by ple-stra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*ft_join_paths(t_pip *pip, char const *p1, size_t len_p1,
	char const *p2)
{
	char	*str;
	size_t	len1;
	size_t	len2;
	int		is_finished;

	if (!p1 || !p2)
		return (NULL);
	len1 = len_p1;
	is_finished = *(p1 + len1 - 1) == '/';
	len2 = ft_strlen(p2);
	str = malloc(sizeof(char) * (len1 + (!is_finished) + len2 + 1));
	if (!str)
		exit(ft_perror_errno(*pip));
	while (len_p1--)
		*str++ = *p1++;
	if (!is_finished)
		*str++ = '/';
	while (*p2)
		*str++ = *p2++;
	*str = 0;
	return (str - (len1 + (!is_finished) + len2));
}

static char	*from_path(t_pip *pip, const char *file, char *path)
{
	char	*abs_path;
	char	*next;
	int		is_last;

	is_last = 0;
	while (*path)
	{
		next = ft_strchr(path, ':');
		if (next == 0)
			next = ft_strchr(path, '\0');
		if (next - path <= 1)
		{
			path = next + 1;
			continue ;
		}
		abs_path = ft_join_paths(pip, path, next - path, file);
		if (access(abs_path, F_OK) == 0)
			return (abs_path);
		free(abs_path);
		if (*next == '\0')
			return (0);
		path = next + 1;
	}
	return (0);
}

char	*get_absolute_path(t_pip *pip, const char *file, char *path,
	int *s_errno)
{
	char	*abs_path;

	abs_path = 0;
	if (access(file, X_OK) == 0)
	{
		abs_path = ft_strdup(file);
		if (!abs_path)
			exit(ft_perror_errno(*pip));
		return (abs_path);
	}
	*s_errno = errno;
	if (path)
		abs_path = from_path(pip, file, path);
	if (abs_path)
	{
		*s_errno = 0;
		return (abs_path);
	}
	return (0);
}

char	*get_path_from_env(char	**env)
{
	while (*env)
	{
		if (ft_strncmp(*env, "PATH=", 5) == 0)
			return (*env + 5);
		env++;
	}
	return (0);
}

const char	*get_exec_basename(const char *path)
{
	const char	*last_separator;

	while (*path)
	{
		if (*path == '/')
			last_separator = path;
		path++;
	}
	return (++last_separator);
}
