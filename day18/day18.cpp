#include <bits/stdc++.h>

using namespace std;

ifstream fin("input.txt");
// ofstream fout("output.txt");
#define cin fin
// #define cout fout

int check_dist(vector<vector<bool>> safe) {
    // Define directions in matrix.
    const int dir[4][2] = {
        {0, 1},
        {0, -1},
        {1, 0},
        {-1, 0}
    };

    // Define matrix size
    int rows = safe.size(), cols = safe[0].size();

    // BFS approach.
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    queue<pair<int, vector<int>>> q;
    q.push({0, {0, 0}});
    while (q.size()) {
        auto top = q.front();
        q.pop();
        int d = top.first, r = top.second[0], c = top.second[1];
        
        if (r == rows-1 && c == cols-1) {
            return d;
        }
    
        if (visited[r][c]) {
            continue;
        }
        visited[r][c] = true;
    
        for (int k = 0; k < 4; k++) {
            int nr = r + dir[k][0], nc = c + dir[k][1];
            if (nr < 0 || nr >= rows || nc < 0 || nc >= cols || !safe[nr][nc]) {
                continue;
            }
            q.push({d+1, {nr, nc}});
        }
    }

    return -1; // No path.
}

int main() {
    // Get input.
    vector<pair<int, int>> coords;
    int r, c;
    while (cin >> c) {
        cin.ignore(1);
        cin >> r;
        coords.push_back({r,c});
    }

    const int rows = 71, cols = 71;
    int steps = 1024;

    // Determine which spots are safe at the last step.
    vector<vector<bool>> safe_cur(rows, vector<bool>(cols, true));
    for (int i = 0; i < steps; i++) {
        safe_cur[coords[i].first][coords[i].second] = false;
    }

    // Calculate answer to Part 1.
    int answer_pt1 = check_dist(safe_cur);
    cout << "Part 1: " << answer_pt1 << '\n';

    // Determine which byte prevents exit.
    safe_cur.assign(rows, vector<bool>(cols, true));
    steps = coords.size();
    for (int i = 0; i < steps; i++) {
        safe_cur[coords[i].first][coords[i].second] = false;

        // If ths byte blocks exit...
        if (check_dist(safe_cur) == -1) {
            cout << "Part 2: " << coords[i].second << ',' << coords[i].first << '\n';
            break;
        }
    }
}
