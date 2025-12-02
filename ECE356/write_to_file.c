#include <stdio.h>
#include <string.h>

int main(void){
    FILE *fp = fopen("output_file.txt", "w");
    if (fp == NULL){
        return 1;
    }
    char buffer[] = "Hello, World!";
    if (fwrite(buffer, sizeof(char), strlen(buffer), fp)){
        printf("data written successfully to file\n");
    } else {
        printf("Something went wrong somewhere :(\n");
    }

    fclose(fp);
    return 0;
}