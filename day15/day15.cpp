#include <bits/stdc++.h>

using namespace std;

ifstream fin("input.txt");
// ofstream fout("output.txt");
#define cin fin
// #define cout fout

// Try to move whatever is at (r1, c1) to (r2, c2)
// true if move was a success, otherwise false
bool try_move(vector<string> &mat, int r1, int c1, int r2, int c2, bool actually_move) {
    // Physics aside, moving nothing is perfectly fine, no matter where we are moving it.
    if (mat[r1][c1] == '.') {
        return true;
    }

    // Cannot move a wall.
    if (mat[r1][c1] == '#') {
        return false;
    }

    int dirR = r2 - r1, dirC = c2 - c1;
    int r3 = r2 + dirR, c3 = c2 + dirC;

    bool can_move = false;

    // Check if we are moving vertically and moving a box
    if (dirR != 0 && (mat[r2][c2] == '['  || mat[r2][c2] == ']')) {
        // There are two pieces of the box: find the other one.
        int other_r2 = r2, other_c2;
        if (mat[r2][c2] == '[') {
            other_c2 = c2 + 1;
        } else {
            other_c2 = c2 - 1;
        }
        
        // Figure out where that other piece needs to end up.
        int other_r3 = other_r2 + dirR, other_c3 = other_c2 + dirC;

        // Make sure both pieces can move.
        can_move = try_move(mat, r2, c2, r3, c3, actually_move) && try_move(mat, other_r2, other_c2, other_r3, other_c3, actually_move);
    }
    // Otherwise, it's a simple move operation, even if we are pushing a box horizontally (because the box parts cannot end up detached).
    else {
        can_move = try_move(mat, r2, c2, r3, c3, actually_move);
    }

    // Move and return the result.
    if (can_move && actually_move) {
        mat[r2][c2] = mat[r1][c1];
        mat[r1][c1] = '.';
    }
    return can_move;
}

// Returns the result for a given matrix
// Note that we do not pass mat by reference here.
int solve(vector<string> mat, const string &instructions) {
    // Find the robot's starting position.
    int robot_r, robot_c;
    for (int r = 0; r < mat.size(); r++) {
        for (int c = 0; c < mat[0].size(); c++) {
            if (mat[r][c] == '@') {
                robot_r = r, robot_c = c;
            }
        }
    }

    // Read through the instructions
    for (auto c : instructions) {
        int dirR, dirC;

        // I'm sorry.
        if (c == '>') {
            dirR = 0, dirC = 1;
        } else if (c == '<') {
            dirR = 0, dirC = -1;
        } else if (c == '^') {
            dirR = -1, dirC = 0;
        } else if (c == 'v') {
            dirR = 1, dirC = 0;
        } else {
            assert(false); // Shouldn't happen (of course it did happen...)
        }

        // Try to move the robot from (r1, c1) to (r2, c2)
        int r2 = robot_r + dirR, c2 = robot_c + dirC;
        if (try_move(mat, robot_r, robot_c, r2, c2, false)) {
            try_move(mat, robot_r, robot_c, r2, c2, true);
            robot_r = r2, robot_c = c2;
        }

        // For debugging purposes: print out the matrix step by step.
        // for (auto row : mat) {
        //     for (auto i : row) {
        //         cout << i;
        //     }
        //     cout << '\n';
        // }
    }

    // Compute total score (or whatever they called it this time).
    int total = 0;
    for (int r = 0; r < mat.size(); r++) {
        for (int c = 0; c < mat[0].size(); c++) {
            if (mat[r][c] == 'O' || mat[r][c] == '[') {
                total += 100 * r + c;
            }
        }
    }
    return total;
}

int main() {
    // Get input matrix
    string line;
    vector<string> mat;
    while (true) {
        getline(cin, line);
        if (line.empty()) break;
        mat.push_back(line);
    }

    // Get the instructions
    string instructions;
    while (getline(cin, line)) {
        instructions += line;
    }

    int answer_pt1 = 0, answer_pt2 = 0;
    answer_pt1 = solve(mat, instructions);

    // Widen everything except for the robot by 2x
    for (auto &row : mat) {
        string new_row;
        for (auto c : row) {
            if (c == '@') {
                new_row += "@.";
            } else if (c == 'O') {
                new_row += "[]";
            } else {
                new_row += string(2, c);
            }
        }
        row = new_row;
    }

    // Now, solve Part 2 with the widened matrix
    answer_pt2 = solve(mat, instructions);

    cout << "Part 1: " << answer_pt1 << '\n';
    cout << "Part 2: " << answer_pt2 << '\n';
}
