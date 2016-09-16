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
fprintf(stderr, "%d:%d-%d:%d %d %d\n", x, y, x1, y1, how, guess);
    }
    
    void countLink(int x, int y, int x1, int y1, int how)
    {
        line[y][x] -= how;
        line[y1][x1] -= how;
//if (y == y1) fprintf(stderr, "links: %d + %d\n", horLink[y][x][x1], horLink[y][x1][x]);
//if (x == x1) fprintf(stderr, "links: %d + %d\n", verLink[x][y][y1], verLink[x][y1][y]);
        if (y == y1) horLink[y][x][x1] += how;
        if (x == x1) verLink[x][y][y1] += how;
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
            for (int k = 0; k < height; k++) verLink[j][i][k] = 0;
            for (int k = 0; k < width;  k++) horLink[i][j][k] = 0;
        }
        fgetc(stdin);
        line[i][width] = 0;
//fprintf(stderr, "%s\n", line[i]);
    }
    
    void intialPhaseAdd(int x, int y, int x1, int y1)
    {
        int already = 0;
        for (int i = 0; !already && (i < dCnt); i++)
            already = ((x == doneLink[i].X) && (x1 == doneLink[i].X1) || 
                       (x1 == doneLink[i].X) && (x == doneLink[i].X1)) &&
                      ((y == doneLink[i].Y) && (y1 == doneLink[i].Y1) ||        
                       (y1 == doneLink[i].Y) && (y == doneLink[i].Y1));
        if (!already) addLink(x, y, x1, y1, 1, 0);
    }
    
    for (int y = 0; y < height; y++) 
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
    for (int i = 0; i < dCnt; i++)
        countLink(doneLink[i].X, doneLink[i].Y, doneLink[i].X1, doneLink[i].Y1, doneLink[i].How);
    
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
//fprintf(stderr, "надо>2 связей? Откат \n");
                                rollback();
                                x = width;
                                y = height;
                            }
//fprintf(stderr, "[1] ");
                            if (maxLink[0] && findNode(x, y,  0,  1, &x1, &y1)) makeLink(x, y, x1, y1, needs, 0);
                            if (maxLink[1] && findNode(x, y,  0, -1, &x1, &y1)) makeLink(x1, y1, x, y, needs, 0);
                            if (maxLink[2] && findNode(x, y,  1,  0, &x1, &y1)) makeLink(x, y, x1, y1, needs, 0);
                            if (maxLink[3] && findNode(x, y, -1,  0, &x1, &y1)) makeLink(x1, y1, x, y, needs, 0);
                            count++;
                            curNodeX = 0;
                            curNodeY = 0;
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
//fprintf(stderr, "[ конец цикла, count = %d ]\n", count);
//fprintf(stderr, "=============\n");
//for (int i = 0; i < height; i++) fprintf(stderr, "%s\n", line[i]);
//fprintf(stderr, "=============\n");

        } while (count > 0);

fprintf(stderr, "[?] [%d:%d] ", curNodeX, curNodeY);
        if (nextNode(&curNodeX, &curNodeY)) 
{        
fprintf(stderr, "[?] [%d:%d] ", curNodeX, curNodeY);
         if (!setLinkFrom(curNodeX, curNodeY)) rollback();
}
        else    
{        
//fprintf(stderr, "нет ничего после [%d:%d], групп - %d ", curNodeX, curNodeY, countGroups());

         if (countGroups() == 1)
          break;
         else
          rollback();//Все раздали, граф несвязен
}          
    } while (1);
    for (int i = 0; i < dCnt; i++)
        printf("%d %d %d %d %d\n", doneLink[i].X, doneLink[i].Y, doneLink[i].X1, doneLink[i].Y1, doneLink[i].How);
    return 0;
}
