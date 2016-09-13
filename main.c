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
        int result = 0;
        if (line[y1][x1] == '0')
            result = 0;
        else if (y == y1)      
        {
            if ((horLink[y][x][x1] + horLink[y][x1][x]) < 2) 
            {
                int count = 0;
                int xmin = x > x1 ? x1 : x;
                int xmax = x > x1 ? x : x1;
                for (int xi = xmin + 1; xi < xmax; xi++)
                    for (int i = 0; i < y; i++)
                        for (int j = y + 1; j < height; j++)
                        {
                            count += verLink[xi][i][j];
                            count += verLink[xi][j][i];
                        }
                result = (count == 0);
            }
        }
        else if (x == x1) 
        {
            if ((verLink[x][y][y1] + verLink[x][y1][y]) < 2)
            {
                int count = 0;
                int ymin = y > y1 ? y1 : y;
                int ymax = y > y1 ? y : y1;
                for (int yi = ymin + 1; yi < ymax; yi++)
                    for (int i = 0; i < x; i++)
                        for (int j = x + 1; j < width; j++)
                        {
                            count += horLink[yi][i][j];
                            count += horLink[yi][j][i];
                        }
                result = (count == 0);
            }
        }
        return result;
    }

    void makeLink(int x, int y, int x1, int y1, int how)
    {
        printf("%d %d %d %d %d\n", x, y, x1, y1, how);
        line[y][x] -= how;
        line[y1][x1] -= how;
//if (y == y1) fprintf(stderr, "links: %d + %d\n", horLink[y][x][x1], horLink[y][x1][x]);
//if (x == x1) fprintf(stderr, "links: %d + %d\n", verLink[x][y][y1], verLink[x][y1][y]);
        if (y == y1) horLink[y][x][x1] += how;
        if (x == x1) verLink[x][y][y1] += how;
//fprintf(stderr, "%d %d %d %d %d\n=============\n", x, y, x1, y1, how);
//for (int i = 0; i < height; i++) fprintf(stderr, "%s\n", line[i]);
//fprintf(stderr, "=============\n");
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
fprintf(stderr, " %d %d : %d |", x1, y1, Val);
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
fprintf(stderr, " %d %d : %d |", x1, y1, Val);
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
fprintf(stderr, " %d %d : %d |", x1, y1, Val);
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
fprintf(stderr, " %d %d : %d |", x1, y1, Val);
            if (Val > max) 
            {
                max = Val;
                maxX = x1;
                maxY = y1;
            }
        }

fprintf(stderr, "| %d\n", max);
        if (max > 0) makeLink(x, y, maxX, maxY, how);
        else fprintf(stderr, "Алгоритм не работает\n");
    }

    
    while (maxVal())
    {
        int count;
        do
        {
            count = 0;
            for (int y = 0; y < height; y++) 
                for (int x = 0; x < width; x++) 
                {
                    int needs = line[y][x] - '0';
                    if ((needs > 0) && (needs <= 8))
                    {
                        int x1;
                        int y1;
                        int nodes = 0;
                        int maxLink[4] = {0, 0, 0, 0};
                        int getMaxLink(int x, int y, int x1, int y1)
                        {
                            int max1 = 2;
                            if (y == y1) max1 -= (horLink[y][x][x1] + horLink[y][x1][x]);
                            else         max1 -= (verLink[x][y][y1] + verLink[x][y1][y]);
                            int max2 = (line[y1][x1] > '1') ? 2 : 1;
                            return (max1 < max2) ? max1 : max2;
                        }
                        
                        if (findNode(x, y,  0,  1, &x1, &y1) && canLink(x, y, x1, y1))
                        {
                            nodes++;
                            maxLink[0] = getMaxLink(x, y, x1, y1);
                        }
                        if (findNode(x, y,  0, -1, &x1, &y1) && canLink(x, y, x1, y1))
                        {
                            nodes++;
                            maxLink[1] = getMaxLink(x, y, x1, y1);
                        }
                        if (findNode(x, y,  1,  0, &x1, &y1) && canLink(x, y, x1, y1))
                        {
                            nodes++;
                            maxLink[2] = getMaxLink(x, y, x1, y1);
                        }
                        if (findNode(x, y, -1,  0, &x1, &y1) && canLink(x, y, x1, y1))
                        {
                            nodes++;
                            maxLink[3] = getMaxLink(x, y, x1, y1);
                        }

                        if (nodes == 1)
                        {
fprintf(stderr, "+++У %d %d всего один сосед \n", x, y);
                            if (maxLink[0] && findNode(x, y,  0,  1, &x1, &y1)) makeLink(x, y, x1, y1, needs);
                            if (maxLink[1] && findNode(x, y,  0, -1, &x1, &y1)) makeLink(x, y, x1, y1, needs);
                            if (maxLink[2] && findNode(x, y,  1,  0, &x1, &y1)) makeLink(x, y, x1, y1, needs);
                            if (maxLink[3] && findNode(x, y, -1,  0, &x1, &y1)) makeLink(x, y, x1, y1, needs);
                            count++;
                        }
                        else if (needs == maxLink[0] + maxLink[1] + maxLink[2] + maxLink[3])
                        {
fprintf(stderr, "+++У %d %d (%d) возможные связи - %d, %d, %d, %d\n", x, y, needs, maxLink[0], maxLink[1], maxLink[2], maxLink[3]);
                            if (maxLink[0] && findNode(x, y,  0,  1, &x1, &y1)) makeLink(x, y, x1, y1, maxLink[0]);
                            if (maxLink[1] && findNode(x, y,  0, -1, &x1, &y1)) makeLink(x, y, x1, y1, maxLink[1]);
                            if (maxLink[2] && findNode(x, y,  1,  0, &x1, &y1)) makeLink(x, y, x1, y1, maxLink[2]);
                            if (maxLink[3] && findNode(x, y, -1,  0, &x1, &y1)) makeLink(x, y, x1, y1, maxLink[3]);
                            count++;
                        }
                        else
fprintf(stderr, "---У %d %d (%d) возможные связи - %d, %d, %d, %d\n", x, y, needs, maxLink[0], maxLink[1], maxLink[2], maxLink[3]);
                    }
                }
fprintf(stderr, "[ конец цикла, count = %d ]\n", count);
fprintf(stderr, "=============\n");
for (int i = 0; i < height; i++) fprintf(stderr, "%s\n", line[i]);
fprintf(stderr, "=============\n");
        } while (count > 0);

        int max = maxVal();
        if (max > 0)
        {
fprintf(stderr, "пробуем наугад ткнуть ячейку с %d неустановленных связей\n", max);
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
fprintf(stderr, "=============\n");
for (int i = 0; i < height; i++) fprintf(stderr, "%s\n", line[i]);
fprintf(stderr, "=============\n");
        }    
                    
    } 

    return 0;
}
