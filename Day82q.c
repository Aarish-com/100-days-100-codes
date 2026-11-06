/ Enum with SUCCESS, FAILURE, TIMEOUT. Input: 0, 1 or 2.
#include <stdio.h>
enum Status { SUCCESS, FAILURE, TIMEOUT };
int main(void) {
    int v;
    if (scanf("%d", &v) != 1) return 1;
    switch ((enum Status)v) {
        case SUCCESS: printf("Operation completed successfully\n"); break;
        case FAILURE: printf("Operation failed\n"); break;
        case TIMEOUT: printf("Operation timed out\n"); break;
        default:      printf("Unknown status\n");
    }
    return 0;
}
