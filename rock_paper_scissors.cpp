#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <thread>

using namespace std;

int main() {
  srand(static_cast<unsigned int>(time(nullptr)));
  int computer_choice = 0;
  bool dowin = false;
  int rock = 0;
  int sessiors = 1;
  int paper = 2;
  int wins = 0;
  int losses = 0;
  char play_again;

  do {
    int rounds = 5;
    int round_played = 0;
    int players_choice = 0;

    cout << "\n=============================================\n";
    cout << "🎮  ROCK - PAPER - SCISSORS: BEST OF FIVE  🎮\n";
    cout << "=============================================\n\n";

    while (round_played < rounds) {
      cout << "\n-------------------------------------------------------------"
              "\n";
      cout << "Round " << (round_played + 1) << " / " << rounds << endl;
      cout << "-------------------------------------------------------------\n";
      cout << "Enter your choice:\n";
      cout << "🪨  0 → Rock\n✂️  1 → Scissors\n📄  2 → Paper\n";
      cout << "-------------------------------------------------------------\n";
      cout << "👉 Your choice: ";
      cin >> players_choice;

      round_played++;
      computer_choice = rand() % 3;

      cout << "\nStarting in ";
      for (int i = 1; i <= 3; i++) {
        cout << i << "...";
        cout.flush();
        this_thread::sleep_for(chrono::seconds(1));
      }
      cout << "\n\n";

      string choice;
      string comp_choice;
      if (players_choice == 0) {
        choice = "🪨 Rock";
      } else if (players_choice == 1) {
        choice = "✂️ Scissors";
      } else {
        choice = "📄 Paper";
      }

      if (computer_choice == 0) {
        comp_choice = "🪨 Rock";
      } else if (computer_choice == 1) {
        comp_choice = "✂️ Scissors";
      } else {
        comp_choice = "📄 Paper";
      }

      cout << "🧠 You chose: " << choice << endl;
      cout << "💻 Computer chose: " << comp_choice << endl;

      cout << "-------------------------------------------------------------\n";

      if (players_choice == computer_choice) {
        cout << "🤝 It's a draw! We both chose the same thing.\n";
      } else if ((players_choice == 0 && computer_choice == 1) ||
                 (players_choice == 1 && computer_choice == 2) ||
                 (players_choice == 2 && computer_choice == 0)) {
        cout << "🎉 You won! Nice job!\n";
        dowin = true;
        wins++;
      } else {
        cout << "💀 You lost this round.\n";
        losses++;
      }

      cout << "🏆 Total Wins: " << wins << "   💔 Total Losses: " << losses
           << endl;
      cout << "-------------------------------------------------------------\n";
      cout << "\nPress Enter to continue to the next round...";
      cin.ignore();
      cin.get();
    }

    if (wins >= 3) {
      cout << "\n🥇 You won the best of five! Champion energy!\n";
      break;
    } else {
      dowin = false;
    }

    if (!dowin) {
      cout << "\n🔥 That was a great game! Want to play another best of 5? "
              "(y/n): ";
      cin >> play_again;
    }

  } while (play_again == 'y' || play_again == 'Y');

  cout << "\n=============================================\n";
  cout << "🏁 Final Scoreboard 🏁\n";
  cout << "Total Wins: " << wins << "\n";
  cout << "Total Losses: " << losses << "\n";
  cout << "Thanks for playing! 👋\n";
  cout << "=============================================\n";

  return 0;
}
