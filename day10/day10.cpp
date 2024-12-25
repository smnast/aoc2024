#include <bits/stdc++.h>

using namespace std;

ifstream fin("input.txt");
// ofstream fout("output.txt");
#define cin fin
// #define cout fout

// Define directions globally
const int dir[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

// Returns how many paths we can access from here
// Optional parameter: visited. If nullptr, then we do not check if we go over a path multiple times
int count_paths(vector<vector<int>> &mat, vector<vector<bool>> *visited, int r, int c) {
    // If visited array is provided, do not go over a path multiple times
    if (visited && (*visited)[r][c]) {
        return 0;
    } else if (visited) {
        (*visited)[r][c] = true;
    }

    // Return for 9 values AFTER checking if we visited
    if (mat[r][c] == 9) {
        return 1;
    }

    // Check in the four directions
    int paths = 0;
    for (int k = 0; k < 4; k++) {
        int nr = r + dir[k][0];
        int nc = c + dir[k][1];

        // Check if the move is valid
        if (nr < 0 || nc < 0 || nr >= mat.size() || nc >= mat[0].size() ||
            mat[nr][nc] - mat[r][c] != 1) {
            continue;
        }

        paths += count_paths(mat, visited, nr, nc);
    }

    return paths;
}

int main() {
    // Get input matrix
    vector<vector<int>> mat;
    string line;
    while (getline(cin, line)) {
        vector<int> row;
        for (auto c : line) {
            row.push_back(c - '0');
        }
        mat.push_back(row);
    }

    // Define some variables for matrices
    int rows = mat.size();
    int cols = mat[0].size();

    // Compute answers to parts 1 and 2
    int answer_pt1 = 0;
    int answer_pt2 = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // Only count paths for zeros (starting points)
            if (mat[i][j] != 0) {
                continue;
            }

            // Provide the visited array to the function for part 1
            vector<vector<bool>> visited(rows, vector<bool>(cols, false));
            answer_pt1 += count_paths(mat, &visited, i, j);

            // Don't provide the visited array to the function for part 2
            answer_pt2 += count_paths(mat, nullptr, i, j);
        }
    }

    cout << "Part 1: " << answer_pt1 << '\n';
    cout << "Part 2: " << answer_pt2 << '\n';
}
