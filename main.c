#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * The machines are gaining ground. Time to show them what we're really made of...
 **/

typedef struct link 
{
 int X, Y, X1, Y1, How, Guess;
} LINK;

int width; // the number of cells on the X axis
int height; // the number of cells on the Y axis
char** line;
int** horLink;
int** verLink;
LINK* doneLink;
int dCnt = 0;
int curNodeX = 0;
int curNodeY = 0;
int justRollback = 0;
int** groupNbr;

int nextNode(int * fromX, int * fromY)
{
 int x = *fromX;
 for (int y = *fromY; y < height; y++) 
 {
  for ( ;x < width; x++) if ((line[y][x] > '0') && (line[y][x] <= '8')) { *fromX = x; *fromY = y; return 1;}
  x = 0;
 }
 return 0;
}

int findNeighbor(int x, int y, int incX, int incY, int * x1, int * y1)
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
 if ((line[y][x] == '0') || (line[y1][x1] == '0'))
  result = 0;
 else if (y == y1)      
 {
  int xmin = x > x1 ? x1 : x;
  int xmax = x > x1 ? x : x1;
  int x2, y2;
  if (horLink[y][xmin] < 2) 
  {
   int count = 0;
   for (xmin++; xmin < xmax; xmin++)
    if (findNeighbor(xmin, y, 0, -1, &x2, &y2)) count += verLink[y2][x2];
   result = (count == 0);
  }
 }
 else if (x == x1) 
 {
  int ymin = y > y1 ? y1 : y;
  int ymax = y > y1 ? y : y1;
  int x2, y2;
  if (verLink[ymin][x] < 2)
  {
   int count = 0;
   for (ymin++; ymin < ymax; ymin++)
    if (findNeighbor(x, ymin, -1, 0, &x2, &y2)) count += horLink[y2][x2];
   result = (count == 0);
  }
 }
 return result;
}

int maxGroup()
{
 int result = 0;
 for (int i = 0; i < height; i++)
  for (int j = 0; j < width; j++)
   if (result < groupNbr[i][j]) result = groupNbr[i][j];
 return result;        
}

int countGroups()
{
 for (int i = 0; i < height; i++) 
  for (int j = 0; j < width; j++) 
   groupNbr[i][j] = 0;
 for (int i = 0; i < dCnt; i++)
 {
  int x = doneLink[i].X; 
  int y = doneLink[i].Y;
  int x1 = doneLink[i].X1;
  int y1 = doneLink[i].Y1;
  if (groupNbr[y][x] == 0)
  {
   if (groupNbr[y1][x1] == 0)
   {
    groupNbr[y][x] = maxGroup() + 1;
    groupNbr[y1][x1] = groupNbr[y][x];
   }
   else
    groupNbr[y][x] = groupNbr[y1][x1];
  }
  else if (groupNbr[y1][x1] == 0)
   groupNbr[y1][x1] = groupNbr[y][x];
  else 
  {
   int maxGr = groupNbr[y][x];
   int minGr = groupNbr[y1][x1];
   if (groupNbr[y1][x1] > groupNbr[y][x])
   {
    maxGr = groupNbr[y1][x1];
    minGr = groupNbr[y][x];
   }
   for (int i = 0; i < height; i++)
    for (int j = 0; j < width; j++)
     if (groupNbr[i][j] == maxGr) groupNbr[i][j] = minGr;
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
}
 
void countLink(int x, int y, int x1, int y1, int how)
{
 line[y][x] -= how;
 line[y1][x1] -= how;
 if (y == y1) horLink[y][x] += how;
 if (x == x1) verLink[y][x] += how;
}
    
void makeLink(int x, int y, int x1, int y1, int how, int guess)
{
 countLink(x, y, x1, y1, how);
 addLink(x, y, x1, y1, how, guess);
}
    
void rollback()
{
 do
 {
  dCnt--;
  countLink(doneLink[dCnt].X, doneLink[dCnt].Y, doneLink[dCnt].X1, doneLink[dCnt].Y1, -doneLink[dCnt].How);
 } while (!doneLink[dCnt].Guess);
 curNodeX = doneLink[dCnt].X;
 curNodeY = doneLink[dCnt].Y;
 justRollback = 1;
}
    
int setLinkFrom(int x, int y)
{
 int x1;
 int y1;
 if (justRollback && (doneLink[dCnt].Y1 > doneLink[dCnt].Y))
  return 0;
 else if (!justRollback && findNeighbor(x, y, 1, 0, &x1, &y1) && canLink(x, y, x1, y1) ||
                           findNeighbor(x, y, 0, 1, &x1, &y1) && canLink(x, y, x1, y1)) 
 {
  makeLink(x, y, x1, y1, 1, 1);
  return 1;
 }
 else
  return 0;
}

void intialPhaseAdd(int x, int y, int x1, int y1)
{
 int already = 0;
 for (int i = 0; !already && (i < dCnt); i++)
  already = ((x == doneLink[i].X) && (x1 == doneLink[i].X1) && (y == doneLink[i].Y) && (y1 == doneLink[i].Y1));
 if (!already) addLink(x, y, x1, y1, 1, 0);
}

void intialPhase()
{
 int nodes, nodes1or2, nodes2orMore;
 int x1;
 int y1;
 int onlyX;
 int onlyY;
 for (int y = 0; y < height; y++) 
 {
  for (int x = 0; x < width; x++)
  {
   nodes2orMore = 0;      
   nodes = 0;
   nodes1or2 = 0;
   if (findNeighbor(x, y,  0,  1, &x1, &y1))
   {
    if (line[y1][x1] > '1') 
    {
     nodes2orMore++;
     onlyX = x1;
     onlyY = y1;
    }
    nodes++;
    nodes1or2 += (line[y1][x1] <= '2');
   }
   if (findNeighbor(x, y,  0, -1, &x1, &y1)) 
   {
    if (line[y1][x1] > '1') 
    {
     nodes2orMore++;
     onlyX = x1;
     onlyY = y1;
    }
    nodes++;
    nodes1or2 += (line[y1][x1] <= '2');
   }
   if (findNeighbor(x, y,  1,  0, &x1, &y1)) 
   {
    if (line[y1][x1] > '1') 
    {
     nodes2orMore++;
     onlyX = x1;
     onlyY = y1;
    }
    nodes++;
    nodes1or2 += (line[y1][x1] <= '2');
   }
   if (findNeighbor(x, y, -1,  0, &x1, &y1)) 
   {
    if (line[y1][x1] > '1') 
    {
     nodes2orMore++;
     onlyX = x1;
     onlyY = y1;
    }
    nodes++;
    nodes1or2 += (line[y1][x1] <= '2');
   }

   if (nodes2orMore == 1) 
   {
    if ((onlyX >= x) && (onlyY >= y))
     intialPhaseAdd(x, y, onlyX, onlyY);
    else
     intialPhaseAdd(onlyX, onlyY, x, y);
   }
   if (line[y][x] == '2') 
   {
    if ((nodes == 2) && (nodes1or2 == 2))
    {
     if (findNeighbor(x, y,  0,  1, &x1, &y1)) intialPhaseAdd(x, y, x1, y1);
     if (findNeighbor(x, y,  0, -1, &x1, &y1)) intialPhaseAdd(x1, y1, x, y);
     if (findNeighbor(x, y,  1,  0, &x1, &y1)) intialPhaseAdd(x, y, x1, y1);
     if (findNeighbor(x, y, -1,  0, &x1, &y1)) intialPhaseAdd(x1, y1, x, y);
    }
   }
  }
 }
 for (int i = 0; i < dCnt; i++)
  countLink(doneLink[i].X, doneLink[i].Y, doneLink[i].X1, doneLink[i].Y1, doneLink[i].How);
}

int main()
{
 scanf("%d", &width);
 scanf("%d", &height); fgetc(stdin);
 horLink = (int**) malloc(height * sizeof(int*));
 verLink = (int**) malloc(height * sizeof(int*));
 groupNbr = (int**) malloc(height * sizeof(int*));
 line = (char**) malloc(height * sizeof(char*));
 for (int i = 0; i < height; i++)
 {
  horLink[i] = (int*)  malloc(width * sizeof(int));
  verLink[i] = (int*)  malloc(width * sizeof(int));
  groupNbr[i] = (int*)  malloc(width * sizeof(int));
  line[i]    = (char*) malloc((width+1) * sizeof(char));
 }
 doneLink = (LINK*) malloc((width*height*4) * sizeof(LINK));
 dCnt = 0;
 curNodeX = 0;
 curNodeY = 0;
 justRollback = 0;
    
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
                            if (y == y1)
                             max1 -= (x < x1) ? horLink[y][x] : horLink[y][x1]; 
                            else
                             max1 -= (y < y1) ? verLink[y][x] : verLink[y1][x]; 
                            int max2 = (line[y1][x1] > '1') ? 2 : 1;
                            return (max1 < max2) ? max1 : max2;
                        }
                        if (findNeighbor(x, y,  0,  1, &x1, &y1)) if (canLink(x, y, x1, y1))
                        {
                            nodes++;
                            maxLink[0] = getMaxLink(x, y, x1, y1);
                        }
                        if (findNeighbor(x, y,  0, -1, &x1, &y1)) if (canLink(x, y, x1, y1))
                        {
                            nodes++;
                            maxLink[1] = getMaxLink(x, y, x1, y1);
                        }
                        if (findNeighbor(x, y,  1,  0, &x1, &y1)) if (canLink(x, y, x1, y1))
                        {
                            nodes++;
                            maxLink[2] = getMaxLink(x, y, x1, y1);
                        }
                        if (findNeighbor(x, y, -1,  0, &x1, &y1)) if (canLink(x, y, x1, y1))
                        {
                            nodes++;
                            maxLink[3] = getMaxLink(x, y, x1, y1);
                        }
                        if (nodes == 1)
                        {
                            if (needs > 2)
                            {
                                rollback();
                                x = width;
                                y = height;
                            }
                            else
                            {
                             if (maxLink[0] && findNeighbor(x, y,  0,  1, &x1, &y1)) makeLink(x, y, x1, y1, needs, 0);
                             if (maxLink[1] && findNeighbor(x, y,  0, -1, &x1, &y1)) makeLink(x1, y1, x, y, needs, 0);
                             if (maxLink[2] && findNeighbor(x, y,  1,  0, &x1, &y1)) makeLink(x, y, x1, y1, needs, 0);
                             if (maxLink[3] && findNeighbor(x, y, -1,  0, &x1, &y1)) makeLink(x1, y1, x, y, needs, 0);
                             count++;
                             curNodeX = 0;
                             curNodeY = 0;
                            }
                        }
                        else if (needs == (maxLink[0] + maxLink[1] + maxLink[2] + maxLink[3]))
                        {
                            if (maxLink[0] && findNeighbor(x, y,  0,  1, &x1, &y1)) makeLink(x, y, x1, y1, maxLink[0], 0);
                            if (maxLink[1] && findNeighbor(x, y,  0, -1, &x1, &y1)) makeLink(x1, y1, x, y, maxLink[1], 0);
                            if (maxLink[2] && findNeighbor(x, y,  1,  0, &x1, &y1)) makeLink(x, y, x1, y1, maxLink[2], 0);
                            if (maxLink[3] && findNeighbor(x, y, -1,  0, &x1, &y1)) makeLink(x1, y1, x, y, maxLink[3], 0);
                            count++;
                            curNodeX = 0;
                            curNodeY = 0;
                        }
                        else if (needs == maxLink[0] + maxLink[1] + maxLink[2] + maxLink[3] - 1)                        
                        {
                            if ((maxLink[0] == 2) && findNeighbor(x, y,  0,  1, &x1, &y1)) 
                            {
                                makeLink(x, y, x1, y1, 1, 0);
                                count++;
                                curNodeX = 0;
                                curNodeY = 0;
                            }
                            if ((maxLink[1] == 2) && findNeighbor(x, y,  0, -1, &x1, &y1)) 
                            {
                                makeLink(x1, y1, x, y, 1, 0);
                                count++;
                                curNodeX = 0;
                                curNodeY = 0;
                            }
                            if ((maxLink[2] == 2) && findNeighbor(x, y,  1,  0, &x1, &y1)) 
                            {
                                makeLink(x, y, x1, y1, 1, 0);
                                count++;
                                curNodeX = 0;
                                curNodeY = 0;
                            }
                            if ((maxLink[3] == 2) && findNeighbor(x, y, -1,  0, &x1, &y1)) 
                            {
                                makeLink(x1, y1, x, y, 1, 0);
                                count++;
                                curNodeX = 0;
                                curNodeY = 0;
                            }
                        }
                    }
                }
        } while (count > 0);

        if (nextNode(&curNodeX, &curNodeY)) 
        {
         if (!setLinkFrom(curNodeX, curNodeY)) rollback();
        }
        else if (countGroups() == 1)
         break;
        else
         rollback();//Все раздали, граф несвязен
    } while (1);
    for (int i = 0; i < dCnt; i++)
        printf("%d %d %d %d %d\n", doneLink[i].X, doneLink[i].Y, doneLink[i].X1, doneLink[i].Y1, doneLink[i].How);
    return 0;
}
