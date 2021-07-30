#include <stdlib.h> /* srand, rand */
#include <time.h> /* time */
#include<conio.h> /*to Windows getche()*/
#include <bits/stdc++.h>
#define ll long long
using namespace std;


int X[]= {0,1,0,-1};  // adjacent Moves
int Y[]= {-1,0,1,0}; // adjacent Moves

//Possible points
bool isValid(ll x,ll y)
{
    if(x<0||x>=20||y<0||y>=20) return false;
    return true;
}

//key codes
const int KEY_D = 100;
const int KEY_S = 115;
const int KEY_A = 97;
const int KEY_W = 119;

//fields
const int initial = 0;
const int wall = 1;
const int player = 2147483646;
const int monster = 2147483645;

//chars
const char playerChar = 'P';
const char monsterChar = 'M';
const char emptyChar = ' ';
const char wallChar = ':';

//moves
const int toRight = 0;
const int toDown = 1;
const int toLeft = 2;
const int toUp = 3;


const int mazeSize = 20;

//Maze printing
void printMaze(int maze[mazeSize][mazeSize])
{
    for (int row = 0; row < mazeSize; row++)
    {
        for (int column = 0; column < mazeSize; column++)
        {
            int field = maze[row][column];
            cout << (field == wall ? wallChar : field == monster ? monsterChar : field == player ? playerChar : emptyChar);
        }
        cout << endl;
    }
    cout << "Please enter a direction w, a, s, d or E to exit:";
}

//Possible to move on the point
bool isPossibleMoveTo(int maze[mazeSize][mazeSize], int moveTo, int currentRow, int currentColumn)
{
    bool isPossibleMoveTo = false;

    //TODO add the check to see if the adjacents positions are walls

    switch (moveTo)
    {
    case toRight:
        isPossibleMoveTo = currentColumn < mazeSize - 1 && maze[currentRow][currentColumn + 1] != wall;
        break;
    case toDown:
        isPossibleMoveTo = currentRow < mazeSize - 1 && maze[currentRow + 1][currentColumn] != wall;
        break;
    case toLeft:
        isPossibleMoveTo = currentColumn > 0 && maze[currentRow][currentColumn - 1] != wall;
        break;
    case toUp:
        isPossibleMoveTo = currentRow > 0 && maze[currentRow - 1][currentColumn] != wall;
        break;
    }

    return isPossibleMoveTo;
}

//TODO rename this method name (I'm not realy taking of a place and putting in another)
bool moveOnMaze(int maze[mazeSize][mazeSize], int moveTo, int field, int& currentRow, int& currentColumn, bool clearCurrent)
{
    if (!isPossibleMoveTo(maze, moveTo, currentRow, currentColumn))
    {
        return false;
    }

    bool moved = moveTo == toRight || moveTo == toDown || moveTo == toLeft || moveTo == toUp;


    if (moved)
    {
        if (clearCurrent)
        {
            maze[currentRow][currentColumn] = initial;
        }
        switch (moveTo)
        {
        case toRight:
            currentColumn++;
            break;
        case toDown:
            currentRow++;
            break;
        case toLeft:
            currentColumn--;
            break;
        case toUp:
            currentRow--;
            break;
        }
        maze[currentRow][currentColumn] = field;
    }
    return moved;
}


bool isGameEnd(int maze[mazeSize][mazeSize])
{
    return maze[mazeSize-2][mazeSize - 1] == player;
}

//BFS to find the shortest path/distacne form monster to player
int bfs_2d(int mz[20][20], ll x,ll y, ll x2, ll y2)
{
    ll dis[20][20];
    ll vis[20][20];
    memset(dis,0,sizeof(dis));
    memset(vis,0,sizeof(vis));
    queue<pair<ll,ll> >q;
    q.push({x,y});
    vis[x][y]=1;
    vis[x2][y2]=-1;

    while(!q.empty())
    {
        ll xx = q.front().first;
        ll yy = q.front().second;
        q.pop();

        for(int i=0; i<4; i++)
        {
            if(isValid(xx+X[i],yy+Y[i])&&vis[xx+X[i]][yy+Y[i]]!=1&&mz[xx+X[i]][yy+Y[i]]!=wall)
            {
                ll newX = xx + X[i];
                ll newY = yy + Y[i];

                if(vis[newX][newY] == -1)
                {
                    int x = dis[xx][yy]+1;
                    return x;
                }

                q.push({newX,newY});
                vis[newX][newY] = 1;
                dis[newX][newY] = dis[xx][yy]+1;
            }
        }
    }
}

void startGame(int maze[mazeSize][mazeSize])
{
    int currentRow = 0;
    int currentColumn = 0;
    int currentRow2 = 18;
    int currentColumn2 = 10;

    maze[currentRow][currentColumn] = player;
    maze[currentRow2][currentColumn2] = monster;

    printMaze(maze);

//	char keyPressed;
    int keyPressed;
    int moveTo=-1;
    int moveToMonster=-1;
    int lastMove = -1;
    bool win = true;

    while (!isGameEnd(maze))
    {
        //use w, d, s, a for moving the player

        keyPressed = _getche();

        switch (keyPressed)
        {
        case KEY_W://'w':
            moveTo = toUp;
            break;
        case KEY_D://'d':
            moveTo = toRight;
            break;
        case KEY_S://'s':
            moveTo = toDown;
            break;
        case KEY_A://'a':
            moveTo = toLeft;
            break;
        }

        //player move
        bool moved = moveOnMaze(maze, moveTo, player, currentRow, currentColumn, true);


        //Player position
        int des_x = currentRow;
        int des_y = currentColumn;

        int dist = 2000;
        if(moved && maze[currentRow2+1][currentColumn2]!=wall && isValid(currentRow2+1, currentColumn2))
        {
            //Monster position
            int source_x = currentRow2+1;
            int source_y = currentColumn2;

            int currentDist = bfs_2d(maze, des_x, des_y, source_x, source_y);
            if(dist > currentDist)
            {
                //Monster new position
                moveToMonster = toDown;
                dist = currentDist;
            }
        }
        if(moved && maze[currentRow2-1][currentColumn2]!=wall && isValid(currentRow2-1, currentColumn2))
        {
            int source_x = currentRow2-1;
            int source_y = currentColumn2;

            int currentDist = bfs_2d(maze, des_x, des_y, source_x, source_y);
            if(dist > currentDist)
            {
                moveToMonster = toUp;
                dist = currentDist;
            }
        }
        if(moved && maze[currentRow2][currentColumn2+1]!=wall && isValid(currentRow2, currentColumn2+1))
        {
            int source_x = currentRow2;
            int source_y = currentColumn2+1;

            int currentDist = bfs_2d(maze, des_x, des_y, source_x, source_y);
            if(dist > currentDist)
            {
                moveToMonster = toRight;
                dist = currentDist;
            }

        }
        if(moved && maze[currentRow2][currentColumn2-1]!=wall && isValid(currentRow2, currentColumn2-1))
        {
            int source_x = currentRow2;
            int source_y = currentColumn2-1;

            int currentDist = bfs_2d(maze, des_x, des_y, source_x, source_y);
            if(dist > currentDist)
            {
                moveToMonster = toLeft;
                dist = currentDist;
            }
        }

        if(moved)
        {
            bool movedd = moveOnMaze(maze, moveToMonster, monster, currentRow2, currentColumn2, true);
        }

        lastMove = moveTo;
        system("cls");
        printMaze(maze);
        if(currentRow == currentRow2 && currentColumn == currentColumn2)
        {
            win = false;
            break;
        }
    }

    if(win)
    {
        cout << endl << " ----- Congratulations your cleared The Dungeon  ----- " << endl;
    }
    else
    {
        cout << endl << " ----- Game is over ----- " << endl;
    }

}


int main()
{
    system("Color E4");
    //int maze[mazeSize][mazeSize];
    //0 = field 1 = wall
    int field = initial;
    int maze[mazeSize][mazeSize] =
    {
        {field,field,wall,wall,wall,wall,wall,wall,wall,wall,wall,wall,wall,wall,wall,wall,wall,wall,wall,wall},
        {wall,field,wall,field,field,field,field,wall,wall,field,field,field,field,field,field,field,wall,wall,wall,wall},
        {wall,field,wall,wall,wall,wall,field,wall,wall,field,field,field,field,wall,wall,field,wall,field,field,wall},
        {wall,field,field,field,field,field,field,wall,wall,field,field,field,field,wall,field,field,field,field,wall,wall},
        {wall,wall,field,wall,wall,wall,wall,wall,wall,field,field,field,field,wall,field,wall,wall,field,wall,wall},
        {wall,field,field,field,wall,field,field,wall,wall,wall,field,wall,wall,wall,field,wall,wall,field,field,wall},
        {wall,field,wall,field,field,field,field,wall,wall,wall,field,wall,wall,wall,wall,wall,wall,wall,field,wall},
        {wall,field,wall,field,wall,wall,wall,wall,wall,wall,field,wall,wall,wall,wall,wall,wall,wall,field,wall},
        {wall,field,wall,field,field,field,field,field,field,field,field,field,wall,wall,wall,wall,field,field,field,wall},
        {wall,wall,wall,wall,wall,field,wall,wall,wall,wall,field,wall,wall,wall,wall,wall,field,field,wall,wall},
        {wall,wall,wall,wall,wall,field,wall,wall,wall,wall,field,wall,wall,wall,wall,wall,field,field,wall,wall},
        {wall,field,field,field,field,field,wall,wall,wall,field,field,field,wall,wall,wall,wall,wall,field,field,wall},
        {wall,field,field,field,field,wall,wall,wall,wall,field,field,field,field,field,field,field,wall,wall,field,wall},
        {wall,field,field,field,field,wall,wall,wall,wall,field,field,field,field,field,field,field,wall,wall,field,wall},
        {wall,field,field,field,field,wall,wall,wall,wall,field,field,field,wall,field,field,field,wall,wall,field,wall},
        {wall,wall,wall,wall,field,field,wall,wall,wall,field,field,field,wall,field,field,field,wall,wall,field,wall},
        {wall,wall,wall,wall,wall,field,wall,wall,wall,field,field,field,wall,wall,wall,wall,wall,field,field,wall},
        {wall,field,wall,field,field,field,field,wall,wall,wall,wall,wall,wall,wall,wall,wall,wall,field,wall,wall},
        {wall,field,wall,field,field,wall,field,field,field,field,field,wall,wall,wall,wall,wall,wall,field,field,field},
        {wall,wall,wall,wall,wall,wall,wall,wall,wall,wall,wall,wall,wall,wall,wall,wall,wall,wall,wall,wall},
    };
    startGame(maze);
    system("pause");
    return 0;
}

