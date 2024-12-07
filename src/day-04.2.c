#include <assert.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int match(char *start, char *cursor, char *end, size_t right_edge) {
    if (*cursor != 'A') {
        return 0;
    }

    char *nw = cursor - right_edge - 1;
    char *ne = cursor - right_edge + 1;
    char *sw = cursor + right_edge - 1;
    char *se = cursor + right_edge + 1;

    if (nw <= start || nw > end) {
        return 0;
    }

    if (ne <= start || ne > end) {
        return 0;
    }

    if (sw <= start || sw > end) {
        return 0;
    }

    if (se <= start || se > end) {
        return 0;
    }

    bool sw_ne = (*sw == 'M' && (*ne) == 'S') || (*sw == 'S' && (*ne) == 'M');
    bool nw_se = (*nw == 'M' && (*se) == 'S') || (*nw == 'S' && (*se) == 'M');

    return (sw_ne && nw_se);
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
    size_t right_edge = 0;
    while (dp[right_edge] != '\n') {
        right_edge++;
    }
    right_edge++;

    char *cursor = start;
    while (cursor < start + sb.st_size) {
        result += match(start, cursor, end, right_edge);

        cursor++;
    }

    printf("result = %d\n", result);

    munmap(start, sb.st_size);
    close(fd);
    return 0;
}
