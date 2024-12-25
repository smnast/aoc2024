#include <bits/stdc++.h>

using namespace std;

ifstream fin("input.txt");
// ofstream fout("output.txt");
#define cin fin
// #define cout fout

int main() {
    // We read two vectors: the totals (left number), the values (numbers on the right)
    vector<long long> totals;
    vector<vector<long long>> values;
    string line;
    while (getline(cin, line)) {
        // Create a string stream from the current line
        istringstream iss;
        iss.str(line);

        // Get the total (left number)
        long long a;
        iss >> a;
        totals.push_back(a);

        // Ignore ": " (2 characters)
        iss.ignore(2);

        // Get the values (numbers on the right)
        vector<long long> v;
        long long b;
        while (iss >> b) {
            v.push_back(b);
        }
        values.push_back(v);
    }

    // Prints the parsed values in the same format as input for comparison:
    // for (int i = 0; i < totals.size(); i++) {
    //     cout << totals[i] << ": ";
    //     for (auto j : values[i]) {
    //         cout << j << ' ';
    //     }
    //     cout << '\n';
    // }

    long long answer_pt1 = 0;
    long long answer_pt2 = 0;
    for (int i = 0; i < totals.size(); i++) {
        cout << "Currently doing: " << i + 1 << "/" << totals.size() << '\n';

        bool found_pt1 = false;
        bool found_pt2 = false;

        // Create bitmaps where the i-th bit represents the operation
        // between the i-th value and the (i+1)-th value.
        // j represents a bitmap of multiplication operations
        // k represents a bitmap of addition operations
        // if neither j nor k, then append operation
        for (int j = 0; j < (1 << (values[i].size() - 1)); j++) {
            for (int k = 0; k < (1 << (values[i].size() - 1)); k++) {
                // We don't need to have both j and k set, so exit early if that's found.
                if ((j & k) != 0) {
                    continue;
                }

                long long total_pt1 = values[i][0];
                long long total_pt2 = values[i][0];
                for (int l = 0; l < values[i].size() - 1; l++) {
                    int pw = 1 << l;
                    if ((pw & j) && !(pw & k)) { // if only j
                        // Simple product
                        total_pt1 *= values[i][l + 1];
                        total_pt2 *= values[i][l + 1];
                    } else if (!(pw & j) && (pw & k)) { // if only k
                        // Simple addition
                        total_pt1 += values[i][l + 1];
                        total_pt2 += values[i][l + 1];
                    } else if (!(pw & j) && !(pw & k)) { // if neither j nor k
                        // Append in string format
                        total_pt2 = stoll(to_string(total_pt2) + to_string(values[i][l + 1]));
                    }
                }

                // If the total is the number on the left, then add to the answer
                if (total_pt1 == totals[i] && !found_pt1) {
                    answer_pt1 += total_pt1;
                    found_pt1 = true;
                }
                if (total_pt2 == totals[i] && !found_pt2) {
                    answer_pt2 += total_pt2;
                    found_pt2 = true;
                }

                // We can exit early if we found a solution to both parts
                if (found_pt1 && found_pt2) {
                    break;
                }
            }

            // We can exit early if we found a solution to both parts
            if (found_pt1 && found_pt2) {
                break;
            }
        }
    }

    cout << "Part 1: " << answer_pt1 << '\n';
    cout << "Part 2: " << answer_pt2 << '\n';
}
