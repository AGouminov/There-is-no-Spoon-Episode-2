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
    int horLink[height][width][width];
    int verLink[width][height][height];

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
        for (int j = 0; j < width; j++) 
        {
            line[i][j] = fgetc(stdin); // width characters, each either a number or a '.'
            for (int k = 0; k < height; k++) verLink[j][i][k] = 0;
            for (int k = 0; k < width;  k++) horLink[i][j][k] = 0;
        }
        fgetc(stdin);
        line[i][width] = 0;
fprintf(stderr, "%s\n", line[i]);
    }
    
    int canLink(int x, int y, int x1, int y1)
    {
        if ((line[y][x] > '0') && (line[y][x] <= '8') && 
            (line[y1][x1] > '0') && (line[y1][x1] <= '8'))
            if (y == y1)      
            {
fprintf(stderr, "[%d] ", horLink[y][x][x1] + horLink[y][x1][x]);
                return ((horLink[y][x][x1] + horLink[y][x1][x]) < 2);
            }
            else if (x == x1) 
            {
fprintf(stderr, "[%d] ", verLink[x][y][y1] + verLink[x][y1][y]);
                return ((verLink[x][y][y1] + verLink[x][y1][y]) < 2);
            }
        return 0;
    }

    void makeLink(int x, int y, int x1, int y1)
    {
fprintf(stderr, "%d %d %d %d 1\n", x, y, x1, y1);
        printf("%d %d %d %d 1\n", x, y, x1, y1);
        line[y][x]--;
        line[y1][x1]--;
        if (y == y1) horLink[y][x][x1]++;
        if (x == x1) verLink[x][y][y1]++;
    }

    void setLinkFrom(int x, int y)
    {
fprintf(stderr, "%d %d ", x, y);
        int max = 0;
        int x1;
        int y1;
        int i;
        for (i = x - 1; i >= 0; i--)
            if (canLink(x, y, i, y))
            {
                int Val = line[y][i] - '0';
fprintf(stderr, " %d %d : %d", i, y, Val);
                if (Val > max) 
                {
                    max = Val;
                    x1 = i;
                    y1 = y;
                }
                break;
            }
        for (i = x + 1; i < width; i++)
            if (canLink(x, y, i, y))
            {
                int Val = line[y][i] - '0';
fprintf(stderr, " %d %d : %d", i, y, Val);
                if (Val > max) 
                {
                    max = Val;
                    x1 = i;
                    y1 = y;
                }
                break;
            }
        for (i = y - 1; i >= 0; i--)
            if (canLink(x, y, x, i))
            {
                int Val = line[i][x] - '0';
fprintf(stderr, " %d %d : %d", x, i, Val);
                if (Val > max) 
                {
                    max = Val;
                    x1 = x;
                    y1 = i;
                }
                break;
            }
        for (i = y + 1; i < height; i++)
            if (canLink(x, y, x, i))
            {
                int Val = line[i][x] - '0';
fprintf(stderr, " %d %d : %d", x, i, Val);
                if (Val > max) 
                {
                    max = Val;
                    x1 = x;
                    y1 = i;
                }
                break;
            }
fprintf(stderr, "|| %d\n", max);
        if (max > 0) makeLink(x, y, x1, y1);
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
