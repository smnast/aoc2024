#include <bits/stdc++.h>

using namespace std;

ifstream fin("input.txt");
// ofstream fout("output.txt");
#define cin fin
// #define cout fout

// Typing "long long" is too hard apparently...
#define int long long

signed main() {
    // Get input matrix
    string line;
    vector<string> mat;
    while (getline(cin, line)) {
        mat.push_back(line);
    }

    // Some variables from matrix
    int n = mat.size();
    int m = mat[0].size();
    int dir[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    // Keep track of which tiles we've visited already so far so we don't double count
    vector<vector<bool>> visited(n, vector<bool>(m, false));

    // Compute the answers!
    int answer_pt1 = 0, answer_pt2 = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            // Don't count the same tile multiple times (and thus, the same region!)
            if (visited[i][j]) {
                continue;
            }
            
            // Walls keeps track of which tiles have walls in which directions
            // An optimization: move this outside the loop and carefully reset each time.
            vector<vector<vector<bool>>> walls(n, vector<vector<bool>>(m, vector<bool>(4, false)));

            // We compute the area and perimeter for this region
            int cur_area = 0, cur_perimeter = 0, cur_walls = 0;

            // A classic BFS approach works well here
            queue<pair<int, int>> q;
            q.push({i, j});
            while (q.size()) {
                // Get the next tile from the queue
                auto top = q.front();
                q.pop();
                int r = top.first, c = top.second;

                // Don't visit the same tile multiple times
                if (visited[r][c]) {
                    continue;
                }
                visited[r][c] = true;

                // Area of the region = the number of tiles visited
                cur_area++;

                // Check each direction for walls and thus perimeter
                for (int k = 0; k < 4; k++) {
                    // New row and column in the k-th direction
                    int nr = r + dir[k][0], nc = c + dir[k][1];

                    // Check if there should be a wall here
                    if (nr < 0 || nc < 0 || nr >= n || nc >= m || mat[r][c] != mat[nr][nc]) {
                        // Another way to solve this problem (ignore the horrible variable names...)
                        // pair<int, int> anti_direction = {-dir[k][1], dir[k][0]};
                        // int nrr = r + anti_direction.first, ncc = c + anti_direction.second;
                        // if (nrr < 0 || ncc < 0 || nrr >= n || ncc >= m || mat[nrr][ncc] != mat[r][c]) {
                        //     cur_walls++;                           
                        // } else {
                        //     int nnrr = nrr + dir[k][0], nncc = ncc + dir[k][1];
                        //     if (nnrr < 0 || nncc < 0 || nnrr >= n || nncc >= m || mat[nrr][ncc] != mat[nnrr][nncc]) {
                        //     } else {
                        //         cur_walls++;
                        //     }
                        // }

                        walls[r][c][k] = true;
                        cur_perimeter++;
                    }
                    // Otherwise, it's part of the current region
                    else if (!visited[nr][nc]) {
                        q.push({nr, nc});
                    }
                }
            }

            // Maybe this isn't the most elegant, but it works.
            // As we sweep along a row/column, we check if it is the first wall to be seen
            // going in the current direction. If not, then it's part of a larger wall.
            //
            // For example:
            //     
            //   +X+O+O+ +X+
            //   |B B B| |B|
            //   +     +X+ +
            //   |B B B B B|
            //   +-+-+-+-+-+
            //
            // The X's mark where a wall in the up direction starts. The O's mark where
            // we have already seen a wall in this direction without some break. Note there
            // are two larger walls on the first row, because there is a break between them.
            //
            // We check in the horizontal directions first (walls above and below)
            for (int k = 0; k < 2; k++) {
                bool prev = false;
                for (int a = 0; a < n; a++) {
                    for (int b = 0; b < m; b++) {
                        if (walls[a][b][k]) {
                            cur_walls += !prev;
                        }
                        prev = walls[a][b][k];
                    }
                }
            }

            // Then the vertical direction (walls left and right)
            for (int k = 2; k < 4; k++) {
                bool prev = false;
                for (int b = 0; b < m; b++) {
                    for (int a = 0; a < n; a++) {
                        if (walls[a][b][k]) {
                            cur_walls += !prev;
                        }
                        prev = walls[a][b][k];
                    }
                }
            }

            // Add the region's price to the total answer
            answer_pt1 += cur_area * cur_perimeter;
            answer_pt2 += cur_area * cur_walls;
        }
    }

    cout << "Part 1: " << answer_pt1 << '\n';
    cout << "Part 2: " << answer_pt2 << '\n';
}
