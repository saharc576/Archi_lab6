#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
    char input[100];
    fgets(input, 100, stdin);
    printf("%s", input);
}