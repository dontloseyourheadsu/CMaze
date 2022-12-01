#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static char labyrinth[31][31];
static int labyrinthHeight;
static int labyrinthWidth;
static int score = 0;
static char goal = '#';
static char player = 'o';
static char wall = '-';
static char unvisitedPath = '=';
static char path = ' ';
static char solutionPath = '*';
static char possibleSolutionPath = '+';
static char fog = '\176';
static char coin = '$';

void printNewLines(int amountOfLines) {
    for (int i = 0; i < amountOfLines; i++) {
        printf("\n");
    }
}

int randomChoice(int base, int limit) {

    int randomChoice = (rand() % limit) + base;

    return randomChoice;
}

void printLabyrinth() {

    for (int i = 0; i < labyrinthHeight; i++) {
        for (int j = 0; j < labyrinthWidth; j++) {
            printf("%c ", labyrinth[i][j]);
        }
        printf("\n");
    }
}

void printLabyrinthWithFog(int playerRow, int playerColumn) {

    for (int i = 0; i < labyrinthHeight; i++) {
        for (int j = 0; j < labyrinthWidth; j++) {
            //Print surrounding area of the player by 1, else is fog
            if (((i >= (playerRow - 1)) && (i <= playerRow + 1))
                && ((j >= (playerColumn - 1)) && (j <= (playerColumn + 1)))) {
                printf("%c ", labyrinth[i][j]);
            } else {
                printf("%c ", fog);
            }
        }
        printf("\n");
    }
}

int isEven(int number) { return number % 2; }

void createLabyrinth() {

    for (int i = 0; i < labyrinthHeight; i++) {
        for (int j = 0; j < labyrinthWidth; j++) {
            if (isEven(i) && isEven(j)) {
                labyrinth[i][j] = unvisitedPath;
            } else {
                labyrinth[i][j] = wall;
            }
        }
    }
}

void chooseGoalPosition() {

    labyrinth[labyrinthHeight - 2][labyrinthWidth - 2] = goal;
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

    if (solveMaze(row - 1, column) || solveMaze(row, column + 1)
        || solveMaze(row + 1, column) || solveMaze(row, column - 1)) {

        labyrinth[row][column] = solutionPath;
        return 1;
    }

    return 0;
}

void chooseCoinsPosition(int amountOfCoins) {

    for (int i = 0; i < amountOfCoins; i++) {
        int row, column;

        do {
            row = randomChoice(1, labyrinthHeight - 1);
            column = randomChoice(1, labyrinthWidth - 1);
        } while (labyrinth[row][column] != path);

        labyrinth[row][column] = coin;
    }
}

int validateLabyrinthSize(int width, int height) {
    //too small
    if(width < 5 || height < 5) {
        return 0;
    }

    //should be odd
    if(width % 2 == 0 || height % 2 == 0) {
        return 0;
    }

    //cant be bigger than 31
    if(width > 31 || height > 31) {
        return 0;
    }

    return 1;
}

int main() {

    //ask player to choose difficulty level from easy, medium and hard
    int difficultyLevel;
    printf("Choose difficulty level from 1 to 3: ");
    //print the possible difficulty levels
    printf("\n1 - Easy (9x9)\n2 - Medium (19x19)\n3 - Hard (29x29)\n4 - Choose Size (?x?)\nOther - Fog (9x9)\n");
    scanf("%d", &difficultyLevel);
    int useFog = 0;

    switch (difficultyLevel) {
        case 1:
            labyrinthHeight = 11;
            labyrinthWidth = 11;
            break;
        case 2:
            labyrinthHeight = 21;
            labyrinthWidth = 21;
            break;
        case 3:
            labyrinthHeight = 31;
            labyrinthWidth = 31;
            break;
        case 4:
            printf("Choose labyrinth size (odd number between 5 and 31): ");
            scanf("%d %d", &labyrinthWidth, &labyrinthHeight);
            if(!validateLabyrinthSize(labyrinthWidth, labyrinthHeight)) {
                printf("Invalid size!\n");
                return 0;
            }
            break;
        default:
            labyrinthHeight = 11;
            labyrinthWidth = 11;
            useFog = 1;
            break;
    }

    createLabyrinth();

    //set seed for random
    static time_t t;
    srand((unsigned) time(&t));

    generateMaze(1, 1);
    chooseGoalPosition();
    choosePlayerPosition();

    switch (difficultyLevel) {
        case 1:
            chooseCoinsPosition(5);
            break;
        case 2:
            chooseCoinsPosition(10);
            break;
        case 3:
            chooseCoinsPosition(15);
            break;
        default:
            chooseCoinsPosition(5);
            break;
    }

    printf("Here is your maze:\n\n");

    if (useFog) {
        printLabyrinthWithFog(1, 1);
    } else {
        printLabyrinth();
    }

    printNewLines(2);

    printf("Solve it.\n");

    printf("These are the symbols\n");

    printf("Your position: %c\n", player);
    printf("These are walls: %c\n", wall);
    printf("These are paths: %c\n", path);
    printf("This is the goal: %c\n", goal);
    printf("These are coins: %c\n", coin);

    printNewLines(3);

    int goalRow = labyrinthHeight - 2;
    int goalColumn = labyrinthWidth - 2;
    int playerRow = 1;
    int playerColumn = 1;

    //bool to know if player asked the maze to be solved
    int playerGotHelp = 0;

    do {

        printf("Enter w to move up\n");
        printf("Enter a to move left\n");
        printf("Enter s to move down\n");
        printf("Enter d to move right\n");
        printf("Enter g to show solution\n");

        printf("\n");

        if (useFog) {
            printLabyrinthWithFog(playerRow, playerColumn);
        } else {
            printLabyrinth();
        }

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
                //delete all the solution paths and coins
                for (int i = 0; i < labyrinthHeight; i++) {
                    for (int j = 0; j < labyrinthWidth; j++) {
                        if (labyrinth[i][j] == solutionPath || labyrinth[i][j] == coin) {
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
                playerGotHelp = 1;
                break;
            default:
                printf("Invalid input\n");
                continue;
                break;
        }

        //if player will hit a wall or go out of bounds
        if (labyrinth[playerRow + playerRowThrust][playerColumn + playerColumnThrust] != path
            && labyrinth[playerRow + playerRowThrust][playerColumn + playerColumnThrust] != goal
            && labyrinth[playerRow + playerRowThrust][playerColumn + playerColumnThrust] != solutionPath
            && labyrinth[playerRow + playerRowThrust][playerColumn + playerColumnThrust] != coin) {

            printf("You can not move here, it is a wall or out of bounds.\n");
            continue;
        }

        //add to score if player got a coin
        if (labyrinth[playerRow + playerRowThrust][playerColumn + playerColumnThrust] == coin) {
            printf("You got a coin!\n");
            score += 50;
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


    if (playerGotHelp) {
        score += 100;
    } else {
        score += 200;
    }

    labyrinth[goalRow][goalColumn] = player;
    printLabyrinth();
    printf("Congrats!\n");
    printf("You won!\n");
    printf("Your score is: %d\n", score);

    FILE *scoresDataBase;

    scoresDataBase = fopen("scores.txt", "r+");

    if (scoresDataBase == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    int maxScore;
    fscanf(scoresDataBase, "%d", &maxScore);
    fclose(scoresDataBase);


    if (score > maxScore) {
        scoresDataBase = fopen("scores.txt", "w");
        printf("You got the new high score!\n");
        fprintf(scoresDataBase, "%d", score);
        fclose(scoresDataBase);
        maxScore = score;
    }

    printf("The high score is: %d\n", maxScore);

    return 0;
}