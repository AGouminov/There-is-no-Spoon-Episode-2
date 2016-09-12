#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * The machines are gaining ground. Time to show them what we're really made of...
 **/
int main()
{
    int width; // the number of cells on the X axis
    scanf("%d", &width);
    int height; // the number of cells on the Y axis
    scanf("%d", &height); fgetc(stdin);
    for (int i = 0; i < height; i++) {
        char line[32]; // width characters, each either a number or a '.'
        fgets(line, 32, stdin); // width characters, each either a number or a '.'
    }

    // Write an action using printf(). DON'T FORGET THE TRAILING \n
    // To debug: fprintf(stderr, "Debug messages...\n");


    // Two coordinates and one integer: a node, one of its neighbors, the number of links connecting them.
    printf("0 0 2 0 1\n");

    return 0;
}
