#include <iostream>
#include <vector>
#include <stack>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;
//celda del laberinto
struct Cell {
    bool visited = false; 
    bool top = true, bottom = true, left = true, right = true; //cada celda tiene 4 paredes y un booleano para saber si fue visitada
};

class Maze {
    int width, height;
    vector<vector<Cell>> grid;

    void divide(int x, int y, int w, int h) {
        if (w < 2 || h < 2) return;

        bool horizontal;
        if (w < h) {
            horizontal = true;
        } else if (w > h) {
            horizontal = false;
        } else {
            horizontal = rand() % 5 != 0; // 3 de 4 veces horizontal
        }
        if (horizontal) {
            if (h <= 2) return;
            int wallY = y + 1 + rand() % (h - 2);
            int numPassages = std::max(w / 2, 2); // muchos huecos
            vector<int> passages;
            while ((int)passages.size() < numPassages) {
                int px = x + rand() % w;
                if (find(passages.begin(), passages.end(), px) == passages.end())
                    passages.push_back(px);
            }
            for (int i = x; i < x + w; ++i) {
                if (find(passages.begin(), passages.end(), i) != passages.end()) continue;
                grid[wallY][i].top = false;
                grid[wallY - 1][i].bottom = false;
            }
            divide(x, y, w, wallY - y);
            divide(x, wallY, w, y + h - wallY);
        } else {
            if (w <= 2) return;
            int wallX = x + 1 + rand() % (w - 2);
            int numPassages = std::max(h / 2, 2); // muchos huecos
            vector<int> passages;
            while ((int)passages.size() < numPassages) {
                int py = y + rand() % h;
                if (find(passages.begin(), passages.end(), py) == passages.end())
                    passages.push_back(py);
            }
            for (int i = y; i < y + h; ++i) {
                if (find(passages.begin(), passages.end(), i) != passages.end()) continue;
                grid[i][wallX].left = false;
                grid[i][wallX - 1].right = false;
            }
            divide(x, y, wallX - x, h);
            divide(wallX, y, x + w - wallX, h);
        }
    }

public:
    Maze(int w, int h) : width(w), height(h), grid(h, vector<Cell>(w)) {}

    void generate(int startX = 0, int startY = 0) {
        stack<pair<int, int>> cellStack;
        cellStack.push({startY, startX}); // Comienza desde la celda inicial 
        grid[startY][startX].visited = true;

        while (!cellStack.empty()) { //revisa si la pila no está vacía
            int y = cellStack.top().first;
            int x = cellStack.top().second;

            vector<pair<int, int>> neighbors; 
            if (y > 0 && !grid[y-1][x].visited) neighbors.push_back({y-1, x});
            if (y < height-1 && !grid[y+1][x].visited) neighbors.push_back({y+1, x});
            if (x > 0 && !grid[y][x-1].visited) neighbors.push_back({y, x-1});
            if (x < width-1 && !grid[y][x+1].visited) neighbors.push_back({y, x+1});

            if (!neighbors.empty()) { // Si hay vecinos no visitados
                // Elige un vecino aleatorio
                auto [ny, nx] = neighbors[rand() % neighbors.size()];
                if (ny == y-1) { grid[y][x].top = false; grid[ny][nx].bottom = false; }
                if (ny == y+1) { grid[y][x].bottom = false; grid[ny][nx].top = false; }
                if (nx == x-1) { grid[y][x].left = false; grid[ny][nx].right = false; }
                if (nx == x+1) { grid[y][x].right = false; grid[ny][nx].left = false; }
                grid[ny][nx].visited = true;
                cellStack.push({ny, nx});
            } else {
                cellStack.pop();
            }
        }
    }

    void generateRecursiveDivision() {
        for (int y = 0; y < height; ++y)
            for (int x = 0; x < width; ++x)
                grid[y][x] = Cell();
        divide(0, 0, width, height);
        abrirMasCaminos(width * height / 3); // o incluso más
    }

    void abrirMasCaminos(int cantidad) {
        for (int i = 0; i < cantidad; ++i) {
            int y = 1 + rand() % (height - 2);
            int x = 1 + rand() % (width - 2);
            int dir = rand() % 4;
            if (dir == 0 && y > 0) {
                grid[y][x].top = false;
                grid[y-1][x].bottom = false;
            } else if (dir == 1 && y < height-1) {
                grid[y][x].bottom = false;
                grid[y+1][x].top = false;
            } else if (dir == 2 && x > 0) {
                grid[y][x].left = false;
                grid[y][x-1].right = false;
            } else if (dir == 3 && x < width-1) {
                grid[y][x].right = false;
                grid[y][x+1].left = false;
            }
        }
    }

    void print() {
        for (int i = 0; i < width; ++i) cout << " _";
        cout << endl;
        for (int y = 0; y < height; ++y) {
            cout << "|";
            for (int x = 0; x < width; ++x) {
                cout << (grid[y][x].bottom ? "_" : " ");
                cout << (grid[y][x].right ? "|" : " ");
            }
            cout << endl;
        }
    }
};

int main() { //Genera laberintos con dos algoritmos diferentes
    cout << "Generando laberintos..." << endl;
    srand(time(0)); 

    Maze m(15, 10);

    cout << "DFS (Backtracking):" << endl;
    m.generate();
    m.abrirMasCaminos(30); // Abre paredes extra
    m.print();

    cout << "\nDivision Recursiva:" << endl;
    Maze m2(15, 10);
    m2.generateRecursiveDivision();
    m2.print();

    return 0;
}

