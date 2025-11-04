# Number Guessing Game (C++)

A console-based number guessing game written in C++.  
The program randomly selects a number between 1 and 100, and the player has a limited number of attempts to guess it. The game provides tips and keeps track of wins and losses.

## Features

- Random number generation using `srand()` and `random()`.
- Limited attempts (7 per round).
- Hints for close guesses.
- Win/loss tracking with a scoreboard.
- Option to play multiple rounds.
- Polished terminal UI with separators and clear messages.

## How to Play

1. Compile the program:

```bash
g++ NumberGuessingGame.cpp -o NumberGuessingGame
Run the executable:

bash
Copy code
./NumberGuessingGame
Follow the on-screen prompts to guess the number.

After each round, you can choose to play again.

At the end, your total wins and losses are displayed.

Example :--
🎮  WELCOME TO THE ULTIMATE NUMBER GUESSING CHALLENGE! 🎮
I’m your machine. I’ve chosen a secret number between 1 and 100.
Your task? Guess it within the limited number of tries.
...
Requirements
C++11 or higher

Any terminal that supports standard I/O (Linux, Windows, macOS)

License
MIT License

Enjoy testing your guessing skills and refining your intuition!
