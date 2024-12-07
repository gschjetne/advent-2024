#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int match(char *start, char *cursor, char *end, size_t right_edge, int dx,
          int dy) {
    const char *word = "MAS";

    for (int i = 0; i < 3; i++) {
        cursor += dx + dy * right_edge;

        if (cursor < start || cursor >= end) {
            return 0;
        }

        if (*cursor != word[i]) {
            return 0;
        }
    }

    return 1;
}

int main(int argc, char *argv[]) {
    assert(argc >= 2);

    int result = 0;

    int fd = open(argv[1], O_RDONLY);
    struct stat sb;

    if (fstat(fd, &sb) == -1) {
        perror("fstat");
    }

    char *start = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    char *end = start + sb.st_size;

    char *dp = start;
    size_t line_length = 0;
    while (dp[line_length] != '\n') {
        line_length++;
    }
    line_length++;

    char *cursor = start;
    while (cursor < start + sb.st_size) {
        if (*cursor == 'X') {
            result += match(start, cursor, end, line_length, -1, -1);
            result += match(start, cursor, end, line_length, -1, 0);
            result += match(start, cursor, end, line_length, -1, 1);
            result += match(start, cursor, end, line_length, 0, -1);
            result += match(start, cursor, end, line_length, 0, 1);
            result += match(start, cursor, end, line_length, 1, -1);
            result += match(start, cursor, end, line_length, 1, 0);
            result += match(start, cursor, end, line_length, 1, 1);
        }

        cursor++;
    }

    printf("result = %d\n", result);

    munmap(start, sb.st_size);
    close(fd);
    return 0;
}
