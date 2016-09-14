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
    
    int makedLink[width*height*4][6];
    int mkCount = 0;
    const int parX = 0;
    const int parY = 1;
    const int parX1 = 2;
    const int parY1 = 3;
    const int parHow = 4;
    const int parGuess = 5;
    int curNodeX = 0;
    int curNodeY = 0;
    
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
    
    
    int countGroups()
    {
        int groupNbr[width][height];
        int maxGroup()
        {
            int result = 0;
            for (int i = 0; i < width; i++)
                for (int j = 0; j < height; j++)
                    if (result < groupNbr[i][j]) result = groupNbr[i][j];
            return result;        
        }

        for (int i = 0; i < height; i++) 
            for (int j = 0; j < width; j++) 
                groupNbr[j][i] = 0;

        for (int i = 0; i < mkCount; i++)
        {
            int x = makedLink[i][parX]; 
            int y = makedLink[i][parY];
            int x1 = makedLink[i][parX1];
            int y1 = makedLink[i][parY1];

            if (groupNbr[x][y] == 0)
                if (groupNbr[x1][y1] == 0)
                {
                    groupNbr[x][y] = maxGroup() + 1;
                    groupNbr[x1][y1] = groupNbr[x][y];
                }
                else
                    groupNbr[x][y] = groupNbr[x1][y1];
            else
                if (groupNbr[x1][y1] == 0)
                    groupNbr[x1][y1] = groupNbr[x][y];
                else 
                {
                    int maxGr = groupNbr[x][y];
                    int minGr = groupNbr[x1][y1];
                    if (groupNbr[x1][y1] > groupNbr[x][y])
                    {
                        maxGr = groupNbr[x1][y1];
                        minGr = groupNbr[x][y];
                    }
                    for (int i = 0; i < width; i++)
                        for (int j = 0; j < height; j++)
                            if (groupNbr[i][j] == maxGr) groupNbr[i][j] = minGr;
                }
        }
        return maxGroup();
    }
    
    void makeLink(int x, int y, int x1, int y1, int how, int guess)
    {
        line[y][x] -= how;
        line[y1][x1] -= how;
//if (y == y1) fprintf(stderr, "links: %d + %d\n", horLink[y][x][x1], horLink[y][x1][x]);
//if (x == x1) fprintf(stderr, "links: %d + %d\n", verLink[x][y][y1], verLink[x][y1][y]);
        if (y == y1) horLink[y][x][x1] += how;
        if (x == x1) verLink[x][y][y1] += how;
        makedLink[mkCount][parX] = x;
        makedLink[mkCount][parY] = y;
        makedLink[mkCount][parX1] = x1;
        makedLink[mkCount][parY1] = y1;
        makedLink[mkCount][parHow] = how;
        makedLink[mkCount][parGuess] = guess;
        mkCount++;

fprintf(stderr, "%d:%d-%d:%d %d\n", x, y, x1, y1, how);
//fprintf(stderr, "=============\n");
//for (int i = 0; i < height; i++) fprintf(stderr, "%s\n", line[i]);
//fprintf(stderr, "=============\n");
    }
    
    void rollback()
    {
        do
        {
            mkCount--;
            int x = makedLink[mkCount][parX];
            int y = makedLink[mkCount][parY];
            int x1 = makedLink[mkCount][parX1];
            int y1 = makedLink[mkCount][parY1];
            int how = makedLink[mkCount][parHow];
            line[y][x] += how;
            line[y1][x1] += how;
            if (y == y1) horLink[y][x][x1] -= how;
            if (x == x1) verLink[x][y][y1] -= how;
fprintf(stderr, "[-] %d %d %d %d %d\n", x, y, x1, y1, how);
        } while (!makedLink[mkCount][parGuess]);
        curNodeX = makedLink[mkCount][parX];
        curNodeY = makedLink[mkCount][parY];
    }
    
    int setLinkFrom(int x, int y)
    {
//fprintf(stderr, "%d %d>>", x, y);
        int max = 0;
        int maxX;
        int maxY;
        int x1;
        int y1;
        int skip = 0;
        if ((x == curNodeX) && (y == curNodeY) && (mkCount > 0))
        {
            if (makedLink[mkCount][parX1] > makedLink[mkCount][parX]) skip = 1;
            if (makedLink[mkCount][parX1] < makedLink[mkCount][parX]) skip = 2;
            if (makedLink[mkCount][parY1] > makedLink[mkCount][parY]) skip = 3;
            if (makedLink[mkCount][parY1] < makedLink[mkCount][parY]) skip = 4;
        }
        if (((skip < 1) && findNode(x, y,  1,  0, &x1, &y1) && canLink(x, y, x1, y1)) ||
            ((skip < 2) && findNode(x, y, -1,  0, &x1, &y1) && canLink(x, y, x1, y1)) ||
            ((skip < 3) && findNode(x, y,  0,  1, &x1, &y1) && canLink(x, y, x1, y1)) ||
            ((skip < 4) && findNode(x, y, 0, -1, &x1, &y1) && canLink(x, y, x1, y1)))
        {
            makeLink(x, y, x1, y1, 1, 1);
            return 1;
        }
        else return 0;
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
//fprintf(stderr, "%s\n", line[i]);
    }
    
    do
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
                            if (needs > 2)
                            {
fprintf(stderr, "надо>2 связей? Откат \n");
                                rollback();
                                x = width;
                                y = height;
                            }
fprintf(stderr, "[1] ");
                            if (maxLink[0] && findNode(x, y,  0,  1, &x1, &y1)) makeLink(x, y, x1, y1, needs, 0);
                            if (maxLink[1] && findNode(x, y,  0, -1, &x1, &y1)) makeLink(x, y, x1, y1, needs, 0);
                            if (maxLink[2] && findNode(x, y,  1,  0, &x1, &y1)) makeLink(x, y, x1, y1, needs, 0);
                            if (maxLink[3] && findNode(x, y, -1,  0, &x1, &y1)) makeLink(x, y, x1, y1, needs, 0);
                            count++;
                            curNodeX = 0;
                            curNodeY = 0;
                        }
                        else if (needs == (maxLink[0] + maxLink[1] + maxLink[2] + maxLink[3]))
                        {
fprintf(stderr, "[!] ");
                            if (maxLink[0] && findNode(x, y,  0,  1, &x1, &y1)) makeLink(x, y, x1, y1, maxLink[0], 0);
                            if (maxLink[1] && findNode(x, y,  0, -1, &x1, &y1)) makeLink(x, y, x1, y1, maxLink[1], 0);
                            if (maxLink[2] && findNode(x, y,  1,  0, &x1, &y1)) makeLink(x, y, x1, y1, maxLink[2], 0);
                            if (maxLink[3] && findNode(x, y, -1,  0, &x1, &y1)) makeLink(x, y, x1, y1, maxLink[3], 0);
                            count++;
                            curNodeX = 0;
                            curNodeY = 0;
                        }
                        else if (needs == maxLink[0] + maxLink[1] + maxLink[2] + maxLink[3] - 1)                        
                        {
fprintf(stderr, "[*] ");
                            if ((maxLink[0] == 2) && findNode(x, y,  0,  1, &x1, &y1)) 
                            {
                                makeLink(x, y, x1, y1, 1, 0);
                                count++;
                                curNodeX = 0;
                                curNodeY = 0;
                            }
                            if ((maxLink[1] == 2) && findNode(x, y,  0, -1, &x1, &y1)) 
                            {
                                makeLink(x, y, x1, y1, 1, 0);
                                count++;
                                curNodeX = 0;
                                curNodeY = 0;
                            }
                            if ((maxLink[2] == 2) && findNode(x, y,  1,  0, &x1, &y1)) 
                            {
                                makeLink(x, y, x1, y1, 1, 0);
                                count++;
                                curNodeX = 0;
                                curNodeY = 0;
                            }
                            if ((maxLink[3] == 2) && findNode(x, y, -1,  0, &x1, &y1)) 
                            {
                                makeLink(x, y, x1, y1, 1, 0);
                                count++;
                                curNodeX = 0;
                                curNodeY = 0;
                            }
                        }

//                        else
//fprintf(stderr, "---У %d %d (%d) возможные связи - %d, %d, %d, %d\n", x, y, needs, maxLink[0], maxLink[1], maxLink[2], maxLink[3]);
                    }
                }
//fprintf(stderr, "[ конец цикла, count = %d ]\n", count);
//fprintf(stderr, "=============\n");
//for (int i = 0; i < height; i++) fprintf(stderr, "%s\n", line[i]);
//fprintf(stderr, "=============\n");
        } while (count > 0);
        
        int max = maxVal();
        while (max > 0)
        {
//fprintf(stderr, "пробуем наугад ткнуть ячейку с %d неустановленных связей\n", max);
fprintf(stderr, "[?] [%d:%d, %d] ", curNodeX, curNodeY, max);
            int x1 = -1;
            int y1 = -1;
            for (int y = curNodeY; (x1 < 0) && (y < height); y++) 
                for (int x = curNodeX; x < width; x++) 
                    if (line[y][x] == '0' + max) 
                    {
                        x1 = x;
                        y1 = y;
                        break;
                    }
            if (x1 < 0)
            {
                max--;
                curNodeX = 0;
                curNodeY = 0;
                if (max == 0)
                {
fprintf(stderr, "Все перепробовали, еще откат\n");
                    rollback();
                }
            }
            else if (setLinkFrom(x1, y1))
                break;
            else
            {
fprintf(stderr, "%d:%d не с чем связать\n", x1, y1);
                rollback();
            }
        }
            
//fprintf(stderr, "=============\n");
//for (int i = 0; i < height; i++) fprintf(stderr, "%s\n", line[i]);
//fprintf(stderr, "=============\n");
        if (maxVal() == 0) 
            if (countGroups() == 1)
                break;
            else
            {
fprintf(stderr, "Все раздали, граф несвязен\n");
                rollback();
            }
    } while (1);
    for (int i = 0; i < mkCount; i++)
        printf("%d %d %d %d %d\n", makedLink[i][parX], 
                                   makedLink[i][parY], 
                                   makedLink[i][parX1], 
                                   makedLink[i][parY1], 
                                   makedLink[i][parHow]);
    return 0;
}
