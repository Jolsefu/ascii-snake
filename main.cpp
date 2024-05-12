#include <curses.h>
#include <ncurses.h>

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

// Area array
char area[ROW][COLUMN] = {};

int main()
{
  initscr();
  noecho();
  cbreak();
  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);

  setup();

  while ((char) inp != 'q')
  {
    read_input();
    print_area();
    move_snake();

    napms(500);
  }

  clear();
  endwin();

  return 0;
}

void setup()
{
  printw("(WASD) movement keys. (q) to quit.");

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
        printw("*");
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
  // int prev[2] = {snake.position[0][0], snake.position[0][1]};

  snake.position[0][0] += snake.step[0];
  snake.position[0][1] += snake.step[1];

  printw("%d %d", snake.position[0][0], snake.position[0][1]);

  // for (int i = 1; i < snake.size; i++)
  // {
    // int subt[2] = {snake.position[i][0] - prev[0], snake.position[i][1] - prev[1]};
    // snake.position[i][0] += subt[0];
    // snake.position[i][1] += subt[1];
  // }
}

