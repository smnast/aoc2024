#include <bits/stdc++.h>

using namespace std;

ifstream fin("input.txt");
// ofstream fout("output.txt");
#define cin fin
// #define cout fout

int main() {
    string line;

    // Create a map representing which numbers should be after each number
    map<int, set<int>> mp;
    while (getline(cin, line) && line != "") { // Read new lines until a blank line
        // Create a string stream of the current line
        istringstream iss;
        iss.str(line);

        // Read a|b
        int a, b;
        iss >> a;     // read a
        iss.ignore(); // ignore the |
        iss >> b;     // read b

        // Add the input pair
        mp[a].insert(b);
    }

    // Get the initial orderings
    vector<vector<int>> v;
    while (getline(cin, line)) {  // Read until EOF
        // Create a string stream of the current line
        istringstream iss;
        iss.str(line);

        // Create the integer values from format a,b,c,d,e...
        vector<int> cur;
        int a;
        while (iss >> a) {  // Read until there are no more values
            cur.push_back(a);
            iss.ignore();  // ignore the comma (or the newline? i think)
        }

        // Add this ordering
        v.push_back(cur);
    }

    // Calculate the answers
    unsigned long long total_part1 = 0, total_part2 = 0;
    for (auto cur : v) {
        size_t n = cur.size();

        // Find if there are any conflicts
        bool good = true;
        for (size_t i = 0; i < n; i++) {
            for (size_t j = i + 1; j < n; j++) {
                if (mp[cur[j]].count(cur[i])) {
                    // There is a conflict
                    good = false;
                }
            }
        }

        // PART 1
        if (good) {
            // Add the middle elements of all initially good orderings
            total_part1 += (unsigned long long)cur[n / 2];
        }

        // PART 2
        if (!good) {
            // Essentially a bubble sort algorithm
            while (true) {
                // Keep track of swaps
                bool swapped = false;

                // Find conflicts
                for (size_t i = 0; i < n; i++) {
                    for (size_t j = i + 1; j < n; j++) {
                        if (mp[cur[j]].count(cur[i])) {
                            // Conflict: swap the elements
                            swap(cur[j], cur[i]);
                            swapped = true;
                        }
                    }
                }

                // If there were no swaps, then the ordering is good
                if (!swapped) {
                    break;
                }
            }

            // Add the middle elements of all initially not-good orderings
            total_part2 += (unsigned long long)cur[n / 2];
        }
    }

    cout << "Part 1: " << total_part1 << '\n';
    cout << "Part 2: " << total_part2 << '\n';
}
