#include <curses.h>
#include <cstdlib>

// Area dimension size
#define ROW 17
#define COLUMN 52

int inp;

// Struct for snake
typedef struct {
  int position[15 * 50][2];
  int step[2];
  int size;
} Snake;

Snake snake;

void setup();       // Initializes the setup of the game
void print_area();  // Prints the area of the game
void read_input();  // Reads the input from the user
void move_snake();  // Changes the x-y coordinates of the snake one-by-one
void handle_food();  // Handles the food generation of the snake

// Area array
char area[ROW][COLUMN] = {};

// Food array that stores 2 values (x-y coordinates)
int food[2] = {3, 3};

int game_over;

int main()
{
  initscr();
  noecho();
  cbreak();
  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);

  setup();

  while ((char) inp != 'q' || game_over)
  {
    print_area();
    for (int i = 0; i < snake.size; i++)
      printw("%d %d\n", snake.position[i][0], snake.position[i][1]);
    read_input();
    move_snake();
    handle_food();

    napms(500);
  }

  clear();
  endwin();

  return 0;
}

void setup()
{
  printw("(WASD) movement keys. (q) to quit.");

  game_over = 0;

  // Initial snake position
  snake.position[0][0] = 1;
  snake.position[0][1] = 1;

  // Initial snake size
  snake.size = 1;

  // Setup the area for the snake

  // Top & Bottom borders
  for (int i = 0; i < COLUMN; i++)
    area[0][i] = '-';
  for (int i = 0; i < COLUMN; i++)
    area[ROW - 1][i] = '-';

  // Between the area borders
  for (int i = 1; i < ROW - 1; i++)
  {
    for (int j = 0; j < COLUMN; j++)
    {
      if (j == 0 || j == COLUMN - 1)
        area[i][j] = '|';
      else
         area[i][j] = ' ';
    }
  }
}

int in_arr(int x, int y)
{
  for (int i = 0; i < snake.size; i++)
  {
    if (snake.position[i][0] == x && snake.position[i][1] == y)
        return 1;
  }

  return 0;
}

void print_area()
{
  clear();

  for (int i = 0; i < ROW; i++)
  {
    for (int j = 0; j < COLUMN; j++)
    {
      if (in_arr(i, j))
        printw("0");
      else if (food[0] == i && food[1] == j)
        printw("o");
      else
        printw("%c", area[i][j]);
    }
    printw("\n");
  }
}

void read_input()
{
  inp = getch();

  switch(inp)
  {
    // Up
    case 'w':
      snake.step[0] = -1;
      snake.step[1] = 0;
      break;
    // Down
    case 's':
      snake.step[0] = 1;
      snake.step[1] = 0;
      break;
    // Right
    case 'd':
      snake.step[0] = 0;
      snake.step[1] = 1;
      break;
    // Left
    case 'a':
      snake.step[0] = 0;
      snake.step[1] = -1;
      break;
    }
}

void move_snake()
{
  int prev[2] = {snake.position[0][0], snake.position[0][1]};

  snake.position[0][0] += snake.step[0];
  snake.position[0][1] += snake.step[1];

  for (int i = 1; i < snake.size; i++)
  {
    int tmp[2] = {snake.position[i][0], snake.position[i][1]};

    snake.position[i][0] = prev[0];
    snake.position[i][1] = prev[1];

    prev[0] = tmp[0];
    prev[1] = tmp[1];
  }
}

void handle_food()
{
  // If food still doesn't have a value for the coordinates
  // Generate food
  if (!(food[0]) && !(food[1]))
  {
    int x = rand() % 15 + 1;
    int y = rand() % 50 + 1;

    food[0] = x;
    food[1] = y;
  }

  if (snake.position[0][0] == food[0] && snake.position[0][1] == food[1])
  {
    snake.size++;

    food[0] = 0;
    food[1] = 0;

    snake.position[snake.size - 1][0] = snake.position[snake.size - 2][0] + -(snake.step[0]);
    snake.position[snake.size - 1][1] = snake.position[snake.size - 2][1] + -(snake.step[1]);
  }
}
