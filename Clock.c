//digital clock
#include <stdio.h>

int main ()
{
    int h,m,s;
    int d=1000; //add delay of 100 milliseconds
    printf("Set time:");
    scanf("%d%d%d",&h,&m,&s);
    if(h>12||m>60||s>60)
    {
        printf("ERROR! \n");
        //exit(0); //program successfully terminates without errors
    }
    while(1) //infinte loop, always evaluates to true
    {
    //contains all necessary conditionals
        s++;
        if(s>59)
        {
            m++;
            s=0;
        }
        if(m>59)
        {
            h++;
            m=0;
        }
        if(h>12)
        {
            h=1;
        }
        //printing the time by hour, minute, second
        printf("Clock: \n");
        printf(" %02d:%02d:%02d",h,m,s);
        sleep(d); //said to slow down the whilte loop
    }
}