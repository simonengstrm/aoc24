#include <iostream>
#include <optional>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>
#include <fstream>

using std::cout, std::endl, std::string;

void printGrid(std::vector<std::vector<char>> grid) {
    for (std::vector<char> row : grid) {
        for (char c : row) {
            std::cout << c;
        }
        cout << endl;
    }
    cout << endl;
}

struct Coordinate {
    int x, y;

    bool operator==(const Coordinate &other) const {
        return x == other.x && y == other.y;
    }
};

struct Box {
    Coordinate left, right;

    bool operator==(const Box &other) const {
        return left == other.left && right == other.right;
    }
};

namespace std {
    template <>
    struct hash<Coordinate> {
        size_t operator()(const Coordinate &c) const {
            auto h1 = std::hash<int>{}(c.x);
            auto h2 = std::hash<int>{}(c.y);
            return h1 ^ (h2 << 1);
        }
    };

    template <>
    struct hash<Box> {
        size_t operator()(const Box &b) const {
            auto h1 = std::hash<Coordinate>{}(b.left);
            auto h2 = std::hash<Coordinate>{}(b.right);

            return h1 ^ h2;
        }
    };
}

bool isBox(std::unordered_set<Box> &boxes, const Coordinate &c) {
    for (const Box &b : boxes) {
        if (c == b.left || c == b.right) {
            return true;
        }
    }
    return false;
}

Box getBoxFromCoord(std::unordered_set<Box> &boxes, const Coordinate &c) {
    for (const Box &b : boxes) {
        if (c == b.left || c == b.right) {
            return b;
        }
    }
    return Box{{0, 0}, {0, 0}};
}

bool isWall(std::unordered_set<Coordinate> &walls, const Coordinate &c) {
    return walls.find(c) != walls.end();
}

// Recursive function that checks what boxes are affected by a move in the y direction
// Takes a box and a direction and the set of total boxes and walls
// Returns a set of boxes that are affected by the move
std::unordered_set<Box> checkY(Box &b, int dy, std::unordered_set<Box> &boxes, std::unordered_set<Coordinate> &walls) {
    std::unordered_set<Box> affected;
    affected.insert(b);
    Coordinate nextLeft = {b.left.x, b.left.y + dy};
    Coordinate nextRight = {b.right.x, b.right.y + dy};
    if (isWall(walls, nextLeft) || isWall(walls, nextRight)) {
        return affected;
    }
    if (isBox(boxes, nextLeft)) {
        Box nextBox = getBoxFromCoord(boxes, nextLeft);
        affected.insert(nextBox);
        std::unordered_set<Box> nextAffected = checkY(nextBox, dy, boxes, walls);
        for (Box b : nextAffected) {
            affected.insert(b);
        }
    }
    if (isBox(boxes, nextRight)) {
        Box nextBox = getBoxFromCoord(boxes, nextRight);
        affected.insert(nextBox);
        std::unordered_set<Box> nextAffected = checkY(nextBox, dy, boxes, walls);
        for (Box b : nextAffected) {
            affected.insert(b);
        }
    }
    return affected;
}


void move(std::unordered_set<Box> &boxes, std::unordered_set<Coordinate> &walls, Coordinate &current, Coordinate &next) {
    // Check if next is a wall
    for (Coordinate wall : walls) {
        if (wall.x == next.x && wall.y == next.y) {
            return;
        }
    }

    // Check if next is a box
    if (isBox(boxes, next)) {
        if (next.x > current.x || next.x < current.x) { // X direction, box move is trivial
            // Find next free space in x direction
            int dx = next.x > current.x ? 1 : -1;
            std::unordered_set<Box> boxesToMove;
            Coordinate temp = {next.x, next.y};
            while (isBox(boxes, temp) || isWall(walls, temp)) {
                if (isWall(walls, temp)) {
                    return;
                }
                if (isBox(boxes, temp)) {
                    boxesToMove.insert(getBoxFromCoord(boxes, temp));
                }
                temp.x += dx;
            }

            // Update boxes positions
            for (Box b : boxesToMove) {
                boxes.erase(b);
            }
            // Iterator through boxesToMove and update their positions
            for (Box b : boxesToMove) {
                if (next.x > current.x) {
                    b.left.x++;
                    b.right.x++;
                } else {
                    b.left.x--;
                    b.right.x--;
                }
                boxes.insert(b);
            }
        } else {
            // We go y direction and hate our lives
            // Find affected boxes (give me a set)
            int dy = next.y > current.y ? 1 : -1;
            Box nextBox = getBoxFromCoord(boxes, next);
            std::unordered_set<Box> affected = checkY(nextBox, dy, boxes, walls);

            // Check if any of the affected boxes are blocked
            for (Box b : affected) {
                Coordinate nextLeft = {b.left.x, b.left.y + dy};
                Coordinate nextRight = {b.right.x, b.right.y + dy};
                if (isWall(walls, nextLeft) || isWall(walls, nextRight)) {
                    return;
                }
            }

            // Remove affected boxes from boxes set, update their positions and add them back
            for (Box b : affected) {
                boxes.erase(b);
            }

            for (Box b : affected) {
                b.left.y += dy;
                b.right.y += dy;
                boxes.insert(b);
            }
        }
    }


    // Move player
    current = next;
    return;
}

int part2(std::unordered_set<Box> &boxes, std::unordered_set<Coordinate> &walls, std::vector<char> instructions, Coordinate &current) {
    int maxx = 0;
    int maxy = 0;
    for (auto c : walls) {
        if (c.x > maxx) {
            maxx = c.x;
        }
        if (c.y > maxy) {
            maxy = c.y;
        }
    }
    // Follow instructions
    for (char c : instructions) {
        int x = current.x;
        int y = current.y;
        Coordinate next {x, y};
        switch (c) {
            case '^':
                next.y--;
                break;
            case 'v':
                next.y++;
                break;
            case '<':
                next.x--;
                break;
            case '>':
                next.x++;
                break;
        }

        move(boxes, walls, current, next);
    }

    // Calculate score
    int score = 0;
    for (const Box &b : boxes) {
        score += b.left.y * 100 + b.left.x;
    }
    cout << "Score: " << score << endl;

    return 0;
}

int main() {
    std::ifstream file("../d15/input.txt");
    std::vector<char> instructions;
    std::unordered_set<Box> boxes;
    std::unordered_set<Coordinate> walls;
    std::vector<std::vector<char>> grid;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            break;
        }
        std::vector<char> row;
        for (char c : line) {
            if (c == '.') {
                row.push_back('.'); row.push_back('.');
            } else if (c == 'O') {
                row.push_back('['); row.push_back(']');
            } else if (c == '@') {
                row.push_back('@'); row.push_back('.');
            } else if (c == '#') {
                row.push_back('#'); row.push_back('#');
            }
        }
        grid.push_back(row);
    }

    while (std::getline(file, line)) {
        for (char c : line) {
            instructions.push_back(c);
        }
    }

    // Get all boxes and walls for p2
    Coordinate current = {0, 0};
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            if (grid[i][j] == '[') {
                boxes.insert(Box{{j, i}, {j + 1, i}});
            } else if (grid[i][j] == ']') {
                boxes.insert(Box{{j - 1, i}, {j, i}});
            } else if (grid[i][j] == '#') {
                walls.insert({j, i});
            } else if (grid[i][j] == '@') {
                current = {j, i};
            }
        }
    }

    for (const Box &b : boxes) {
        cout << "Box: " << "(" << b.left.x << " " << b.left.y << ") (" << b.right.x << " " << b.right.y << ")" << endl;
    }

    part2(boxes, walls, instructions, current);

    return 0;
}