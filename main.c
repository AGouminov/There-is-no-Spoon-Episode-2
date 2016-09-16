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
 int horLink[height][width];
 int verLink[height][width];

 typedef struct link 
 {
  int X, Y, X1, Y1, How, Guess;
 } LINK;
 
 LINK doneLink[width*height*4];
 int dCnt = 0;
 int curNodeX = 0;
 int curNodeY = 0;
 int justRollback = 0;
    
 int nextNode(int * fromX, int * fromY)
 {
  int x = *fromX;
  int y = *fromY;
  for ( ; y < height; y++) 
  {
   for ( ;x < width; x++) 
   {
    if ((line[y][x] > '0') && (line[y][x] <= '8')) 
    {
     *fromX = x;
     *fromY = y;
     return 1;
    }
   }
   x = 0;
  }
  return 0;
 }

    int findNode(int x, int y, int incX, int incY, int * x1, int * y1)
    {
        int result = 0;
        int xi = x + incX;
        int yi = y + incY;
        while (!result && (xi >= 0) && (xi < width) && (yi >= 0) && (yi < height))
        {
         if ((line[yi][xi] >= '0') && (line[yi][xi] <= '8')) 
         {
          result = 1;
          *x1 = xi;
          *y1 = yi;
         }
         xi += incX;
         yi += incY;
        }
        return result;
    }
    
    int canLink(int x, int y, int x1, int y1)
    {
        int result = 1;
        int xmin = x > x1 ? x1 : x;
        int xmax = x > x1 ? x : x1;
        int ymin = y > y1 ? y1 : y;
        int ymax = y > y1 ? y : y1;
        int x2, y2;

if ((x==2)&&(y==2)) fprintf(stderr, "%d:%d(%c)-%d:%d(%c)|", xmin, ymin, line[ymin][xmin], xmax, ymax, line[ymax][xmax]);

        if ((line[ymax][xmax] == '0') || (line[ymin][xmin] == '0'))
            result = 0;
        else if (ymin == ymax)      
        {
if ((x==2)&&(y==2)) fprintf(stderr, "%d", horLink[ymin][xmin]);
            if (horLink[ymin][xmin] < 2) 
            {
                int count = 0;
                for (xmin++; xmin < xmax; xmin++)
{                
if ((x==2)&&(y==2)) fprintf(stderr, "%d", xmin);
                 if (findNode(xmin, ymin, 0, -1, &x2, &y2))
{                 
if ((x==2)&&(y==2)) fprintf(stderr, ":%d %d|", y2, verLink[y2][x2]);
                  count += verLink[y2][x2];
}
}
                result = (count == 0);
            }
        }
        else if (xmin == xmax) 
        {
if ((x==2)&&(y==2)) fprintf(stderr, "%d", verLink[ymin][xmin]);
            if (verLink[ymin][xmin] < 2)
            {
                int count = 0;
                for (ymin++; ymin < ymax; ymin++)
                 if (findNode(xmin, ymin, -1, 0, &x2, &y2))
                  count += horLink[y2][x2];
                result = (count == 0);
            }
        }
if ((x==2)&&(y==2)) fprintf(stderr, " %d\n", result);
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
                {
                    if (result < groupNbr[i][j]) result = groupNbr[i][j];
                }
            return result;        
        }

        for (int i = 0; i < width; i++) 
            for (int j = 0; j < height; j++) 
                groupNbr[i][j] = 0;

        for (int i = 0; i < dCnt; i++)
        {
            int x = doneLink[i].X; 
            int y = doneLink[i].Y;
            int x1 = doneLink[i].X1;
            int y1 = doneLink[i].Y1;

            if (groupNbr[x][y] == 0)
                if (groupNbr[x1][y1] == 0)
                {
                    groupNbr[x][y] = maxGroup() + 1;
                    groupNbr[x1][y1] = groupNbr[x][y];
                }
                else
                {
                    groupNbr[x][y] = groupNbr[x1][y1];
                }
            else
                if (groupNbr[x1][y1] == 0)
                {
                    groupNbr[x1][y1] = groupNbr[x][y];
                }
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
                            if (groupNbr[i][j] == maxGr)
                            {
                                groupNbr[i][j] = minGr;
                            }    
                }
        }
        return maxGroup();
    }
    
    void addLink(int x, int y, int x1, int y1, int how, int guess)
    {
        justRollback = 0;
        doneLink[dCnt].X = x;
        doneLink[dCnt].Y = y;
        doneLink[dCnt].X1 = x1;
        doneLink[dCnt].Y1 = y1;
        doneLink[dCnt].How = how;
        doneLink[dCnt].Guess = guess;
        dCnt++;
fprintf(stderr, "%2d:%2d - %2d:%2d   %d   %d\n", x, y, x1, y1, how, guess);
    }
    
    void countLink(int x, int y, int x1, int y1, int how)
    {
        line[y][x] -= how;
        line[y1][x1] -= how;
//if (y == y1) fprintf(stderr, "links: %d + %d\n", horLink[y][x][x1], horLink[y][x1][x]);
//if (x == x1) fprintf(stderr, "links: %d + %d\n", verLink[x][y][y1], verLink[x][y1][y]);
        if (y == y1) horLink[y][x] += how;
        if (x == x1) verLink[y][x] += how;
    }
    
    void makeLink(int x, int y, int x1, int y1, int how, int guess)
    {
        countLink(x, y, x1, y1, how);
        addLink(x, y, x1, y1, how, guess);

//fprintf(stderr, "=============\n");
//for (int i = 0; i < height; i++) fprintf(stderr, "%s\n", line[i]);
//fprintf(stderr, "=============\n");
    }
    
    void rollback()
    {
        do
        {
            dCnt--;
            countLink(doneLink[dCnt].X, doneLink[dCnt].Y, doneLink[dCnt].X1, doneLink[dCnt].Y1, -doneLink[dCnt].How);
fprintf(stderr, "[-] %d %d %d %d %d\n", doneLink[dCnt].X, doneLink[dCnt].Y, doneLink[dCnt].X1, doneLink[dCnt].Y1, doneLink[dCnt].How);
        } while (!doneLink[dCnt].Guess);
        curNodeX = doneLink[dCnt].X;
        curNodeY = doneLink[dCnt].Y;
        justRollback = 1;
    }
    
    int setLinkFrom(int x, int y)
    {
        int x1;
        int y1;
//fprintf(stderr, "\n=============\n");
//for (int i = 0; i < height; i++) fprintf(stderr, "%s\n", line[i]);
//fprintf(stderr, "=============\n");
        if (justRollback && (doneLink[dCnt].Y1 > doneLink[dCnt].Y))
         return 0;
        else if (!justRollback && findNode(x, y, 1, 0, &x1, &y1) && canLink(x, y, x1, y1) ||
                 findNode(x, y, 0, 1, &x1, &y1) && canLink(x, y, x1, y1)) 
        {
         makeLink(x, y, x1, y1, 1, 1);
         return 1;
        }
        else
         return 0;
    }

    for (int i = 0; i < height; i++) 
    {
        for (int j = 0; j < width; j++) 
        {
            line[i][j] = fgetc(stdin); // width characters, each either a number or a '.'
            verLink[i][j] = 0;
            horLink[i][j] = 0;
        }
        fgetc(stdin);
        line[i][width] = 0;
//fprintf(stderr, "%s\n", line[i]);
    }
//fprintf(stderr, "=============\n");
//for (int i = 0; i < height; i++) fprintf(stderr, "%s\n", line[i]);
//fprintf(stderr, "=============\n");
    
    void intialPhaseAdd(int x, int y, int x1, int y1)
    {
        int already = 0;
        for (int i = 0; !already && (i < dCnt); i++)
            already = ((x == doneLink[i].X) && (x1 == doneLink[i].X1) && (y == doneLink[i].Y) && (y1 == doneLink[i].Y1));
        if (!already) addLink(x, y, x1, y1, 1, 0);
    }
    
    for (int y = 0; y < height; y++) 
    {
        for (int x = 0; x < width; x++)
        {
            if (line[y][x] == '1') 
            {
                int nodes = 0;
                int x1;
                int y1;
                int onlyX;
                int onlyY;
                if (findNode(x, y,  0,  1, &x1, &y1) && (line[y1][x1] > '1')) 
                {
                    nodes++;
                    onlyX = x1;
                    onlyY = y1;
                }
                if (findNode(x, y,  0, -1, &x1, &y1) && (line[y1][x1] > '1')) 
                {
                    nodes++;
                    onlyX = x1;
                    onlyY = y1;
                }
                if (findNode(x, y,  1,  0, &x1, &y1) && (line[y1][x1] > '1')) 
                {
                    nodes++;
                    onlyX = x1;
                    onlyY = y1;
                }
                if (findNode(x, y, -1,  0, &x1, &y1) && (line[y1][x1] > '1')) 
                {
                    nodes++;
                    onlyX = x1;
                    onlyY = y1;
                }
                if (nodes == 1) 
		         if ((onlyX >= x) && (onlyY >= y))
                  intialPhaseAdd(x, y, onlyX, onlyY);
                 else
                  intialPhaseAdd(onlyX, onlyY, x, y);
            }
            else if (line[y][x] == '2') 
            {
                int x1;
                int y1;
                int nodes = 0;
                int nodes1or2 = 0;
                if (findNode(x, y,  0,  1, &x1, &y1))
                {
                    nodes++;
                    nodes1or2 += (line[y1][x1] <= '2');
                }
                if (findNode(x, y,  0, -1, &x1, &y1))
                {
                    nodes++;
                    nodes1or2 += (line[y1][x1] <= '2');
                }
                if (findNode(x, y,  1,  0, &x1, &y1))
                {
                    nodes++;
                    nodes1or2 += (line[y1][x1] <= '2');
                }
                if (findNode(x, y, -1,  0, &x1, &y1))
                {
                    nodes++;
                    nodes1or2 += (line[y1][x1] <= '2');
                }
                if ((nodes == 2) && (nodes1or2 == 2))
                {
                    if (findNode(x, y,  0,  1, &x1, &y1)) intialPhaseAdd(x, y, x1, y1);
                    if (findNode(x, y,  0, -1, &x1, &y1)) intialPhaseAdd(x1, y1, x, y);
                    if (findNode(x, y,  1,  0, &x1, &y1)) intialPhaseAdd(x, y, x1, y1);
                    if (findNode(x, y, -1,  0, &x1, &y1)) intialPhaseAdd(x1, y1, x, y);
                }
            }
        }
    }

    for (int i = 0; i < dCnt; i++)
        countLink(doneLink[i].X, doneLink[i].Y, doneLink[i].X1, doneLink[i].Y1, doneLink[i].How);

    do
    {
        int count;
        do
        {
fprintf(stderr, "=============\n");
for (int i = 0; i < height; i++) fprintf(stderr, "%s\n", line[i]);
fprintf(stderr, "=============\n");
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
                            if (y == y1)
                             max1 -= (x < x1) ? horLink[y][x] : horLink[y][x1]; 
                            else
                             max1 -= (y < y1) ? verLink[y][x] : verLink[y1][x]; 
                            int max2 = (line[y1][x1] > '1') ? 2 : 1;
                            return (max1 < max2) ? max1 : max2;
                        }
                        if (findNode(x, y,  0,  1, &x1, &y1)) if (canLink(x, y, x1, y1))
                        {
                            nodes++;
                            maxLink[0] = getMaxLink(x, y, x1, y1);
                        }
                        if (findNode(x, y,  0, -1, &x1, &y1)) if (canLink(x, y, x1, y1))
                        {
                            nodes++;
                            maxLink[1] = getMaxLink(x, y, x1, y1);
                        }
                        if (findNode(x, y,  1,  0, &x1, &y1)) if (canLink(x, y, x1, y1))
                        {
                            nodes++;
                            maxLink[2] = getMaxLink(x, y, x1, y1);
                        }
                        if (findNode(x, y, -1,  0, &x1, &y1)) if (canLink(x, y, x1, y1))
                        {
                            nodes++;
                            maxLink[3] = getMaxLink(x, y, x1, y1);
                        }
fprintf(stderr, "[%d:%d]nodes %d needs %d max %d %d %d %d\n", x, y, nodes, needs, maxLink[0], maxLink[1], maxLink[2], maxLink[3]);
                        if (nodes == 1)
                        {
                            if (needs > 2)
                            {
//fprintf(stderr, "%d:%d надо >2 связей? Откат \n", x, y);
                                rollback();
                                x = width;
                                y = height;
                            }
                            else
                            {
//fprintf(stderr, "[1] ");
                             if (maxLink[0] && findNode(x, y,  0,  1, &x1, &y1)) makeLink(x, y, x1, y1, needs, 0);
                             if (maxLink[1] && findNode(x, y,  0, -1, &x1, &y1)) makeLink(x1, y1, x, y, needs, 0);
                             if (maxLink[2] && findNode(x, y,  1,  0, &x1, &y1)) makeLink(x, y, x1, y1, needs, 0);
                             if (maxLink[3] && findNode(x, y, -1,  0, &x1, &y1)) makeLink(x1, y1, x, y, needs, 0);
                             count++;
                             curNodeX = 0;
                             curNodeY = 0;
                            }
                        }
                        else if (needs == (maxLink[0] + maxLink[1] + maxLink[2] + maxLink[3]))
                        {
//fprintf(stderr, "[!] ");
                            if (maxLink[0] && findNode(x, y,  0,  1, &x1, &y1)) makeLink(x, y, x1, y1, maxLink[0], 0);
                            if (maxLink[1] && findNode(x, y,  0, -1, &x1, &y1)) makeLink(x1, y1, x, y, maxLink[1], 0);
                            if (maxLink[2] && findNode(x, y,  1,  0, &x1, &y1)) makeLink(x, y, x1, y1, maxLink[2], 0);
                            if (maxLink[3] && findNode(x, y, -1,  0, &x1, &y1)) makeLink(x1, y1, x, y, maxLink[3], 0);
                            count++;
                            curNodeX = 0;
                            curNodeY = 0;
                        }
                        else if (needs == maxLink[0] + maxLink[1] + maxLink[2] + maxLink[3] - 1)                        
                        {
//fprintf(stderr, "[*] ");
                            if ((maxLink[0] == 2) && findNode(x, y,  0,  1, &x1, &y1)) 
                            {
                                makeLink(x, y, x1, y1, 1, 0);
                                count++;
                                curNodeX = 0;
                                curNodeY = 0;
                            }
                            if ((maxLink[1] == 2) && findNode(x, y,  0, -1, &x1, &y1)) 
                            {
                                makeLink(x1, y1, x, y, 1, 0);
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
                                makeLink(x1, y1, x, y, 1, 0);
                                count++;
                                curNodeX = 0;
                                curNodeY = 0;
                            }
                        }

//                        else
//fprintf(stderr, "---У %d %d (%d) возможные связи - %d, %d, %d, %d\n", x, y, needs, maxLink[0], maxLink[1], maxLink[2], maxLink[3]);
                    }
                }
        } while (count > 0);

fprintf(stderr, "[?] [%d:%d] ", curNodeX, curNodeY);
        if (nextNode(&curNodeX, &curNodeY)) 
{        
fprintf(stderr, "[?] [%d:%d] ", curNodeX, curNodeY);
         if (!setLinkFrom(curNodeX, curNodeY)) 
         {
fprintf(stderr, "не с чем связать\n");
            rollback();
         }
}
        else    
{        
//fprintf(stderr, "нет ничего после [%d:%d], групп - %d ", curNodeX, curNodeY, countGroups());

         if (countGroups() == 1)
          break;
         else
fprintf(stderr, "Все раздали, граф несвязен\n");
          rollback();//Все раздали, граф несвязен
}          
    } while (1);
    for (int i = 0; i < dCnt; i++)
        printf("%d %d %d %d %d\n", doneLink[i].X, doneLink[i].Y, doneLink[i].X1, doneLink[i].Y1, doneLink[i].How);
    return 0;
}
