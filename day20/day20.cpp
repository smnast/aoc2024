#include <bits/stdc++.h>

using namespace std;

ifstream fin("counter_test.txt");
// ofstream fout("output.txt");
#define cin fin
// #define cout fout

int main() {
    // Get input matrix.
    vector<string> mat;
    string line;
    while (cin >> line) {
        mat.push_back(line);
    }

    // Some variables for the matrix.
    const int inf = 1e9;
    const int dir[4][2] = { {-1, 0}, {1, 0}, {0, 1}, {0, -1} };
    int n = mat.size(), m = mat[0].size();

    // How many steps the end is from each position.
    vector<vector<int>> dist(n, vector<int>(m, inf)); // inf means not visited, or a wall.

    // Find the end position and start the queue there.
    queue<pair<int, pair<int, int>>> q;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (mat[i][j] == 'E') {
                q.push({0, {i, j}});
            }
        }
    }

    // BFS 1. Find the length of the shortest path from each position in the matrix to the end.
    while (q.size()) {
        auto top = q.front();
        q.pop();
        int d = top.first, r = top.second.first, c = top.second.second;

        // If we've already visited this, then this path is at least as long.
        if (dist[r][c] != inf) {
            continue;
        }
        dist[r][c] = d;

        // Check each direction to see if we can move there.
        for (int k = 0; k < 4; k++) {
            int nr = r + dir[k][0], nc = c + dir[k][1];
            if (nr >= 0 && nr < n && nc >= 0 && nc < m && mat[nr][nc] != '#') {
                q.push({d+1, {nr, nc}});
            }
        }
    }

    // Time to compute the answers.
    int answer_pt1 = 0, answer_pt2 = 0;
    
    // Find the start position and begin the queue with it instead this time.
    queue<pair<int, int>> q2;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (mat[i][j] == 'S') {
                q2.push({i, j});
            }
        }
    }
    
    // BFS 2. Move along all the possible shortest paths from the start to the end.
    // For each position, we check if cheating starting here would reduce the path by >= 100.
    // Actually, this is WRONG, because we could have a counter test where the shortest path
    // can be even shorter by going a DIFFERENT WAY ENTIRELY USING A CHEAT. Except this wasn't
    // tested.
    // (The "above":)
    // Nevermind, it is RIGHT, because there is only a single shortest path, and the cheat must
    // start and end on this shortest path. I just accidentally assumed that and got lucky haha...
    // ...Even still, see the "counter_test.txt" file for a simple counter test. If this limitation
    // wasn't in place, then ignoring this type of path would cause an incorrect solution.
    vector<vector<bool>> visited(n, vector<bool>(m, false));
    while (q2.size()) {
        auto top = q2.front();
        q2.pop();
        int r1 = top.first, c1 = top.second;
        
        // Don't visit the same place twice, also mitigating overcounting with the unique solutions.
        if (visited[r1][c1]) {
            continue;
        }
        visited[r1][c1] = true;

        // Find the position where the cheat will end.
        for (int r2 = 0; r2 < n; r2++) {
            for (int c2 = 0; c2 < m; c2++) {
                // Check how long we use the cheat for in this case.
                int jump = abs(c2-c1) + abs(r2-r1);
                int asdf = dist[r1][c1] - dist[r2][c2] - jump;
                if (asdf < 100000 && asdf > 90 && jump <= 2) {
                    cout << asdf << '\n';
                }
                if (dist[r1][c1] - dist[r2][c2] >= 100 + jump) {
                    // Increase answer for each part.
                    if (jump <= 2) {
                        answer_pt1++;
                    }
                    if (jump <= 20) {
                        answer_pt2++;
                    }
                }
            }
        }

        // Check each adjacent position to see if going there would take us along a shortest path.
        // This is actually -w-r-o-n-g- RIGHT (SEE ABOVE).
        for (int k = 0; k < 4; k++) {
            int nr = r1 + dir[k][0], nc = c1 + dir[k][1];
            if (nr >= 0 && nr < n && nc >= 0 && nc < m && dist[nr][nc] == dist[r1][c1] - 1) {
                q2.push({nr, nc});
            }
        }
    }

    cout << "Part 1: " << answer_pt1 << '\n';
    cout << "Part 2: " << answer_pt2 << '\n';
}
