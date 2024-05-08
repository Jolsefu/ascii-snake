#include <curses.h>

#define ROW 17
#define COLUMN 52

// User input
int inp;

typedef struct {
  // Area row is 50, and the column is 50
  int position[15 * 50];
  int step;
} Snake;

Snake snake;

void setup();
void print_area();
void read_input();

char area[ROW][COLUMN] = {};

int main()
{
  initscr();
  noecho();

  setup();

  while ((char) inp != 'q')
  {

    read_input();

    halfdelay(20);

    print_area();

  }

  endwin();

  return 0;
}

void read_input()
{
  inp = getch();

  switch(inp)
  {
    case 'w':
      area[3][10] = 'u';
      snake.step = -1;
      break;
    case 's':
      area[3][10] = 'd';
      break;
    case 'a':
      area[3][10] = 'l';
      break;
    case 'd':
      area[3][10] = 'r';
      break;
  }
}

void setup()
{
  printw("Press any key to start. (WASD) movement keys.");

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

void print_area()
{
  clear();

  for (int i = 0; i < ROW; i++)
  {
    for (int j = 0; j < COLUMN; j++)
    {
      printw("%c", area[i][j]);
    }
    printw("\n");
  }
}
