#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void *checkDigits(void *creditCard); /* threads call this function */
void *firstDigit(void *initialDigit);
void *evenAdd(void *evenDigits);
void *oddAdd(void *oddDigits);

/* argc number of items entered in the command line */
/* argv[] is an array that holds string values that were entered on the command line */

bool card = true;
int evenTotal = 0;
int oddTotal = 0;
char initial = 'x';

int main(int argc, char* argv[])
{
    int i, sum = 0;

    /* %d means we will be supplying an integer */
    /* what comes after the quotes is where we are getting the integer from */
    printf("argc = %d\n", argc);

    if(argc > 1) /* we were given paramaters on the command line */
    {
        /* Can't declare a variable inside a for loop in C */
        for(i=1; i< argc; i++)
        {
            /* %s means we will be placing a string value */
            printf("argv[%d] = %s\n", i, argv[i]);
            card = true;//Must reset for each given card
            evenTotal = 0;
            oddTotal = 0;
            initial = 'x';
            pthread_t tid; /* the thread identifier */
            pthread_t tid1;
            pthread_t tid2;
            pthread_t tid3;
            pthread_attr_t attr; /* set of thread attributes */
            /* set the default attributes of the thread */
            pthread_attr_init(&attr);
            pthread_create(&tid, &attr, checkDigits, argv[i]);
            pthread_create(&tid1, &attr,firstDigit, argv[i]);
            pthread_create(&tid2, &attr,evenAdd, argv[i]);
            pthread_create(&tid3, &attr,oddAdd, argv[i]);
            pthread_join(tid,NULL);
            pthread_join(tid1,NULL);
            pthread_join(tid2,NULL);
            pthread_join(tid3,NULL);
            //printf("Odd total is %d\n",oddTotal);
            //printf("Even total is %d\n",evenTotal);
            sum = evenTotal + oddTotal;
            //printf("Even total plus odd total is %d\n",sum);
            if(sum%10!=0)
            {
                printf("Even total plus odd total is not divisible by 10\n");
                card = false;
            }
            if(card==false)
            {
                printf("Credit Card is false\n");
            }
            else
            {
                printf("Credit Card %s is true", argv[i]);
                switch(initial)
                {
                    case 'V':
                        printf("Card is a Visa\n");
                        break;
                    case 'D':
                        printf("Card is a Discover Card\n");
                        break;
                    case 'M':
                        printf("Card is a Master Card\n");
                        break;
                    case 'A':
                        printf("Card is a American Express");
                        break;
                        //printf("Credit Card is a %s\n",cardType);
                }
            }
        }
    }
    return 0;
}

/* The thread will execute in this function */
void *checkDigits(void *creditCard)
{
    char *ptr = (char*)creditCard;
    int count = 0;
    do
    {
        //printf("The current char is %c\n",*ptr);
        count = count + 1;
        *ptr = *++ptr;
        //printf("Count is currently %d\n", count);
    }while(*ptr!='\0');

    //printf("Count is currently %d\n", count);
    if(count<13||count>19)
    {
        printf("Card has invalid number of digits\n");
        card = false;
    }
    pthread_exit(0);
}

void *firstDigit(void *initialDigit)
{
    char *ptr = (char*)initialDigit;

    if(*ptr=='4' || *ptr=='3' || *ptr=='6' || *ptr=='5')
    {
        if(*ptr=='4')//Card is a Visa
        {
            initial = 'V';
        }
        if(*ptr=='6')//Card is a Discover
        {
            initial = 'D';
        }
        if(*ptr=='3')//Second digit must be 4 or 7
        {
            *ptr = *++ptr;
            if(*ptr!='4'&&*ptr!='7')
            {
                printf("Card starts with an invalid digits\n");
                card = false;
            }
            else//Card is an American Express
            {
                initial = 'A';
            }
        }
        if(*ptr=='5')//Second digit must be 1-5
        {
            *ptr = *++ptr;
            if(*ptr!='1'&&*ptr!='2'&&*ptr!='3'&&*ptr!='4'&&*ptr!='5')
            {
                printf("Card starts with an invalid digits\n");
                card = false;
            }
            else//Card is a Mastercard
            {
                initial = 'M';
            }
        }
    }
    else
    {
        printf("Card starts with an invalid digit\n");
    }
    pthread_exit(0);
}
void *evenAdd(void *evenDigits)
{
    char *ptr = (char*)evenDigits;
    int size = 0;
    int value = 0;
    do
    {
        //printf("Current char is %c\n", *ptr);
        if(*ptr!='\0')
        {
            *ptr = *++ptr;
            size = size + 1;
            // printf("Total size is %d\n",size);
        }
    }while(*ptr!='\0');
    *ptr = *--ptr;//pointer is currently at null character
    if(size>12 && size<20)
    {
        *ptr = *--ptr;
        while(size>0)
        {
            //printf("Current char for even is %c\n", *ptr);
            value = *ptr - 48;
            //printf("Current value for even is %d\n",value);
            value = value * 2;
            //printf("Value after doubling is %d\n",value);
            if(value > 9)
            {
                value = value%10 + 1;
                //printf("Value after adding both digits is %d\n",value);
            }
            //printf("Current value is %d\n",value);
            evenTotal = evenTotal + value;
            //printf("Even total so far is %d\n",evenTotal);
            size = size - 2;
            *ptr = *--ptr;
            *ptr = *--ptr;
        }
    }
    pthread_exit(0);
}
void *oddAdd(void *oddDigits)
{
    char *ptr = (char*)oddDigits;
    int size = 0;
    int value = 0;
    do
    {
        //printf("Current char is %c\n", *ptr);
        if(*ptr!='\0')
        {
            *ptr = *++ptr;
            size = size + 1;
            //printf("Total size is %d\n",size);
        }
    }while(*ptr!='\0');
    *ptr = *--ptr;//pointer is currently at null character
    if(size>12 && size<20)
    {
        if(size%2!=0)
        {
            *ptr = *--ptr;
        }
        while(size>0)
        {
            //printf("Current char is %c\n", *ptr);
            value = *ptr - 48;
            //printf("Current value is %d\n",value);
            oddTotal = oddTotal + value;
            //printf("Odd total so far is %d\n",oddTotal);
            size = size - 2;
            *ptr = *--ptr;
            *ptr = *--ptr;
        }
    }
    pthread_exit(0);
}
