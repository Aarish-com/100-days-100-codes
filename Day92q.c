// Enum for traffic lights. Input: 0 (RED), 1 (YELLOW) or 2 (GREEN).
#include <stdio.h>
enum Light { RED, YELLOW, GREEN };
int main(void) {
    int v;
    if (scanf("%d", &v) != 1) return 1;
    switch ((enum Light)v) {
        case RED:    printf("Stop\n"); break;
        case YELLOW: printf("Wait\n"); break;
        case GREEN:  printf("Go\n");   break;
        default:     printf("Invalid light\n");
    }
    return 0;
}
