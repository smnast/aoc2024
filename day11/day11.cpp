#include <bits/stdc++.h>

using namespace std;

ifstream fin("input.txt");
// ofstream fout("output.txt");
#define cin fin
// #define cout fout

// Typing "long long" is too hard apparently...
#define int long long

// Recursive DP: count number of stones of a single rock after blinking some number of times
int count_stones(map<pair<int, int>, int> &mp, int value, int rem_blinks) {
    // Base case: no more blinks
    if (rem_blinks == 0) {
        return 1;
    }

    // Current state, and check if we have been here already and have an answer saved
    pair<int, int> cur_pair = {value, rem_blinks};
    if (mp.count(cur_pair)) {
        return mp[cur_pair];
    }

    // Find the return value
    int return_val;
    string str_rep = to_string(value);
    // Case 1: value = 0 -> turn it into a 1
    if (value == 0) {
        return_val = count_stones(mp, 1, rem_blinks - 1);
    // Case 2: number of digits is even -> split into two halves
    } else if (str_rep.size() % 2 == 0) {
        int first_half = stoll(str_rep.substr(0, str_rep.size() / 2));
        int second_half = stoll(str_rep.substr(str_rep.size() / 2));
        return_val = count_stones(mp, first_half, rem_blinks - 1) +
                     count_stones(mp, second_half, rem_blinks - 1);
    // Case 3: else -> multiply by 2024
    } else {
        return_val = count_stones(mp, value * 2024, rem_blinks - 1);
    }

    // Save value for this state
    mp[cur_pair] = return_val;
    return return_val;
}

signed main() {
    // Get vector of numbers input (very small input today!)
    vector<int> a;
    int v;
    while (cin >> v) {
        a.push_back(v);
    }

    // Map for DP
    map<pair<int, int>, int> mp;
    
    // Store counts for both parts
    int total_pt1 = 0, total_pt2 = 0;
    for (auto i : a) {
        total_pt1 += count_stones(mp, i, 25);
        total_pt2 += count_stones(mp, i, 75);
    }

    cout << "Part 1: " << total_pt1 << '\n';
    cout << "Part 2: " << total_pt2 << '\n';
}
