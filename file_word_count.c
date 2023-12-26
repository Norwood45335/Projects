# Rahmin Norwood
#the program counts number of words in file
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define max 30
int main (int argc, char *argv[]){
    unsigned long counter=0;
    char word[max]; 
    FILE *fp;
    fp=fopen(argv[1],"r");
    while(fscanf(fp,"%s",word) != EOF){
    
	counter++;
    }
    fclose(fp);    
    printf("There are %d words in the file.\n",counter);
    return 0;
}
