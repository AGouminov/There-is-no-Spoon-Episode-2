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
    
    int findNode(int x, int y, int incX, int incY, int * x1, int * y1)
    {
        *x1 = x + incX;
        *y1 = y + incY;
        while ((*x1 >= 0) && (*x1 < width) && (*y1 >= 0) && (*y1 < height))
        {
         if ((line[*y1][*x1] >= '0') && (line[*y1][*x1] <= '8')) return 1;
            *x1 += incX;
            *y1 += incY;
        }
        return 0;
    }
    
    int canLink(int x, int y, int x1, int y1)
    {
        if (y == y1)      
        {
            return ((horLink[y][x][x1] + horLink[y][x1][x]) < 2);
        }
        else if (x == x1) 
        {
            return ((verLink[x][y][y1] + verLink[x][y1][y]) < 2);
        }
        return 0;
    }

    void makeLink(int x, int y, int x1, int y1, int how)
    {
fprintf(stderr, "%d %d %d %d %d\n", x, y, x1, y1, how);
        printf("%d %d %d %d %d\n", x, y, x1, y1, how);
        line[y][x] -= how;
        line[y1][x1] -= how;
        if (y == y1) horLink[y][x][x1] += how;
        if (x == x1) verLink[x][y][y1] += how;
    }
    
    void setLinkFrom(int x, int y, int how)
    {
fprintf(stderr, "%d %d>>", x, y);
        int max = 0;
        int maxX;
        int maxY;
        int x1;
        int y1;
        if (findNode(x, y, 1, 0, &x1, &y1) && canLink(x, y, x1, y1))
        {
            int Val = line[y1][x1] - '0';
fprintf(stderr, " %d %d : %d", x1, y1, Val);
            if (Val > max) 
            {
                max = Val;
                maxX = x1;
                maxY = y1;
            }
        }
        if (findNode(x, y, -1, 0, &x1, &y1) && canLink(x, y, x1, y1))
        {
            int Val = line[y1][x1] - '0';
fprintf(stderr, " %d %d : %d", x1, y1, Val);
            if (Val > max) 
            {
                max = Val;
                maxX = x1;
                maxY = y1;
            }
        }
        if (findNode(x, y, 0, 1, &x1, &y1) && canLink(x, y, x1, y1))
        {
            int Val = line[y1][x1] - '0';
fprintf(stderr, " %d %d : %d", x1, y1, Val);
            if (Val > max) 
            {
                max = Val;
                maxX = x1;
                maxY = y1;
            }
        }
        if (findNode(x, y, 0, -1, &x1, &y1) && canLink(x, y, x1, y1))
        {
            int Val = line[y1][x1] - '0';
fprintf(stderr, " %d %d : %d", x1, y1, Val);
            if (Val > max) 
            {
                max = Val;
                maxX = x1;
                maxY = y1;
            }
        }

fprintf(stderr, "|| %d\n", max);
        if (max > 0) makeLink(x, y, maxX, maxY, how);
        else fprintf(stderr, "Алгоритм не работает\n");
    }

    
    while (maxVal())
    {
        int count = 0;
        for (int y = 0; y < height; y++) 
            for (int x = 0; x < width; x++) 
                if ((line[y][x] > '0') && (line[y][x] <= '8'))
                {
                    int x1;
                    int y1;
                    if ((findNode(x, y,  0,  1, &x1, &y1) +
                         findNode(x, y,  0, -1, &x1, &y1) +
                         findNode(x, y,  1,  0, &x1, &y1) +
                         findNode(x, y, -1,  0, &x1, &y1)) == 1)
                    {
fprintf(stderr, "%d %d has one choice \n", x, y);
                        setLinkFrom(x, y, line[y][x] - '0');
                        count++;
                    }
                }

        int max = maxVal();
        if (max > 0)
        {
            int x1 = -1;
            int y1 = -1;
            for (int y = 0; (x1 < 0) && (y < height); y++) 
                for (int x = 0; x < width; x++) 
                    if (line[y][x] == '0' + max) 
                    {
                        x1 = x;
                        y1 = y;
                        break;
                    }
            setLinkFrom(x1, y1, 1);
        }    
                    
    } 

    return 0;
}
