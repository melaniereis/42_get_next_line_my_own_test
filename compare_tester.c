/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compare_tester.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meferraz <meferraz@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 11:01:01 by meferraz          #+#    #+#             */
/*   Updated: 2024/11/14 11:01:36 by meferraz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/get_next_line.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

static int compare_files(int fd1, int fd2)
{
    static char buf1[BUFFER_SIZE], buf2[BUFFER_SIZE];
    ssize_t n1, n2;

    while (1) {
        n1 = read(fd1, buf1, BUFFER_SIZE);
        n2 = read(fd2, buf2, BUFFER_SIZE);

        if (n1 != n2)
            return 0;
        if (n1 == 0)
            return 1;
        
        for (ssize_t i = 0; i < n1; i++) {
            if (buf1[i] != buf2[i])
                return 0;
        }
    }
}

static int str_len(const char *str)
{
    int len = 0;
    while (str[len])
        len++;
    return len;
}

static int create_and_write_temp_file(const char *original_file)
{
    int fd = open(original_file, O_RDONLY);
    if (fd == -1)
        return -1;

    int temp_fd = open("/tmp/gnl_test", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (temp_fd == -1) {
        close(fd);
        return -1;
    }

    char *line;
    while ((line = get_next_line(fd)) != NULL) {
        write(temp_fd, line, str_len(line));
        free(line);
    }

    close(fd);
    close(temp_fd);
    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 2)
        return 1;

    if (create_and_write_temp_file(argv[1]) == -1)
        return 1;

    int fd = open(argv[1], O_RDONLY);
    int temp_fd = open("/tmp/gnl_test", O_RDONLY);
    
    if (fd == -1 || temp_fd == -1) {
        if (fd != -1) close(fd);
        if (temp_fd != -1) close(temp_fd);
        return 1;
    }

    int result = compare_files(fd, temp_fd);

    close(fd);
    close(temp_fd);

    write(1, result ? "OK\n" : "KO\n", 3);

    return 0;
}
