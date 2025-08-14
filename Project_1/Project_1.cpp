#include <iostream>
#include <fstream>
#include <windows.h> 
#include <cstdlib>  

using namespace std;

// Определение размеров поля
const int MAX_ROWS = 100; 
const int MAX_COLS = 100; 


// Инициализация поля 
class Universe {
public:
    Universe(int rows, int cols) : rows(rows), cols(cols) {
       
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                grid[i][j] = false; 
            }
        }
    }

    // Установка клетки как "живой"
    void setAlive(int row, int col) {
        if (row >= 0 && row < rows && col >= 0 && col < cols) {
            grid[row][col] = true; 
        }
    }

    // Метод, отображающий текущее состояние поля
    void display(int generation) {
        system("cls"); 
        cout << "Generation: " << generation << endl;
        int aliveCount = 0;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                cout << (grid[i][j] ? '*' : '-') << ' ';
                if (grid[i][j]) aliveCount++;
            }
            cout << endl;
        }
        cout << "Alive cells: " << aliveCount << endl;
    }

    // Метод для обновления состояния клеток
    bool update() {
        bool newGrid[MAX_ROWS][MAX_COLS] = { false }; 
        bool changed = false;

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                int aliveNeighbors = countAliveNeighbors(i, j);
                if (grid[i][j]) {
                    
                    if (aliveNeighbors < 2 || aliveNeighbors > 3) {
                        newGrid[i][j] = false; 
                        changed = true;
                    }
                    else {
                        newGrid[i][j] = true; 
                    }
                }
                else {
                   
                    if (aliveNeighbors == 3) {
                        newGrid[i][j] = true; 
                        changed = true;
                    }
                }
            }
        }

        // Копия нового состояния в текущее и обновление текущего состояния
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                grid[i][j] = newGrid[i][j];
            }
        }

        return changed;
    }

    // Проверка, все ли клетки мертвы
    bool allCellsDead() {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (grid[i][j]) return false;
            }
        }
        return true; 
    }

private:
    int rows, cols;
    bool grid[MAX_ROWS][MAX_COLS]; // Статический массив для хранения состояния клеток

    // Метод, подсчитывающий живых соседей
    int countAliveNeighbors(int row, int col) {
        int count = 0;
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                if (i == 0 && j == 0) continue; 
                int newRow = row + i;
                int newCol = col + j;
                if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols) {
                    if (grid[newRow][newCol]) count++;
                }
            }
        }
        return count;
    }
};

int main() {
    ifstream inputFile("universe.txt");
    if (!inputFile) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    int rows, cols;
    inputFile >> rows >> cols;

    Universe universe(rows, cols); // Создание объекта класса Universe
    int rowAlive, colAlive; // Переменные - хранят координаты живых клеток
    bool hasAliveCells = false;

    // Цикл считывания координат живых клеток из файла
    while (inputFile >> rowAlive >> colAlive) {
        universe.setAlive(rowAlive, colAlive);
        hasAliveCells = true; 
    }
    inputFile.close();

    int generation = 1;

    if (!hasAliveCells) {
        universe.display(generation);
        cout << "All cells are dead. Game over." << endl;
        return 0;
    }

    while (true) {
        universe.display(generation); // Отображение текущего состояния
        if (!universe.update()) { // Обновление состояния клеток
            cout << "Game over: No changes in the last generation." << endl;
            break;
        }
        generation++;
        Sleep(1000); 
    }

    universe.display(generation); // Отображение последнего состояния
    return 0;
}

