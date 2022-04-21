#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //Asks for user's name
    string username = get_string("What's your name? ");
    //Salutes user
    printf("hello, %s ", username);
}
