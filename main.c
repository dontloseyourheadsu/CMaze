#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static char labyrinth[10][20];
static int labyrinthHeight;
static int labyrinthWidth;
static char goal = '#';
static char player = 'o';
static char wall = '-';
static char unvisitedPath = '=';
static char path = ' ';
static char solutionPath = '*';
static char possibleSolutionPath = '+';

void printNewLines(int amountOfLines) {
    for (int i = 0; i < amountOfLines; i++) {
        printf("\n");
    }
}

int randomChoice(int base, int limit) {

    int randomChoice = (rand() % limit) + base;

    return randomChoice;
}

int isEven(int number) { return number % 2; }

void printLabyrinth() {

    for (int i = 0; i < labyrinthHeight; i++) {
        for (int j = 0; j < labyrinthWidth; j++) {
            printf("%c ", labyrinth[i][j]);
        }
        printf("\n");
    }
}

void chooseGoalPosition() {

    labyrinth[labyrinthHeight - 1][labyrinthWidth - 1] = goal;
}

void choosePlayerPosition() {

    labyrinth[1][1] = player;
}

int cellNotVisited(int row, int column) {

    if (labyrinth[row][column] == unvisitedPath) {
        return 1;
    }

    return 0;
}

void generateMaze(int row, int column) {

    do {

        int side = randomChoice(1, 4);
        int rowThrust = 0, columnThrust = 0;

        switch (side) {
            case 1:
                rowThrust = -1;
                break;
            case 2:
                columnThrust = 1;
                break;
            case 3:
                rowThrust = 1;
                break;
            default:
                columnThrust = -1;
                break;
        }

        int neighbourCellRow = (row + (rowThrust * 2));
        int neighbourCellColumn = (column + (columnThrust * 2));
        int neighbourWallRow = (row + rowThrust);
        int neighbourWallColumn = (column + columnThrust);

        if (cellNotVisited(neighbourCellRow, neighbourCellColumn)) {

            //set wall as a path
            labyrinth[neighbourWallRow][neighbourWallColumn] = path;
            //set neighbour cell as a path
            labyrinth[neighbourCellRow][neighbourCellColumn] = path;

            generateMaze(neighbourCellRow, neighbourCellColumn);
        }

        // while not all the neighbour cells are visited
    } while (cellNotVisited(row - 2, column) || cellNotVisited(row + 2, column)
             || cellNotVisited(row, column - 2) || cellNotVisited(row, column + 2));
}

int solveMaze(int row, int column) {

    if (labyrinth[row][column] == goal) {

        return 1;
    }

    if (labyrinth[row][column] != path && labyrinth[row][column] != player) {

        return 0;
    }

    labyrinth[row][column] = possibleSolutionPath;

    if(solveMaze(row - 1, column) || solveMaze(row, column + 1)
       || solveMaze(row + 1, column) || solveMaze(row, column - 1)) {

        labyrinth[row][column] = solutionPath;
        return 1;
    }

    return 0;
}

int main() {

    //ask player to choose difficulty level from easy, medium and hard
    int difficultyLevel;
    printf("Choose difficulty level from 1 to 3: ");
    //print the possible difficulty levels
    printf("\n1 - Easy (6x6)\n2 - Medium (10x10)\n3 - Hard (10x20)\nDefault (10x10)\n");
    scanf("%d", &difficultyLevel);

    switch (difficultyLevel) {
        case 1:
            labyrinthHeight = 6;
            labyrinthWidth = 6;
            break;
        case 2:
            labyrinthHeight = 10;
            labyrinthWidth = 10;
            break;
        case 3:
            labyrinthHeight = 10;
            labyrinthWidth = 20;
            break;
        default:
            labyrinthHeight = 10;
            labyrinthWidth = 10;
            break;
    }

    for (int i = 0; i < labyrinthHeight; i++) {
        for (int j = 0; j < labyrinthWidth; j++) {
            if (isEven(i) && isEven(j)) {
                labyrinth[i][j] = unvisitedPath;
            } else {
                labyrinth[i][j] = wall;
            }
        }
    }

    //set seed for random
    static time_t t;
    srand((unsigned) time(&t));

    generateMaze(1, 1);
    chooseGoalPosition();
    choosePlayerPosition();

    printf("Here is your maze:\n\n");

    printLabyrinth();

    printNewLines(2);

    printf("Solve it.\n");

    printf("These are the symbols\n");

    printf("Your position: %c\n", player);
    printf("These are walls: %c\n", wall);
    printf("These are paths: %c\n", path);
    printf("This is the goal: %c\n", goal);

    printNewLines(3);

    int goalRow = labyrinthHeight - 1;
    int goalColumn = labyrinthWidth - 1;
    int playerRow = 1;
    int playerColumn = 1;

    do {

        printf("Enter w to move up\n");
        printf("Enter a to move left\n");
        printf("Enter s to move down\n");
        printf("Enter d to move right\n");
        printf("Enter g to show solution\n");

        printf("\n");

        printLabyrinth();

        printf("\n");

        int playerRowThrust = 0, playerColumnThrust = 0;

        char input;
        printf("Enter a key:\n");
        //the space is for scanf to ignore white spaces
        scanf(" %c", &input);
        printf("\n");

        switch (input) {
            case 'w':
                playerRowThrust = -1;
                break;
            case 'a':
                playerColumnThrust = -1;
                break;
            case 's':
                playerRowThrust = 1;
                break;
            case 'd':
                playerColumnThrust = 1;
                break;
            case 'g':
                //delete all the possible solution paths
                for (int i = 0; i < labyrinthHeight; i++) {
                    for (int j = 0; j < labyrinthWidth; j++) {
                        if (labyrinth[i][j] == solutionPath) {
                            labyrinth[i][j] = path;
                        }
                    }
                }

                solveMaze(playerRow, playerColumn);

                //delete all the possible solution paths
                for (int i = 0; i < labyrinthHeight; i++) {
                    for (int j = 0; j < labyrinthWidth; j++) {
                        if (labyrinth[i][j] == possibleSolutionPath) {
                            labyrinth[i][j] = path;
                        }
                    }
                }

                //reassign player to the position it was in the turn
                labyrinth[playerRow][playerColumn] = player;

                printf("Follow this path to get to the goal: %c\n", solutionPath);
                break;
            default:
                printf("Invalid input\n");
                continue;
                break;
        }

        //if player will hit a wall or go out of bounds
        if (labyrinth[playerRow + playerRowThrust][playerColumn + playerColumnThrust] != path
            && labyrinth[playerRow + playerRowThrust][playerColumn + playerColumnThrust] != goal
            && labyrinth[playerRow + playerRowThrust][playerColumn + playerColumnThrust] != solutionPath) {

            printf("You can not move here, it is a wall or out of bounds.\n");
            continue;
        }

        //current player position becomes a path
        labyrinth[playerRow][playerColumn] = path;

        //shift player to new position
        playerRow += playerRowThrust;
        playerColumn += playerColumnThrust;

        //assign new position to player
        labyrinth[playerRow][playerColumn] = player;

        printf("\n");

    } while (labyrinth[goalRow][goalColumn] != player);

    labyrinth[goalRow][goalColumn] = player;
    printLabyrinth();
    printf("Congrats!\n");
    printf("You won!\n");

    return 0;
}