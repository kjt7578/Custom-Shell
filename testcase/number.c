#include <stdio.h>
int main() {
    int n;
    int success = fscanf(stdin, "%d", &n);
    if (success != 1) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    fprintf(stdout, "Your number is %d\n", n);
    return 0;
}