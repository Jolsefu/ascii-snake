#include <curses.h>
#include <cstdlib>
#include <string.h>
#include <time.h>

// Area dimension size
#define ROW 17
#define COLUMN 52

// Snake properties
#define SNAKE_SIZE 5
#define INTERVAL 500

int inp;

// Struct for snake
typedef struct {
  int position[15 * 50][2];
  int step[2];
  int size;
} Snake;

Snake snake;

void setup();                         // Initializes the setup of the game
void print_area();                    // Prints the area of the game
void read_input(bool snake_movement); // Reads the input from the user
void move_snake();                    // Changes the x-y coordinates of the snake one-by-one
void handle_food();                   // Handles the food generation of the snake
void print_game_over();               // Print game over message

int in_snake_arr(int x, int y, bool include_head); // Utiliy function: Returns a boolean if current x-y is the snake

// Area array
char area[ROW][COLUMN] = {};

// Food array that stores 2 values (x-y coordinates)
int food[2];

// Status properties
bool started_moving = FALSE;
bool game_over = FALSE;
char game_over_reason[50];

int main()
{
  srand(time(NULL));

  initscr();
  noecho();
  cbreak();
  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);

  setup();

  while ((char) inp != 's')
    read_input(FALSE);

  while ((char) inp != 'q')
  {
    print_area();
    read_input(TRUE);
    move_snake();
    handle_food();

    if (game_over)
      break;

    napms(INTERVAL);
  }

  print_game_over();

  while ((char) inp != 'q')
    read_input(FALSE);

  clear();
  endwin();

  return 0;
}

void setup()
{
  printw("(WASD) movement keys. (q) to quit. (s) to start.\n");

  game_over = FALSE;

  // Initial snake size
  snake.size = SNAKE_SIZE;

  // Initial snake position
  for (int i = 0; i < snake.size; i++)
  {
    snake.position[i][0] = 1;
    snake.position[i][1] = 1;
  }

  // Setup the area for the snake

  // Top & Bottom borders
  for (int i = 0; i < COLUMN; i++)
  {
    area[0][i] = '-';
    area[ROW - 1][i] = '-';
  }

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

int in_snake_arr(int x, int y, bool include_head)
{
  int start = 1;
  if (include_head)
    start = 0;

  for (int i = start; i < snake.size; i++)
  {
    if (snake.position[i][0] == x && snake.position[i][1] == y)
        return 1;
  }

  return 0;
}

void print_area()
{
  clear();

  for (int x = 0; x < ROW; x++)
  {
    for (int y = 0; y < COLUMN; y++)
    {
      // If the head of the snake touches the border, it is game over
      if ((area[x][y] == '|' || area[x][y] == '-') && snake.position[0][0] == x && snake.position[0][1] == y)
      {
        game_over = TRUE;
        strcpy(game_over_reason, "Don't touch the borders!");
      }
      else if (in_snake_arr(x, y, TRUE))
        printw("0");
      else if (food[0] == x && food[1] == y && food[0] != 0 && food[1] != 0)
        printw("o");
      else
        printw("%c", area[x][y]);
    }
    printw("\n");
  }

  if (in_snake_arr(snake.position[0][0], snake.position[0][1], FALSE) && started_moving)
  {
    game_over = TRUE;
    strcpy(game_over_reason, "Don't bump into your own body!");
  }
}

void read_input(bool snake_movement)
{
  inp = getch();

  if (!snake_movement)
    return;

  // Prevent from moving at the opposite direction of the current direction
  if (inp == 'w' && snake.step[0] == 1)
    return;
  else if (inp == 's' && snake.step[0] == -1)
    return;
  else if (inp == 'a' && snake.step[1] == 1)
    return;
  else if (inp == 'd' && snake.step[1] == -1)
    return;

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
  if (!started_moving && snake.step[0] || snake.step[1])
    started_moving = TRUE;

  int prev[2] = {snake.position[0][0], snake.position[0][1]};

  // Move the head of the snake
  snake.position[0][0] += snake.step[0];
  snake.position[0][1] += snake.step[1];

  // Cascade the movement of the snake
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
    int x = 0, y = 0;

    while (1)
    {
      x = rand() % 15 + 1;
      y = rand() % 50 + 1;

      if (!in_snake_arr(x, y, TRUE))
        break;
    }

    food[0] = x;
    food[1] = y;
  }

  // If snake's head has the same coordinates as the food
  if (snake.position[0][0] == food[0] && snake.position[0][1] == food[1])
  {
    snake.size++;

    // If the snake has reached maximum size
    if (snake.size >= 15 * 50)
    {
      game_over = TRUE;
      strcpy(game_over_reason, "You won! Congratulations!");
    }

    food[0] = 0;
    food[1] = 0;

    // Append a single snake chunk at the tail
    snake.position[snake.size - 1][0] = snake.position[snake.size - 2][0] + -(snake.step[0]);
    snake.position[snake.size - 1][1] = snake.position[snake.size - 2][1] + -(snake.step[1]);
  }
}

void print_game_over()
{
  clear();

  printw("====================== GAME OVER =======================\n");
  printw("%s\n\n", game_over_reason);
  printw("press 'q' to quit\n");
}
