/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meferraz <meferraz@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 15:56:25 by meferraz          #+#    #+#             */
/*   Updated: 2024/11/08 16:30:00 by meferraz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs_bonus/get_next_line_bonus.h"
#include "ansi.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILENAME 256

int	is_file_in_folder(char *folder, char *file)
{
	char	file_path[MAX_FILENAME];
	int		fd;
	size_t	folder_len;
	size_t	file_len;

	folder_len = strlen(folder);
	file_len = strlen(file);
	if (folder_len + file_len + 2 > MAX_FILENAME)
		return (0);
	strcpy(file_path, folder);
	file_path[folder_len] = '/';
	strcpy(file_path + folder_len + 1, file);
	fd = open(file_path, O_RDONLY);
	if (fd == -1)
		return (0);
	close(fd);
	return (1);
}

void	print_header(char *file)
{
	printf(BBLU);
	printf("\n==================== %s ====================\n", file);
	printf(RESET);
}

void	test_files(char **files, int file_count)
{
	int		*fds;
	char	*line;
	int		i;
	int		active_files;

	fds = malloc(sizeof(int) * file_count);
	if (!fds)
	{
		printf(RED "Memory allocation failed\n" RESET);
		return;
	}
	active_files = file_count;

	for (i = 0; i < file_count; i++)
	{
		fds[i] = open(files[i], O_RDONLY);
		if (fds[i] == -1)
		{
			printf(RED "Error opening file: %s\n" RESET, files[i]);
			active_files--;
			fds[i] = -1; // Mark as closed if there was an error
		}
		else
		{
			print_header(files[i]);
		}
	}

	while (active_files > 0)
	{
		for (i = 0; i < file_count; i++)
		{
			if (fds[i] != -1) // Only process open fds
			{
				line = get_next_line(fds[i]);
				if (line)
				{
					printf(YEL "File %d: " RESET, i + 1);
					printf("%s", line);
					free(line); // Free the line after printing
				}
				else // line is NULL
				{
					close(fds[i]); // Close the fd
					fds[i] = -1; // Mark this fd as closed
					active_files--; // Decrease active files count

					printf(GRN "\nEnd of file reached for %s\n" RESET, files[i]);
				}
			}
		}
	}
	free(fds); // Free the array of file descriptors
}

int	main(int argc, char **argv)
{
	char **files;
	int	file_count;
	int	i;
	size_t	len;

	if (argc < 2)
    {
        printf(RED "Usage: %s <file1> <file2> ...\n" RESET, argv[0]);
        return (1);
    }
	
	printf(BMAG "\n====== Testing get_next_line_bonus ======\n" RESET);
	printf(CYN "Maximum number of files that can be opened: %d\n" RESET, FOPEN_MAX);

	files = malloc(sizeof(char *) * FOPEN_MAX);
	if (!files)
    {
        printf(RED "Memory allocation failed\n" RESET);
        return (1);
    }

	file_count = 0;
	for (i = 1; i < argc && file_count < FOPEN_MAX; i++)
    {
        if (is_file_in_folder("files", argv[i]))
        {
            len = strlen("files/") + strlen(argv[i]);
            files[file_count] = malloc(len + 1);
            if (!files[file_count])
            {
                printf(RED "Memory allocation failed\n" RESET);
                exit(1);
            }
            strcpy(files[file_count], "files/");
            strcat(files[file_count], argv[i]);
            file_count++;
        }
    }

	printf(CYN "Number of files found: %d\n" RESET, file_count);

	test_files(files, file_count);

	for (i = 0; i < file_count; i++)
    {
        free(files[i]);
    }
	free(files);

	printf(BMAG "\n====== End of tests ======\n" RESET);

	return (0);
}
