#include <bits/stdc++.h>

using namespace std;

ifstream fin("input.txt");
// ofstream fout("output.txt");
#define cin fin
// #define cout fout

// I am lazy.
#define int long long

signed main() {
    // Get the input matrix.
    vector<string> mat;
    string line;
    while (getline(cin, line)) {
        mat.push_back(line);
    }

    // Find the starting position.
    int start_r, start_c;
    for (int r = 0; r < mat.size(); r++) {
        for (int c = 0; c < mat[0].size(); c++) {
            if (mat[r][c] == 'S') {
                start_r = r, start_c = c;
            }
        }
    }

    // Define some constants for the problem.
    const int dir[4][2] = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };
    const int forward_cost = 1;
    const int turn_cost = 1000;

    // I stole this from one of my old CSES submissions. Dijkstra's algorithm:
    const int INF = 1e18;
    vector<vector<vector<int>>> dist(mat.size(), vector<vector<int>>(mat[0].size(), vector<int>(4, INF)));
    priority_queue<pair<int, vector<int>>, vector<pair<int, vector<int>>>, greater<pair<int, vector<int>>>> pq;
    dist[start_r][start_c][0] = 0;
    pq.push({0, {start_r, start_c, 0}});
    while (pq.size()) {
        // Get the node with current least distance.
        auto top = pq.top();
        pq.pop();
        int d = top.first;
        vector<int> node = top.second;
        int r = node[0], c = node[1], rot = node[2];
        
        // If the distance was updated since placing in queue, there is a better path.
        // I believe this saves us from having to remove nodes from the queue.
        if (dist[r][c][rot] != d) continue;
 
        // Forward.
        int nr = r + dir[rot][0], nc = c + dir[rot][1];
        if (nr >= 0 && nr < mat.size() && nc >= 0 && nc < mat[0].size() && mat[nr][nc] != '#') {
            if (dist[nr][nc][rot] > d + forward_cost) {
                dist[nr][nc][rot] = d + forward_cost;
                pq.push({d + forward_cost, {nr, nc, rot}});
            }
        }

        // Clockwise rotation.
        if (dist[r][c][(rot+1)%4] > d + turn_cost) {
            dist[r][c][(rot+1)%4] = d + turn_cost;
            pq.push({d + turn_cost, {r, c, (rot+1)%4}});
        }

        // Counterclockwise rotation.
        if (dist[r][c][(rot+3)%4] > d + turn_cost) {
            dist[r][c][(rot+3)%4] = d + turn_cost;
            pq.push({d + turn_cost, {r, c, (rot+3)%4}});
        }
    }

    int answer_pt1 = INF, answer_pt2 = 0;

    // Find the end position in the matrix.
    queue<vector<int>> q;
    int end_r, end_c;
    for (int r = 0; r < mat.size(); r++) {
        for (int c = 0; c < mat[0].size(); c++) {
            if (mat[r][c] == 'E') {
                // Find the minimum distance (answer to Part 1).
                for (int rot = 0; rot < 4; rot++) {
                    answer_pt1 = min(answer_pt1, dist[r][c][rot]);
                }

                // Find which rotations lead to this minimum distance (used for Part 2).
                for (int rot = 0; rot < 4; rot++) {
                    if (dist[r][c][rot] == answer_pt1) {
                        q.push({r, c, rot});
                    }
                }
            }
        }
    }

    // Here we do a similar algorithm to Dijkstra's used in Part 1, but in reverse.
    // We maintain a queue containing only states that are on minimum paths, and work backward
    // to find which states can lead to that state.
    vector<vector<vector<bool>>> visited(mat.size(), vector<vector<bool>>(mat[0].size(), vector<bool>(4, false)));
    while (q.size()) {
        // Find the next node from the queue.
        auto node = q.front();
        q.pop();
        int r = node[0], c = node[1], rot = node[2];
        
        // Visited keeps track of which states are on the best paths.
        if (visited[r][c][rot]) continue;
        visited[r][c][rot] = true;

        // Forward (in reverse!).
        int nr = r - dir[rot][0], nc = c - dir[rot][1];
        if (nr >= 0 && nr < mat.size() && nc >= 0 && nc < mat[0].size() && mat[nr][nc] != '#') {
            if (dist[nr][nc][rot] + forward_cost == dist[r][c][rot]) {
                q.push({nr, nc, rot});
            }
        }

        // Clockwise rotation (in reverse!)
        if (dist[r][c][(rot+3)%4] + turn_cost == dist[r][c][rot]) {
            q.push({r, c, (rot+3)%4});
        }

        // Counterclockwise rotation (in reverse!)
        if (dist[r][c][(rot+1)%4] + turn_cost == dist[r][c][rot]) {
            q.push({r, c, (rot+1)%4});
        }
    }

    // Count which tiles have possible states.
    for (int r = 0; r < mat.size(); r++) {
        for (int c = 0; c < mat[0].size(); c++) {
            // Check if any rotations of this tile are on the best paths.
            bool found = false;
            for (int k = 0; k < 4; k++) {
                if (visited[r][c][k]) {
                    found = true;
                }
            }

            // If so, increase the answer to Part 2.
            if (found) {
                answer_pt2++;
            }
        }
    }

    cout << "Part 1: " << answer_pt1 << '\n';
    cout << "Part 2: " << answer_pt2 << '\n';
}
