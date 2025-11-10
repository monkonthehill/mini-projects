#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#else
#include <cstdlib>
#endif

using namespace std;

struct Meta_data {
  string Task_name;
  string Task_discription;
  bool is_completed;
};

void clearScreen() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

void display_tasks(const vector<Meta_data> &tasks) {
  clearScreen();
  cout << "========== YOUR TASKS ==========\n\n";
  if (tasks.empty()) {
    cout << "No tasks found. Add one to get started!\n\n";
    return;
  }
  for (size_t i = 0; i < tasks.size(); i++) {
    cout << i + 1 << ". " << (tasks[i].is_completed ? "[X] " : "[ ] ")
         << tasks[i].Task_name << "\n"
         << "   " << tasks[i].Task_discription << "\n\n";
  }
}

void addTask(vector<Meta_data> &tasks) {
  clearScreen();
  Meta_data t;
  cin.ignore();
  cout << "========== ADD NEW TASK ==========\n";
  cout << "Enter task name: ";
  getline(cin, t.Task_name);
  cout << "Enter task description: ";
  getline(cin, t.Task_discription);
  t.is_completed = false;
  tasks.push_back(t);
  cout << "\n✅ Task added successfully!\n\n";
}

void markTaskDone(vector<Meta_data> &tasks) {
  clearScreen();
  display_tasks(tasks);
  if (tasks.empty())
    return;

  int n;
  cout << "Enter task number to mark as done: ";
  cin >> n;
  if (n >= 1 && n <= (int)tasks.size()) {
    tasks[n - 1].is_completed = true;
    cout << "\n✅ Task marked as done!\n\n";
  } else {
    cout << "\n❌ Invalid task number.\n\n";
  }
}

void deleteTask(vector<Meta_data> &tasks) {
  clearScreen();
  display_tasks(tasks);
  if (tasks.empty())
    return;

  int n;
  cout << "Enter task number to delete: ";
  cin >> n;
  if (n >= 1 && n <= (int)tasks.size()) {
    tasks.erase(tasks.begin() + (n - 1));
    cout << "\n🗑️ Task deleted successfully!\n\n";
  } else {
    cout << "\n❌ Invalid task number.\n\n";
  }
}

void saveTasks(const vector<Meta_data> &tasks, const string &filename) {
  ofstream file(filename);
  for (const Meta_data &t : tasks) {
    file << t.is_completed << "|" << t.Task_name << "|" << t.Task_discription
         << "\n";
  }
}

void loadTasks(vector<Meta_data> &tasks, const string &filename) {
  ifstream file(filename);
  if (!file)
    return;
  tasks.clear();
  string line;
  while (getline(file, line)) {
    Meta_data t;
    size_t first = line.find('|');
    size_t second = line.find('|', first + 1);
    if (first == string::npos || second == string::npos)
      continue;

    t.is_completed = (line[0] == '1');
    t.Task_name = line.substr(first + 1, second - first - 1);
    t.Task_discription = line.substr(second + 1);
    tasks.push_back(t);
  }
}

int main() {
  vector<Meta_data> tasks;
  loadTasks(tasks, "tasks.txt");

  while (true) {
    clearScreen();
    cout << "========== TODO CLI TOOL ==========\n";
    cout << "1. View tasks\n";
    cout << "2. Add task\n";
    cout << "3. Mark task as done\n";
    cout << "4. Delete task\n";
    cout << "5. Clear screen\n";
    cout << "6. Exit\n";
    cout << "===================================\n";
    cout << "Enter your choice: ";

    int choice;
    cin >> choice;

    switch (choice) {
    case 1:
      display_tasks(tasks);
      break;
    case 2:
      addTask(tasks);
      break;
    case 3:
      markTaskDone(tasks);
      break;
    case 4:
      deleteTask(tasks);
      break;
    case 5:
      clearScreen();
      cout << "Screen cleared.\n";
      break;
    case 6:
      saveTasks(tasks, "tasks.txt");
      cout << "\n💾 Tasks saved. Goodbye!\n";
      return 0;
    default:
      cout << "\n❌ Invalid choice. Try again.\n";
    }

    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
  }
}
