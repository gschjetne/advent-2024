#include <stdio.h>

int main(void) {
    int f1, f2, chr = 0, result = 0;

start:
    if (chr == EOF)
        goto end;

    f1 = f2 = 0;

    if ((chr = getchar()) != 'm')
        goto start;
    if ((chr = getchar() != 'u'))
        goto start;
    if ((chr = getchar() != 'l'))
        goto start;
    if ((chr = getchar() != '('))
        goto start;

f1_digits:
    chr = getchar();
    if (chr >= '0' && chr <= '9') {
        f1 = (f1 * 10) + (chr - '0');
        goto f1_digits;
    } else if (chr == ',') {
        goto f2_digits;
    } else {
        goto start;
    }

f2_digits:
    chr = getchar();
    if (chr >= '0' && chr <= '9') {
        f2 = (f2 * 10) + (chr - '0');
        goto f2_digits;
    } else if (chr == ')') {
        goto multiply;
    } else {
        goto start;
    }

multiply:
    result += f1 * f2;
    goto start;

end:
    printf("result = %d\n", result);

    return 0;
}
