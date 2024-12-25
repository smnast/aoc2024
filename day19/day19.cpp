#include <bits/stdc++.h>

using namespace std;

ifstream fin("input.txt");
// ofstream fout("output.txt");
#define cin fin
// #define cout fout

// Lazy.
#define int long long

// Damn... this was easy.
signed main() {
    // Get the available towel designs.
    // Here, I use the fact that the final design does not have a comma after, so I just keep 
    // reading until there is a word with no trailing comma. Also, I make sure to remove the
    // comma for the other ones.
    vector<string> available;
    string s;
    while (cin >> s) {
        if (s.back() == ',') {
            s.pop_back();
            available.push_back(s);
        } else {
            available.push_back(s);
            break;
        }
    }

    // Read the desired patterns.
    vector<string> patterns;
    while (cin >> s) {
        patterns.push_back(s);
    }

    // Solve both parts at the same time.
    int answer_pt1 = 0, answer_pt2 = 0;
    for (auto p : patterns) {
        // Kind of a DP array. I started by storing a vector<bool> of whether it was possible
        // to continue the pattern at a given index using the available towel designs. For Part 2,
        // I just had to change to a vector<int> that stores the number of ways to continue.
        vector<int> possible(p.size()+1, 0);
        possible[p.size()] = 1; // There is one way to form nothing... I mean... I guess?

        // Iterate through the desired design.
        for (int j = p.size()-1; j >= 0; j--) {
            // Check for each available pattern.
            for (auto a : available) {
                // If this pattern would take us beyond the length of the desired pattern,
                // or it's not possible to continue, we can ignore this one.
                if (a.size() + j > p.size() || !possible[a.size() + j]) {
                    continue;
                }

                // Check if the strings match (i.e. we can use this pattern in the solution)
                bool match = true;
                for (int k = 0; k < a.size(); k++) {
                    if (a[k] != p[j+k]) {
                        match = false;
                        break;
                    }
                }

                // If they do match, then using this pattern adds possible[a.size() + j] new ways
                // to form the pattern from index `j` onward.
                if (match) {
                    possible[j] += possible[a.size() + j];
                }
            }
        }

        // If there are any ways to continue the pattern from the first index onward, add to Part 1.
        if (possible[0]) {
            answer_pt1++;
        }

        // Add to Part 2 to the total number of ways to make the desired pattern.
        answer_pt2 += possible[0];
    }

    // That's it.
    cout << "Part 1: " << answer_pt1 << '\n';
    cout << "Part 2: " << answer_pt2 << '\n';
}
