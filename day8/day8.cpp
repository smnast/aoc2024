#include <bits/stdc++.h>

using namespace std;

ifstream fin("input.txt");
// ofstream fout("output.txt");
#define cin fin
// #define cout fout

int main() {
    // Get input grid
    vector<string> mat;
    string line;
    while (getline(cin, line)) {
        mat.push_back(line);
    }

    // I was originally using pair<int, int>, but I realized complex works really well for this!
    map<char, vector<complex<int>>> mp;
    int n = mat.size();
    int m = mat[0].size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (mat[i][j] == '.') {
                continue;
            }

            // Add node location
            mp[mat[i][j]].push_back({i, j});
        }
    }

    // Store which positions have antinodes (so we do not overcount them)
    // Need a separate array for each part. Otherwise nodes found in part 2 would
    // count for part 1, but wouldn't increase the answer.
    vector<vector<bool>> contains_pt1(n, vector<bool>(m, false));
    vector<vector<bool>> contains_pt2 = contains_pt1;
    int answer_pt1 = 0, answer_pt2 = 0;
    for (auto p : mp) {
        for (int i = 0; i < p.second.size(); i++) {
            for (int j = i + 1; j < p.second.size(); j++) {
                // Indices array to switch between starting points
                vector<int> indices = {i, j};
                for (int start = 0; start <= 1; start++) {
                    int first = indices[start], second = indices[(start+1)%2];
                    int k = 0;  // k is the distance multiplier.
                    while (true) {
                        // std::complex is beautiful...
                        complex<int> displacement = p.second[first] - p.second[second];
                        complex<int> anti = p.second[first] + displacement * k;
                        if (anti.real() >= 0 && anti.real() < n && anti.imag() >= 0 && anti.imag() < m) {
                            // Part 1: can only have antinode by one times the distance away
                            if (k == 1) {
                                answer_pt1 += !contains_pt1[anti.real()][anti.imag()];
                                contains_pt1[anti.real()][anti.imag()] = true;
                            }
                            
                            // Can increase the answer for part 2 at any integer multiple of the distance
                            answer_pt2 += !contains_pt2[anti.real()][anti.imag()];
                            contains_pt2[anti.real()][anti.imag()] = true;
                        } else {
                            break;
                        }

                        // Increase distance each time
                        k += 1;
                    }
                }
            }
        }
    }

    cout << "Part 1: " << answer_pt1 << '\n';
    cout << "Part 2: " << answer_pt2 << '\n';
}
