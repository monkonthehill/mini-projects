//@monk_from_the_hill
//this is just my learning project and there might be mistakes and edge cases (like inputting numbers way bigger than normal)
//its totally handmade within 50mins
//mini projects --1;


#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

int main() {
  // machine by itself cannot generate any random number so the line below is
  // used to seed the machine so that it can generate a psudo random number
  srand(static_cast<unsigned int>(time(nullptr)));
  int random_num = 0;

  cout << "=============================================================\n";
  cout << "🎮  WELCOME TO THE ULTIMATE NUMBER GUESSING CHALLENGE! 🎮\n";
  cout << "=============================================================\n\n";

  cout << "I’m your machine. I’ve chosen a secret number between 1 and 100.\n";
  cout << "Your task? Guess it within the limited number of tries.\n";
  cout << "If you succeed, victory is yours — fail, and I win!\n\n";

  cout << "Let’s see if your mind can read mine...\n";
  cout << "-------------------------------------------------------------\n\n";

  char play_again;
  int total_win = 0;
  int total_loss = 0;

  do {
    bool has_won = false;   // monitor the user's win or loss;
    int total_attempts = 7; // limits the number of user_attemts

    cout << "\n🔹 You have " << total_attempts
         << " total guesses. Use them wisely!\n";
    cout << "-------------------------------------------------------------\n\n";

    int user_attemts = 1; // monitor the current number of user attempt
    // psudo random number is assigned to random_num
    random_num = random() % 100 + 1; // initialize the random_num

    while (total_attempts >= user_attemts) {
      int user_guess; // initialize the user input number

      cout << "👉 Attempt #" << user_attemts << ": Enter your guess → ";
      cin >> user_guess;

      user_attemts++;
      cout << endl;

      // add one as soon as user gives the number
      if (user_guess == random_num) {
        cout << "🎉 Congratulations! You guessed it right!\n";
        cout << "✅ The secret number was: " << random_num << endl;
        cout << "🕒 You took " << user_attemts - 1 << " attempt(s) to win!\n";
        has_won = true;
        total_win++;
        break;
      } else if (user_guess < random_num) {
        cout << "⬆️  TIP: Try a higher number next time.\n";
      } else if (user_guess == random_num + 10 ||
                 user_guess == random_num - 10) {
        cout << "🔥 TIP: You’re really close! Think carefully...\n";
      } else if (user_guess > 100) {
        cout << "⚠️  Please enter a number between 1 and 100.\n";
      } else {
        cout << "⬇️  TIP: Go for a smaller number.\n";
      }

      int left_attempts = total_attempts - user_attemts + 1;
      cout << "💭 Attempts remaining: " << left_attempts << endl;
      cout << "-------------------------------------------------------------\n";
    }

    if (!has_won) {
      cout << "❌ Out of attempts! The correct number was: " << random_num
           << endl;
      total_loss++;
    }

    // asking user to play again
    cout << "\nWould you like to play again? (y/n): ";
    cin >> play_again;
    cout << "\n=============================================================\n";

  } while (play_again == 'y' || play_again == 'Y');

  // Show win/loss record
  cout << "\n🏁 FINAL SCOREBOARD 🏁\n";
  cout << "-------------------------------------------------------------\n";
  cout << "🏆 Total Wins  : " << total_win << endl;
  cout << "💀 Total Losses: " << total_loss << endl;
  cout << "-------------------------------------------------------------\n";
  cout << "Thanks for playing, challenger! See you next time. 👋\n";
  cout << "=============================================================\n";

  return 0;
}
