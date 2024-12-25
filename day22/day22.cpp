#include <bits/stdc++.h>

using namespace std;

ifstream fin("input.txt");
// ofstream fout("output.txt");
#define cin fin
// #define cout fout

#define int long long

// Turn vector into number (problem specific).
// It is essentially converting from base-20 representation to base-10 (adding 10 to make every digit positive).
int hash_vector(vector<int> pattern) {
    int total = 0;
    int power = 1;
    for (auto i : pattern) {
        total += (i + 10) * power;
        power *= 20;
    }

    return total;
}

// Type used for the DP map.
typedef unordered_map<int, int> dp_map;

// Mix operation.
int mix(int secret, int n) {
    return n ^ secret;
}

// Prune operation.
int prune(int n) {
    return n % 16777216;
}

// Compute how many bananas are earned by using this pattern.
int solve(const vector<int> &values, const vector<int> &pattern, dp_map &mp) {
    int state = hash_vector(pattern);
    int bananas = mp[state];
    return bananas;
}

// Store pairs of the value and the hash of each 4-length difference vector mapped to their values.
// Also stores all the patterns that occur.
void pre_computations(const vector<int> &values, dp_map &mp) {
    for (int j = 0; j < values.size(); j++) {
        int i = values[j];

        // Keep track of changes relative to the previous value.
        vector<int> changes;
        int prev = (i%10);

        // This keeps track of which change patterns have already been included in the map for this index.
        unordered_set<int> cur_patterns;

        // Simulate 2000 new numbers.
        for (int k = 0; k < 2000; k++) {
            // As described in the problem statement.
            i = prune(mix(i, i * 64));
            i = prune(mix(i, i / 32));
            i = prune(mix(i, i * 2048));

            // Compute difference based on previous value.
            int dif = (i%10) - prev;
            prev = (i%10);

            // Update the changes.
            changes.push_back(dif);
            while (changes.size() > 4) {
                changes.erase(changes.begin());
            }

            // If this state has not been added already, add the banana value to the map.
            int state = hash_vector(changes);
            if (changes.size() == 4 && !cur_patterns.count(state)) {
                cur_patterns.insert(state);
                mp[state] += prev;
            }
        }
    }
}

signed main() {
    vector<int> values;
    int v;
    while (cin >> v) {
        values.push_back(v);
    }

    // Compute answer to Part 1.
    int answer_pt1 = 0;
    for (auto i : values) {
        for (int j = 0; j < 2000; j++) {
            i = prune(mix(i, i * 64));
            i = prune(mix(i, i / 32));
            i = prune(mix(i, i * 2048));
        }

        answer_pt1 += i;
    }

    // Hash the pairs mapped to their values in the dp map.
    dp_map mp;
    pre_computations(values, mp);

    // Brute-force for Part 2.
    int answer_pt2 = 0;
    for (int a = -9; a <= 9; a++) {
        for (int b = -9; b <= 9; b++) {
            for (int c = -9; c <= 9; c++) {
                for (int d = -9; d <= 9; d++) {
                    vector<int> p = {a, b, c, d};
                    answer_pt2 = max(answer_pt2, solve(values, p, mp));
                }
            }
        }
    }

    cout << "Part 1: " << answer_pt1 << '\n';
    cout << "Part 2: " << answer_pt2 << '\n';
}
