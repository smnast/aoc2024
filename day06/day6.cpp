#include <bits/stdc++.h>

using namespace std;

ifstream fin("input.txt");
// ofstream fout("output.txt");
#define cin fin
// #define cout fout

bool check_loop(vector<string> &map, vector<vector<int>> &visited) {
    int n = map.size();
    int m = map[0].size();

    // Find the initial position and rotation
    int dir_index = 0;
    pair<int, int> dir = {-1, 0};
    pair<int, int> pos = {-1, 0};
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (map[i][j] == '^') {
                pos = {i, j};
            }
        }
    }
    assert(pos.first != -1);  // make sure the position is found

    bool done = false;
    while (!done) {
        // If we have already been here with this rotation, then there is a loop
        if (visited[pos.first][pos.second] == dir_index) {
            return true;
        }
        visited[pos.first][pos.second] = dir_index;  // Store the last rotation for this position
        
        // Keep rotating until we can move forward
        while (true) {
            // next_pos = pos + dir
            pair<int, int> next_pos = {pos.first + dir.first, pos.second + dir.second};

            // Handle moving outside of the map
            if (next_pos.first < 0 || next_pos.first >= n || next_pos.second < 0 || next_pos.second >= m) {
                done = true;
                break;
            }
            
            // If the next pos is a block then rotate
            if (map[next_pos.first][next_pos.second] == '#') {
                dir = {dir.second, -dir.first};
                dir_index = (dir_index+1)%4;  // Keep track of the rotation as 1 of 4 indices
            } else {
                pos = next_pos;
                break;
            }
        }
    }

    // There was no loop
    return false;
}

int main() {
    // Get map input from file
    vector<string> map;
    string line;
    while (getline(cin, line)) {
        map.push_back(line);
    }

    int n = map.size();
    int m = map[0].size();

    // PART 1: Check how many were visited before leaving
    vector<vector<int>> visited(n, vector<int>(m, -1));
    check_loop(map, visited);
    int count_visited = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            count_visited += (visited[i][j] != -1);
        }
    }

    // PART 2: Check how many tiles can create loops
    int count_loop = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            // Only place loops on empty spots
            if (map[i][j] != '.') {
                continue;
            }
        
            visited.assign(n, vector<int>(m, -1)); // Reset visited
            map[i][j] = '#';  // Add block
            if (check_loop(map, visited)) {  // Check for loop
                count_loop++;
            }
            map[i][j] = '.';  // Remove block
        }
    }

    cout << "Part 1: " << count_visited << '\n';
    cout << "Part 2: " << count_loop << '\n';
}
