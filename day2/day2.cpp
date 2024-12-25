#include <bits/stdc++.h>

using namespace std;

ifstream fin("input.txt");
// ofstream fout("output.txt");
#define cin fin
// #define cout fout

// Check if this list is "safe".
bool is_safe(vector<int> a) {
    // Maintain three variables for each condition.
    bool goes_up = true, goes_down = true, dif_safe = true;
    for (size_t i = 1; i < a.size(); i++) {
        if (a[i] > a[i-1]) {
            goes_down = false;
        } else if (a[i] < a[i-1]) {
            goes_up = false;
        }

        int dif = abs(a[i] - a[i-1]);
        if (dif < 1 || dif > 3) {
            dif_safe = false;
        }
    }

    return (goes_up || goes_down) && dif_safe;
}

int main() {
    // Update these as we go through the input.
    int cnt_pt1 = 0, cnt_pt2;

    // Read each line into a string.
    string str;
    while (getline(cin, str)) {
        // Put the string into a stringstream.
        istringstream iss;
        iss.str(str);

        // Read the current line (from the stringstream) into a `vector<int>`.
        vector<int> a;
        int val;
        while (iss >> val) {
            a.push_back(val);
        }

        // Check if it is safe by default.
        bool safe_pt1 = is_safe(a);
        bool safe_pt2 = safe_pt1;

        // Check if removing some element makes it safe.
        for (size_t j = 0; j < a.size(); j++) {
            vector<int> b = a;
            b.erase(b.begin() + j);
            safe_pt2 = safe_pt2 || is_safe(b);
        }
    
        cnt_pt1 += safe_pt1;
        cnt_pt2 += safe_pt2;
    }

    cout << "Part 1: " << cnt_pt1 << '\n';
    cout << "Part 2: " << cnt_pt2 << '\n';
}
