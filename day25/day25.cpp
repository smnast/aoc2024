#include <bits/stdc++.h>

using namespace std;

ifstream fin("input.txt");
// ofstream fout("output.txt");
#define cin fin
// #define cout fout

// This one mostly just took me a minute because I'm a little slow at typing...
// Honestly, it's quite sad to see Part 2 just be free like that, but in fact maybe it helps me on my private leaderboard...
// Loved the problems, however, and it's nice to end with this one... because I can go to bed on time for once haha
int main() {
    // We read all the keys line-by-line, adding to the vectors when there is an empty line.
    string line;
    vector<vector<string>> locks;
    vector<vector<string>> keys;
    vector<string> cur;
    while (getline(cin, line)) {
        // If there is an empty line, add to the corresponding vector.
        if (line.empty()) {
            // Empty last line means it's a lock.
            if (cur.back() == ".....") {
                locks.push_back(cur);
            } else {
                keys.push_back(cur);
            }
            cur.clear();
        } else {
            cur.push_back(line);
        }
    }

    int rows = locks[0].size();
    int cols = locks[0][0].size();

    // Compute the heights for each lock.
    vector<vector<int>> lock_heights(locks.size(), vector<int>(cols));
    for (int i = 0; i < locks.size(); i++) {
        for (int j = 0; j < cols; j++) {
            int height = 0;
            for (int k = 0; k < rows; k++) {
                if (locks[i][k][j] == '.') {
                    break;
                }
                height++;
            }
            lock_heights[i][j] = height;
        }
    }

    // Compute the heights for each key.
    vector<vector<int>> key_heights(keys.size(), vector<int>(cols));
    for (int i = 0; i < keys.size(); i++) {
        for (int j = 0; j < cols; j++) {
            int height = 0;
            for (int k = rows-1; k >= 0; k--) {
                if (keys[i][k][j] == '.') {
                    break;
                }
                height++;
            }
            key_heights[i][j] = height;
        }
    }

    // Compute the answer to Part 1 by checking each pair of a key and a lock.
    int answer_pt1 = 0;
    for (auto a : key_heights) {
        for (auto b : lock_heights) {
            // Check if it fits. Each column should not add to more than `rows`.
            bool fits = true;
            for (int j = 0; j < cols; j++) {
                int total_height = a[j] + b[j];
                if (total_height > rows) {
                    fits = false;
                    break;
                } 
            }

            // Add to the answer if it fits.
            answer_pt1 += fits;
        }
    }

    cout << "Part 1: " << answer_pt1 << '\n';
    
    // ... and that's it.
}

