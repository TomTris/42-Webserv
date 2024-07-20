
#include "unistd.h"

void print(long long t)
{
    if (t)
    {
        char temp = t % 10 + '0';
        print(t / 10);
        write(1, &temp, 1);
    }
}

void ft_putnumb(int i)
{
    long long t = i;
    if (t < 0)
    {
        write(1, "-", 1);
        t = -t;
    }
    else if (t == 0)
        write(1, "0", 1);
    print(t);
    write(1, "\n", 1);
}


int main()
{
    ft_putnumb(231);
    ft_putnumb(0);
    ft_putnumb(2147483647);
    ft_putnumb(-2147483648);
    ft_putnumb(42);
}