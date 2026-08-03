# 🦠 Conway's Game of Life (Pixel-Perfect Engine)

A high-density, real-time implementation of John Conway's **Game of Life** built in C++ using the **Raylib** graphics framework. 

This specific version maps the simulation grid directly to a **1000x1000 pixel landscape**, where every single individual pixel acts as a living or dead cell governed by a 2D boolean vector.

---

## 📜 The Rules of Life

The universe of the Game of Life is an infinite, two-dimensional orthogonal grid of square cells. Every cell interacts with its eight neighbors (**Moore neighborhood**), which are adjacent horizontally, vertically, or diagonally. 

At each tick of the clock, the following state transitions occur simultaneously:
1. **Underpopulation:** Any live cell with fewer than **two** live neighbors dies.
2. **Survival:** Any live cell with **two or three** live neighbors lives on to the next generation.
3. **Overpopulation:** Any live cell with more than **three** live neighbors dies.
4. **Reproduction:** Any dead cell with exactly **three** live neighbors becomes a live cell.

---

## 🖥️ Technical Architecture

* **Resolution:** 1000px × 1000px window canvas.
* **Grid Resolution:** 1,000,000 unique simulation modules (1000 columns × 1000 rows).
* **Data Structure:** High-performance, double-buffered tracking using two-dimensional C++ STL vectors (`std::vector<std::vector<bool>>`).
* **Frame Optimization:** Double buffering ensures that updates are calculated using snapshots of the current generation before swapping state pointers, preventing race conditions or localized calculation bias.

---

## 🛠️ Multi-Platform Compilation

This project includes an automated cross-platform `Makefile` that auto-detects whether you are running on **Linux** or **macOS** (both Apple Silicon and Intel) and seamlessly applies the necessary compiler arguments.

### Prerequisites

Ensure you have a C++ compiler and the Raylib development library installed on your system.

#### For Linux (Ubuntu/Debian/Asahi):
```bash
sudo apt update
sudo apt install build-essential g++ libraylib-dev
```

#### For macOS (via Homebrew):
```bash
brew install raylib
```

### Build Commands

Open your terminal in the project directory and run:

```bash
# Compile the project
make

# Compile and launch the simulation immediately
make run

# Clean up binaries and temporary output files
make clean
```

---

## 🎮 How to Control

* **[SPACEBAR]** - Toggle Pause / Play simulation.
* **[LEFT MOUSE CLICK]** - Draw alive cells onto the 1000x1000 pixel canvas (Can be dragged or tapped while paused).
