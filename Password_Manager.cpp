/*
 * ========================================================
 * Password Manager - Practice Mini Project
 * Author: monkonthehill
 * GitHub: https://github.com/monkonthehill
 *
 * NOTE:
 * This is a **practice project** for learning C++.
 * It is NOT intended to be a secure or real password manager.
 * Encryption is minimal (XOR) and should NOT be used for
 * storing real passwords.
 *
 * Purpose:
 * - Practice structs, vectors, file I/O, loops, and functions.
 * - Implement simple XOR encryption/decryption for learning.
 * - Build a beginner-friendly console menu interface.
 *
 * HOW TO USE:
 * 1. Run the program.
 * 2. Use the menu to add, display, or delete accounts.
 * 3. Passwords are encrypted using XOR for demonstration.
 * 4. Accounts are saved in "accounts.txt" when you choose Save & Exit.
 * ========================================================
 */

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// ------------------- Struct -------------------
struct accounts {
  string NameOfAccount;
  string Email;
  string password;
};

// ------------------- XOR Encryption/Decryption -------------------
string xorEncryptDecrypt(const string &input, char key = 69) {
  string output = input;
  for (size_t i = 0; i < output.size(); i++) {
    output[i] ^= key;
  }
  return output;
}

// ------------------- Display All Account Names -------------------
void displayAccountNames(const vector<accounts> &AccountList) {
  cout << "\n========== YOUR ACCOUNTS ==========\n";
  if (AccountList.empty()) {
    cout << "No accounts found. Add new accounts to see them here.\n";
    return;
  }
  for (size_t i = 0; i < AccountList.size(); i++) {
    cout << i + 1 << ". " << AccountList[i].NameOfAccount << " | ";
  }
  cout << "\n";
}

// ------------------- Add New Account -------------------
void EnterNewAccount(vector<accounts> &AccountList) {
  cin.ignore(); // clear input buffer
  accounts acc;
  cout << "\n========== ADD NEW ACCOUNT ==========\n";
  cout << "Enter name of Account: ";
  getline(cin, acc.NameOfAccount);
  cout << "Enter email address: ";
  getline(cin, acc.Email);
  cout << "Enter password: ";
  string password;
  getline(cin, password);

  acc.password = xorEncryptDecrypt(password); // store encrypted
  AccountList.push_back(acc);
  cout << "✅ Account added successfully!\n";
}

// ------------------- Display Individual Account -------------------
void DisplayIndividualAccount(const vector<accounts> &AccountList) {
  if (AccountList.empty()) {
    cout << "No accounts to show. Add one first.\n";
    return;
  }

  cout << "\nEnter the number of account to display:\n";
  for (size_t i = 0; i < AccountList.size(); i++) {
    cout << i + 1 << ". " << AccountList[i].NameOfAccount << "\n";
  }

  int n;
  cin >> n;
  cin.ignore();

  if (n < 1 || n > AccountList.size()) {
    cout << "Invalid number!\n";
    return;
  }

  const accounts &acc = AccountList[n - 1];
  string decryptedPassword = xorEncryptDecrypt(acc.password);

  cout << "\n===== ACCOUNT DETAILS =====\n";
  cout << "Name: " << acc.NameOfAccount << "\n";
  cout << "Email: " << acc.Email << "\n";
  cout << "Password: " << decryptedPassword << "\n";
}

// ------------------- Delete Account -------------------
void DeleteAccounts(vector<accounts> &AccountList) {
  if (AccountList.empty()) {
    cout << "No accounts to delete. Add one first.\n";
    return;
  }

  displayAccountNames(AccountList);
  cout << "\nEnter the number of the account to delete: ";
  int n;
  cin >> n;
  cin.ignore();

  if (n < 1 || n > AccountList.size()) {
    cout << "Invalid number!\n";
    return;
  }

  AccountList.erase(AccountList.begin() + (n - 1));
  cout << "✅ Account deleted successfully!\n";
}

// ------------------- Save/Load Accounts -------------------
void saveAccounts(const vector<accounts> &AccountList, const string &filename) {
  ofstream file(filename);
  for (const accounts &acc : AccountList) {
    file << acc.NameOfAccount << "|" << acc.Email << "|" << acc.password
         << "\n";
  }
}

void loadAccounts(vector<accounts> &AccountList, const string &filename) {
  ifstream file(filename);
  if (!file)
    return;

  AccountList.clear();
  string line;
  while (getline(file, line)) {
    size_t first = line.find('|');
    size_t second = line.find('|', first + 1);
    if (first == string::npos || second == string::npos)
      continue;

    accounts acc;
    acc.NameOfAccount = line.substr(0, first);
    acc.Email = line.substr(first + 1, second - first - 1);
    acc.password = line.substr(second + 1);
    AccountList.push_back(acc);
  }
}

// ------------------- MAIN FUNCTION -------------------
int main() {
  vector<accounts> Accounts;
  loadAccounts(Accounts, "accounts.txt");

  while (true) {
    cout << "\n===== PASSWORD MANAGER =====\n";
    cout << "1. Add new account\n";
    cout << "2. Display all accounts\n";
    cout << "3. Display individual account\n";
    cout << "4. Delete account\n";
    cout << "5. Save & Exit\n";
    cout << "Enter your choice: ";

    int choice;
    cin >> choice;
    cin.ignore();

    switch (choice) {
    case 1:
      EnterNewAccount(Accounts);
      break;
    case 2:
      displayAccountNames(Accounts);
      break;
    case 3:
      DisplayIndividualAccount(Accounts);
      break;
    case 4:
      DeleteAccounts(Accounts);
      break;
    case 5:
      saveAccounts(Accounts, "accounts.txt");
      cout << "✅ Accounts saved. Goodbye!\n";
      return 0;
    default:
      cout << "Invalid choice! Try again.\n";
    }
  }
}
