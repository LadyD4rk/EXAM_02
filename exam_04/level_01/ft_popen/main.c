#define _GNU_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int ft_popen(const char *file, char *const argv[], char type);

int main()
{
	int	fd = ft_popen("ls", (char *const []){"ls", NULL}, 'r');
    FILE* file = fdopen(fd, "r");

    char line[256];

	printf("=============================OUTPUT TEST=============================\n");
    if (file != NULL) {
        while (fgets(line, sizeof(line), file)) {
            printf("%s", line);
        }
        fclose(file);
    }
    else {
        fprintf(stderr, "Unable to open file!\n");
    }
	printf("=============================INPUT TEST==============================\n");
	fd = ft_popen("grep", (char *const []){"grep", "c", NULL}, 'w');
    if (fd < 0)
    {
        fprintf(stderr, "ft_popen failed\n");
        return 1;
    }

    while (fgets(line, sizeof(line), stdin))
        write(fd, line, strlen(line));

    close(fd);

    return 0;
}