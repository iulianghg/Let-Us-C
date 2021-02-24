// -------------------------------------------------------------------------------
// Name:        Graph Analysis
//
// Author:      Julian Gherghel
// -------------------------------------------------------------------------------
//
// Pass string as an argument in command line

#include <stdio.h>
#include <string.h>


int main(int argc, char* argv[])
{
    //check for passing the arguement
    if(argc < 2)
    {
        printf("Try again by passing arguement\n");
        return 1;
    }
    
    //check for passing a string argument
    for(int j=1;j<= argc-1;j++)
    {
        for(int k=0;argv[j][k]!='\0';k++)
        {
            if ((argv[j][k]) == 0)
            {
                printf("Try again by passing a string\n");
                return 2;
            }
        }
    }
    
    //Logic for reversing the string
    for (int i = argc - 1; i > 0; i--)
    {
        for (int j = strlen(argv[i]) - 1; j >= 0; j--)
            printf("%c", argv[i][j]);
    }
    printf("\n");
        
return 0;
}