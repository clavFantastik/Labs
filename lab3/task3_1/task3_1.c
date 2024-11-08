#include <stdio.h>

typedef enum {
    SUCCESS,
    INPUT_ERROR
} status;

int addition(int x, int y) 
{
    int transfer = (x & y) << 1;
    int res = x ^ y;

    if (transfer == 0)
        return res;
    
    return addition(transfer, res);
}

int negation(int x) 
{
    return addition(~x, 1);
}

status decimalToBase2PowX(int num, int r, char *ans, int *size) 
{
    int mask, digit;
    char base[] = "0123456789ABCDEFGHIJKLMNOPQRSTUV", flag = 0;
    *size = 0;

    if (!ans || !size || r < 1 || r > 5 )
        return INPUT_ERROR;
    
    if (num < 0) {
        num = negation(num);
        flag = '1';
    }

    do {
        mask = addition(1 << r, -1);
        digit = num & mask;
        num >>= r;
        ans[*size] = base[digit];
        *size = addition(*size, 1);
    } while (num);

    if (flag) {
        ans[*size] = '-';
        *size = addition(*size, 1);
    }

    return SUCCESS;
}


void printResult(const char *res, int size) 
{
    int i;
    printf("ANSWER:  ");

    for (i = addition(size, -1); i >= 0; i = addition(i, -1))
        putchar(res[i]);

    putchar('\n');
}


int main() 
{
    char ans[128];
    int size = 0, x, r;
    scanf("%d %d", &x, &r);

    if (decimalToBase2PowX(x, r, ans, &size)) {
        printf("Input error\n");
        return INPUT_ERROR;
    }

    printResult(ans, size);

    return 0;
}
