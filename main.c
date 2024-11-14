/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meferraz <meferraz@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 15:56:25 by meferraz          #+#    #+#             */
/*   Updated: 2024/11/08 16:30:00 by meferraz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../get_next_line.h"
#include "ansi.h"
#include <string.h>

char *is_file_in_folder(const char *filename, const char *folder_path,
                        char *full_path, size_t path_size);

static int total_lines = 0;
static int total_chars = 0;

static void print_header(const char *title)
{
    printf("%s=== %s ===%s\n", BHBLU, title, RESET);
}

int main(int argc, char **argv)
{
    const char *folder_path = "files";
    char full_path[1024];
    char *line;
    int fd;

    if (argc != 2)
    {
        printf("%sPlease execute the program again with a valid filename.%s\n", HRED, RESET);
        return (1);
    }

    if (!is_file_in_folder(argv[1], folder_path, full_path, sizeof(full_path)))
    {
        printf("%s'%s' is not a valid file in the 'files' folder.%s\n", HRED, argv[1], RESET);
        return (1);
    }

    print_header("File Information");
    printf("%sFile: %s%s\n", HCYN, full_path, RESET);
    printf("%sBUFFER_SIZE: %d%s\n\n", HCYN, BUFFER_SIZE, RESET);

    fd = open(full_path, O_RDONLY);
    if (fd == -1)
    {
        printf("%sError opening file.%s\n", HRED, RESET);
        return (1);
    }

    print_header("File Contents");
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s%s%s", HGRN, line, RESET);
        total_lines++;
        total_chars += strlen(line) - 1;
        free(line);
    }

    close(fd);

    print_header("Summary");
    printf("%sTotal lines read: %d%s\n", HYEL, total_lines, RESET);
    printf("%sTotal characters read: %d%s\n", HYEL, total_chars, RESET);

    return (0);
}

char	*is_file_in_folder(const char *filename, const char *folder_path,
		char *full_path, size_t path_size)
{
	size_t	folder_len;
	size_t	file_len;

	folder_len = strlen(folder_path);
	file_len = strlen(filename);
	if (folder_len + file_len + 2 > path_size)
		return (NULL);
	strcpy(full_path, folder_path);
	if (folder_path[folder_len - 1] != '/')
		strcat(full_path, "/");
	strcat(full_path, filename);
	return (full_path);
}
