#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define RESET       "\033[0m"
#define RED         "\033[31m"      /* Red */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define GREEN       "\033[32m"      /* Green */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */

#define GRID_WIDTH 70
#define GRID_LENGTH 40
#define DEF_ITER_NUM 1000
#define DEF_PERCENT 80

typedef enum
{
    DEAD,
    ALIVE,
    UNDEFINED
}state_t;

typedef struct _cell
{
    state_t nextState;
    state_t currentState;
    char representation;
}cell_t;

cell_t board[GRID_WIDTH][GRID_LENGTH];
////////////////////////////////////////////////////////////////////////
/// Board related functions
////////////////////////////////////////////////////////////////////////
void initBoard(cell_t grid[][GRID_LENGTH], uint8_t fillingPercent);
void nextStep(cell_t grid[][GRID_LENGTH]);
void displayBoard(cell_t grid[][GRID_LENGTH]);
int oneStillAlive(cell_t grid[][GRID_LENGTH]);
////////////////////////////////////////////////////////////////////////
/// Cell related functions
////////////////////////////////////////////////////////////////////////
int getNeighbourNumber(cell_t grid[][GRID_LENGTH], int xPos, int yPos);
void setNextState(cell_t grid[][GRID_LENGTH], int xPos, int yPos, state_t state);
void putDiruptiveElement(cell_t grid[][GRID_LENGTH]);


int main(int argc, char *argv[])
{
    int tmp, iterNum, percent;
    if(argc == 2)
    {
        iterNum = atoi(argv[1]);
        if(iterNum == 0)
        {
            iterNum = DEF_ITER_NUM;
        }
        percent = DEF_PERCENT;
    }
    else if(argc == 3)
    {
        iterNum = atoi(argv[1]);
        if(iterNum == 0)
        {
            iterNum = DEF_ITER_NUM;
        }
        percent = atoi(argv[2]);
        if(percent == 0)
        {
            percent = DEF_PERCENT;
        }
    }
    else
    {
        iterNum = DEF_ITER_NUM;
        percent = DEF_PERCENT;
    }
    srand(time(NULL));
    initBoard(board, percent);
    displayBoard(board);
    for(tmp = 0; tmp < iterNum; tmp++)
    {
        nextStep(board);
        displayBoard(board);
        if((tmp % 100) == 0)
        {
            putDiruptiveElement(board);
        }
        if(!oneStillAlive(board))
        {
            printf("YOUR CELLS ARE ALL DEAD!!!\n");
            break;
        }
        nanosleep((const struct timespec[]){{0, 100000000L}}, NULL);
    }
    printf("Number of iterations : %d\n", tmp);
    return 0;
}


////////////////////////////////////////////////////////////////////////
/// Board related functions
////////////////////////////////////////////////////////////////////////
void initBoard(cell_t grid[][GRID_LENGTH], uint8_t fillingPercent)
{
    int x, y, luck;
    for(y = 0; y < GRID_LENGTH; y++)
    {
        for(x = 0; x < GRID_WIDTH; x++)
        {
            luck = rand() % 100;
            if(luck < fillingPercent)
            {
                grid[x][y].representation = '*';
                grid[x][y].currentState = ALIVE;
                grid[x][y].nextState = UNDEFINED;
            }
            else
            {
                grid[x][y].representation = ' ';
                grid[x][y].currentState = DEAD;
                grid[x][y].nextState = UNDEFINED;
            }
        }
    }
}

void nextStep(cell_t grid[][GRID_LENGTH])
{
    int x, y;
    for(y = 0; y < GRID_LENGTH; y++)
    {
        for(x = 0; x < GRID_WIDTH; x++)
        {
            switch(getNeighbourNumber(grid, x, y))
            {
            case 2:
                //setNextState(grid, x, y, ALIVE);
                setNextState(grid, x, y, grid[x][y].currentState);
                break;
            case 3:
                setNextState(grid, x, y, ALIVE);
                break;
            default:
                setNextState(grid, x, y, DEAD);
            }
        }
    }
}

void displayBoard(cell_t grid[][GRID_LENGTH])
{
    int x, y;
    system("clear\n");
    printf(" ");
    for(x = 0; x < GRID_WIDTH; x++)
    {
        printf("-");
    }
    printf("\n");
    for(y = 0; y < GRID_LENGTH; y++)
    {
        printf("|");
        for(x = 0; x < GRID_WIDTH; x++)
        {
            if(grid[x][y].nextState == DEAD)
            {
                printf(BOLDRED"%c"RESET, grid[x][y].representation);
            }
            else if(grid[x][y].nextState == ALIVE)
            {
                printf(BOLDGREEN"%c"RESET, grid[x][y].representation);
            }
        }
        printf("|\n");
    }
    printf(" ");
    for(x = 0; x < GRID_WIDTH; x++)
    {
        printf("-");
    }
    printf("\n");
}

int oneStillAlive(cell_t grid[][GRID_LENGTH])
{
    int x, y, cnt = GRID_LENGTH * GRID_WIDTH;
    for(y = 0; y < GRID_LENGTH; y++)
    {
        for(x = 0; x < GRID_WIDTH; x++)
        {
            if(grid[x][y].currentState == DEAD)
            {
                cnt--;
                if(!cnt)
                {
                    return 0;
                }
            }
        }
    }
    return 1;
}

////////////////////////////////////////////////////////////////////////
/// Cell related functions
////////////////////////////////////////////////////////////////////////
int getNeighbourNumber(cell_t grid[][GRID_LENGTH], int xPos, int yPos)
{
    int num = 0;
    if(((xPos - 1) >= 0) && ((yPos - 1) >= 0) &&
            grid[xPos - 1][yPos - 1].currentState == ALIVE)
    {
        num++;
    }
    if(((yPos - 1) >= 0) &&
            grid[xPos][yPos - 1].currentState == ALIVE)
    {
        num++;
    }
    if(((xPos + 1) < GRID_WIDTH) && ((yPos - 1) >= 0) &&
            grid[xPos + 1][yPos - 1].currentState == ALIVE)
    {
        num++;
    }
    if(((xPos - 1) >= 0) &&
            grid[xPos - 1][yPos].currentState == ALIVE)
    {
        num++;
    }
    if(((xPos + 1) < GRID_WIDTH) &&
            grid[xPos + 1][yPos].currentState == ALIVE)
    {
        num++;
    }
    if(((xPos - 1) >= 0) && ((yPos + 1) < GRID_LENGTH) &&
            grid[xPos - 1][yPos + 1].currentState == ALIVE)
    {
        num++;
    }
    if(((yPos + 1) < GRID_LENGTH) &&
            grid[xPos][yPos + 1].currentState == ALIVE)
    {
        num++;
    }
    if(((xPos + 1) < GRID_WIDTH) && ((yPos + 1) < GRID_LENGTH) &&
            grid[xPos + 1][yPos + 1].currentState == ALIVE)
    {
        num++;
    }
    return num;
}

void setNextState(cell_t grid[][GRID_LENGTH], int xPos, int yPos, state_t state)
{
    grid[xPos][yPos].currentState = grid[xPos][yPos].nextState;
    grid[xPos][yPos].nextState = state;
    if(grid[xPos][yPos].currentState == DEAD)
    {
        grid[xPos][yPos].representation = ' ';
    }
    else
    {
        grid[xPos][yPos].representation = '*';
    }
}

void putDiruptiveElement(cell_t grid[][GRID_LENGTH])
{
    int x = rand() % GRID_WIDTH;
    int y = rand() % GRID_LENGTH;

    setNextState(grid, x, y, ALIVE);
}
