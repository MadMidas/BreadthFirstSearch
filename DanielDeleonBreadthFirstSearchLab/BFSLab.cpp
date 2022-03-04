#include <cstdlib>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <queue>
#include <conio.h>
#include <deque>

using namespace std;

struct Node {
    bool isWall = false;
    Node * parent = nullptr;
    vector<Node*> cn; //connected nodes / adjacency list
    bool visited = false;
};

class Grid {
public:
    static const int SIZE = 20;
    Node * maze[SIZE][SIZE];
    Node * start = nullptr;
    Node * target = nullptr;
    deque<Node*> path;
    bool validBFS = true;

    string mazeFile =
            "x x x x x x x x x x x x x x x x x x x x "
            "x o o o x o x o o o o o o o x o x o o x "
            "x x x o o o x x x o x x x o o o x o o x "
            "x o x o x o o o x o o o x o x o o o x x "
            "x o o o x x x o o o x o o o x x x o o x "
            "x o x x x o x x x o o o x x x o x x x x "
            "x o o x x o x x x x x o o o x o x o o x "
            "x o o x x o x x x x x o o o o o o o o x "
            "x x o x x o o o o o x x o x x o o o o x "
            "x o o o o o x o x o o o o o o o x o x x "
            "x x o o x o x x x x x x o o x o x o x x "
            "x o o o x o x o o o o x o o x o x o o x "
            "x x x o o o x x x o x x x o o o x x x x "
            "x o x o x o o o x o x o x o x o o o x x "
            "x o o o x x x o o o x o x o x x x o o x "
            "x o x x x o x x x o o o x o x o x x x x "
            "x o o o x o x o o o x o o o x o x o o x "
            "x o o o o o o o o x x o o o o o o o o x "
            "x o o x x o o o o x x x o x x o o o o x "
            "x x x x x x x x x x x x x x x x x x x x ";

    Grid() {
        stringstream ss(mazeFile, ios::in);
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                char buffer;
                Node * n = new Node();
                ss >> buffer;
                if (buffer == 'x') {
                    n->isWall = true;
                }
                maze[i][j] = n;
            }
        }

        //connect the nodes horizontally and vertically
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if ((i - 1) >= 0) {
                    maze[i][j]->cn.push_back(maze[i - 1][j]);
                }
                if ((i + 1) <= SIZE - 1) {
                    maze[i][j]->cn.push_back(maze[i + 1][j]);
                }
                if ((j - 1) >= 0) {
                    maze[i][j]->cn.push_back(maze[i][j - 1]);
                }
                if ((j + 1) <= SIZE - 1) {
                    maze[i][j]->cn.push_back(maze[i][j + 1]);
                }
            }
        }
    }

    void displayMaze() {
        for (int i = 0; i < SIZE; i++) {
            cout << i << "\t";
            for (int j = 0; j < SIZE; j++) {
                if (maze[i][j]->isWall) {
                    cout << char(254) << " ";

                } else {
                    cout << "  ";
                }
            }
            cout << endl;
        }
    }

    void bfs(int startRow, int startCol, int targetRow, int targetCol) {
        try {
            if (startRow > 19 || startRow < 0 || startCol > 19 || startCol < 0 || targetRow > 19 || targetRow < 0 || targetCol > 19 || targetCol < 0)
                throw 0;
            if (startRow == targetRow && startCol == targetCol)
                throw 1;
            start = maze[startRow][startCol];
            target = maze[targetRow][targetCol];
            Node * current = start; //represents whatever node I am working with
            deque<Node *> q;
            if (current->isWall)
                throw 2;
            if (target->isWall)
                throw 3;

            q.push_back(current);
            bool out = false;
            while (!out) {
                for (int i = 0; i < current->cn.size(); i++) {
                    if (current->cn[i] == target) {
                        //found target move to path
                        //path.push_front(current->cn[i]);
                        //target
                        //current->cn[i]->parent = current;
                        //current = target;
                        out = true;
                    }
                    if (!current->cn[i]->isWall && !current->cn[i]->parent) {
                        current->cn[i]->parent = current;
                        //current->cn[i]->isVisited = true;
                        q.push_back(current->cn[i]);
                    }

                }
                q.pop_front();
                current = q.front();
            };
            current = target;
            while (1) {
                if (current == start) {
                    break;
                }
                path.push_front(current); //parent of target
                current = current->parent;
            }
        } catch (int error) {
            switch (error) {
                case 0:
                    cout << "\nInvalid Coordinates.\n" << endl;
                    validBFS = false;
                    break;
                case 1:
                    cout << "\nStart and Target coordinates cannot be the same.\n" << endl;
                    validBFS = false;
                    break;
                case 2:
                    cout << "\nStarting coordinate cannot be a wall.\n" << endl;
                    validBFS = false;
                    break;
                case 3:
                    cout << "\nTarget coordinate cannot be a wall.\n" << endl;
                    validBFS = false;
                    break;
            }          
            return;
        } catch (...) {
            cout << "Unknown Error.";
            validBFS = false;
            return;
        }
        //target parent push that onto path, change that to current parent two lines in while loop
    }

    bool onPath(Node * mazeCoord) {
        for (int i = 0; i < path.size(); i++) {
            if (mazeCoord == path[i]) {
                return true;
            }
        }
    }

    void display() {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (maze[i][j]->isWall) {
                    cout << char(254) << " ";

                } else {
                    cout << "  ";
                }
                if (maze[i][j] == start) {
                    cout << "\b\bs ";
                }

                if (onPath(maze[i][j])) {
                    cout << "\b\b. ";
                }
                if (maze[i][j] == target) {
                    cout << "\b\bt ";
                }
            }
            cout << endl;
        }
    }
};

//if(onpath(maze[i][j]))//doesnt work //check every node on path //put in display to create path
//   bool onPath(Node *){
//       //return true or false
//  }

int main(int argc, char** argv) {
    int xs, ys, xt, yt;
    while (1) {
        try {
            Grid g;
            cout << "\t0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9\n";
            g.displayMaze();
            cout << "Enter starting row: ";
            cin >> xs;
            if (xs == -1)
                break;
            cout << "Enter starting column: ";
            cin >> ys;
            cout << "Enter target row: ";
            cin >> xt;
            cout << "Enter target column: ";
            cin >> yt;

            g.bfs(ys, xs, yt, xt);
            if(g.validBFS){
               g.display(); 
            }           
            cout << "\n\nPress any key to restart:";
            getch();
        } catch (...) {
            cout << "Unknown Error\n" << endl;
        }
    }

    return 0;
}