#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main(void) {
    time_t start;
    time_t end;
    int count = 0;

    while (1)
    {
        count = (count + 1) * 2;
        int a = count * 2;
        int b = a * count;
        for (int i = 0; i < count; i++)
        {
            a++;
            b++;
        }
        
    }

    return 0;
}