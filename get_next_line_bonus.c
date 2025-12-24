/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ancheab <ancheab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 17:54:33 by ancheab           #+#    #+#             */
/*   Updated: 2025/12/19 16:45:00 by ancheab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*get_line(int fd, char *line)
{
	char	*buffer;
	ssize_t	bytes_number;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE) + 1);
	if (!buffer)
		return (NULL);
	bytes_number = 1;
	while (!ft_strchr(line, '\n') && bytes_number > 0)
	{
		bytes_number = read(fd, buffer, BUFFER_SIZE);
		if (bytes_number == -1)
		{
			free(buffer);
			return (NULL);
		}
		buffer[bytes_number] = '\0';
		line = ft_strjoin(line, buffer);
	}
	free(buffer);
	return (line);
}

char	*the_next_line(char *line)
{
	int		i;
	char	*str;

	i = 0;
	if (!line[i])
		return (NULL);
	while (line[i] && line[i] != '\n')
		i++;
	str = (char *)malloc(i + 2);
	if (!str)
		return (NULL);
	i = 0;
	while (line[i] && line[i] != '\n')
	{
		str[i] = line[i];
		i++;
	}
	if (line[i] == '\n')
	{
		str[i] = line[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	*get_new_line(char *line)
{
	int		i;
	int		j;
	char	*str;

	i = 0;
	while (line[i] && line[i] != '\n')
		i++;
	if (!line[i])
	{
		free(line);
		return (NULL);
	}
	str = (char *)malloc(sizeof(char) * (ft_strlen(line) - i + 1));
	if (!str)
	{
		free(line);
		return (NULL);
	}
	i++;
	j = 0;
	while (line[i])
		str[j++] = line[i++];
	str[j] = '\0';
	free(line);
	return (str);
}

char	*get_next_line(int fd)
{
	static char	*line[OPEN_MAX];
	char		*next_line;

	if (fd < 0 || fd >= OPEN_MAX || BUFFER_SIZE <= 0)
		return (NULL);
	line[fd] = get_line(fd, line[fd]);
	if (!line[fd])
		return (NULL);
	next_line = the_next_line(line[fd]);
	line[fd] = get_new_line(line[fd]);
	if (line[fd] && !*line[fd])
	{
		free(line[fd]);
		line[fd] = NULL;
	}
	return (next_line);
}
