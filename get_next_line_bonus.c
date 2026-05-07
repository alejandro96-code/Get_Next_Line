/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejanr2 <alejanr2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 18:50:00 by alejanr2          #+#    #+#             */
/*   Updated: 2026/05/07 14:12:31 by alejanr2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*line_from_stash(char *stash)
{
	char	*line;
	size_t	i;

	i = 0;
	if (stash[i] == '\0')
		return (NULL);
	while (stash[i] != '\0' && stash[i] != '\n')
		i++;
	if (stash[i] == '\n')
		i++;
	line = malloc(i + 1);
	if (!line)
		return (NULL);
	i = 0;
	while (stash[i] != '\0' && stash[i] != '\n')
	{
		line[i] = stash[i];
		i++;
	}
	if (stash[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

static char	*stash_after_line(char *stash)
{
	size_t	i;
	size_t	j;
	char	*rest;

	i = 0;
	while (stash[i] != '\0' && stash[i] != '\n')
		i++;
	if (stash[i] == '\n')
		i++;
	rest = malloc(ft_strlen(stash + i) + 1);
	if (!rest)
		return (free(stash), NULL);
	j = 0;
	while (stash[i] != '\0')
		rest[j++] = stash[i++];
	rest[j] = '\0';
	free(stash);
	return (rest);
}

static char	*read_to_stash(int fd, char *stash)
{
	char	*buffer;
	int		bytes_read;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (free(stash), NULL);
	bytes_read = 1;
	while (bytes_read > 0 && ft_strchr(stash, '\n') == 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
			return (free(buffer), free(stash), NULL);
		buffer[bytes_read] = '\0';
		stash = ft_strjoin(stash, buffer);
		if (!stash)
			return (free(buffer), NULL);
	}
	free(buffer);
	return (stash);
}

char	*get_next_line(int fd)
{
	static char		*stash[OPEN_MAX];
	char			*line;

	if (fd < 0 || fd >= OPEN_MAX || BUFFER_SIZE <= 0)
		return (NULL);
	if (!stash[fd])
	{
		stash[fd] = malloc(1);
		if (!stash[fd])
			return (NULL);
		stash[fd][0] = '\0';
	}
	stash[fd] = read_to_stash(fd, stash[fd]);
	if (!stash[fd])
		return (NULL);
	line = line_from_stash(stash[fd]);
	stash[fd] = stash_after_line(stash[fd]);
	if (!line || !stash[fd])
		return (free(line), free(stash[fd]), stash[fd] = NULL, NULL);
	if (stash[fd][0] == '\0')
		return (free(stash[fd]), stash[fd] = NULL, line);
	return (line);
}

/*
#include <fcntl.h>
#include <stdio.h>

int	main(int argc, char **argv)
{
	int		*fds;
	char	**lines;
	int		i;
	int		active;

	if (argc < 2)
		return (1);
	fds = malloc(sizeof(int) * (argc - 1));
	lines = malloc(sizeof(char *) * (argc - 1));
	if (!fds || !lines)
		return (free(fds), free(lines), 1);
	i = 0;
	while (i < argc - 1)
	{
		fds[i] = open(argv[i + 1], O_RDONLY);
		if (fds[i] < 0)
		{
			while (i > 0)
				close(fds[--i]);
			return (free(fds), free(lines), 1);
		}
		lines[i] = get_next_line(fds[i]);
		i++;
	}
	active = 1;
	while (active)
	{
		active = 0;
		i = 0;
		while (i < argc - 1)
		{
			if (lines[i])
			{
				printf("%s", lines[i]);
				free(lines[i]);
				lines[i] = get_next_line(fds[i]);
			}
			if (lines[i])
				active = 1;
			i++;
		}
	}
	i = 0;
	while (i < argc - 1)
		close(fds[i++]);
	free(fds);
	free(lines);
	return (0);
}
*/
