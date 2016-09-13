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
    char line[height][width+1]; // width characters, each either a number or a '.'

    int maxVal ()
    {
        int result = 0;
        for (int i = 0; i < height; i++) 
        for (int j = 0; j < width; j++) 
        {
            if ((line[i][j] >= '0') && (line[i][j] <= '8'))
            {
                int Val = line[i][j] - '0';
                if (Val > result) result = Val;
            }
        }
        return result;
    }

    for (int i = 0; i < height; i++) 
    {
        for (int j = 0; j < width; j++) line[i][j] = fgetc(stdin); // width characters, each either a number or a '.'
        fgetc(stdin);
        line[i][width] = 0;
fprintf(stderr, "%s\n", line[i]);
    }

    void setLinkFrom(int x, int y)
    {
fprintf(stderr, "%d %d ", x, y);
        int max = 0;
        int x1;
        int y1;
        int i;
        for (i = x - 1; i >= 0; i--)
            if ((line[y][i] >= '0') && (line[y][i] <= '8'))
            {
                int Val = line[y][i] - '0';
fprintf(stderr, ">> %d %d : %d", i, y, Val);
                if (Val > max) 
                {
                    max = Val;
                    x1 = i;
                    y1 = y;
                }
                break;
            }
        for (i = x + 1; i < width; i++)
            if ((line[y][i] >= '0') && (line[y][i] <= '8'))
            {
                int Val = line[y][i] - '0';
fprintf(stderr, ">> %d %d : %d", i, y, Val);
                if (Val > max) 
                {
                    max = Val;
                    x1 = i;
                    y1 = y;
                }
                break;
            }
        for (i = y - 1; i >= 0; i--)
            if ((line[i][x] >= '0') && (line[i][x] <= '8'))
            {
                int Val = line[i][x] - '0';
fprintf(stderr, ">> %d %d : %d", x, i, Val);
                if (Val > max) 
                {
                    max = Val;
                    x1 = x;
                    y1 = i;
                }
                break;
            }
        for (i = y + 1; i < height; i++)
            if ((line[i][x] >= '0') && (line[i][x] <= '8'))
            {
                int Val = line[i][x] - '0';
fprintf(stderr, ">> %d %d : %d", x, i, Val);
                if (Val > max) 
                {
                    max = Val;
                    x1 = x;
                    y1 = i;
                }
                break;
            }
fprintf(stderr, "|| %d\n", max);
        if (max > 0)
        {
fprintf(stderr, "%d %d %d %d 1\n", x, y, x1, y1);
         printf("%d %d %d %d 1\n", x, y, x1, y1);
         line[y][x]--;
         line[y1][x1]--;
        }
        else fprintf(stderr, "Алгоритм не работает\n");
    }

    for (int max = maxVal(); max > 0; max = maxVal())
    {
        for (int i = 0; i < height; i++) 
            for (int j = 0; j < width; j++) 
                if (line[i][j] == '0' + max) setLinkFrom(j, i);
    } 

    return 0;
}
