#include <cstdlib>
#include <ctime>
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;

// ANSI color codes for Linux/macOS
string green = "\033[1;32m";
string red = "\033[1;31m";
string yellow = "\033[1;33m";
string cyan = "\033[1;36m";
string reset = "\033[0m";

void printGrid(char grid[5][5], int treasures) {
  cout << cyan << "\n=== TREASURE HUNT ===\n" << reset;
  cout << "Treasures found: " << green << treasures << reset << "/4\n\n";

  cout << yellow << "   0  1  2  3  4\n" << reset;
  cout << yellow << "  +----------------+\n" << reset;

  for (int i = 0; i < 5; i++) {
    cout << yellow << i << " | " << reset;
    for (int j = 0; j < 5; j++) {
      cout << grid[i][j] << "  ";
    }
    cout << "|\n";
  }

  cout << yellow << "  +----------------+\n" << reset;
  cout << "\nUse " << cyan << "W/A/S/D" << reset
       << " to move, then press Enter.\n";
}

int main() {
#ifdef _WIN32
  // Disable color if Windows console doesn’t support it
  green = red = yellow = cyan = reset = "";
#endif

  char hidden[5][5];
  char grid[5][5];
  srand(time(0));

  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      hidden[i][j] = '.';
      grid[i][j] = '.';
    }
  }

  // treasures
  for (int i = 0; i < 4; i++) {
    int x, y;
    do {
      x = rand() % 5;
      y = rand() % 5;
    } while (hidden[x][y] != '.');
    hidden[x][y] = 'T';
  }

  // pits
  for (int i = 0; i < 4; i++) {
    int x, y;
    do {
      x = rand() % 5;
      y = rand() % 5;
    } while (hidden[x][y] != '.');
    hidden[x][y] = 'X';
  }

  int px = 0, py = 0, treasures = 0;
  grid[px][py] = 'P';

  while (true) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    printGrid(grid, treasures);

    char move;
    cin >> move;

    grid[px][py] = '.';

    if (move == 'w' && px > 0)
      px--;
    else if (move == 's' && px < 4)
      px++;
    else if (move == 'a' && py > 0)
      py--;
    else if (move == 'd' && py < 4)
      py++;
    else {
      cout << red << "Invalid move! Try again.\n" << reset;
      continue;
    }

    if (hidden[px][py] == 'T') {
      cout << green << "You found a treasure!\n" << reset;
      treasures++;
      hidden[px][py] = '.';
    } else if (hidden[px][py] == 'X') {
      cout << red << "You fell into a pit! Game over.\n" << reset;
      break;
    }

    grid[px][py] = 'P';

    if (treasures == 4) {
      cout << green << "You found all the treasures! You win!\n" << reset;
      break;
    }
  }

  cout << yellow << "\nFinal Map:\n" << reset;
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      char c = hidden[i][j];
      if (c == 'T')
        cout << green << "T " << reset;
      else if (c == 'X')
        cout << red << "X " << reset;
      else
        cout << ". ";
    }
    cout << endl;
  }

  cout << cyan << "\nThanks for playing!\n" << reset;
  return 0;
}
