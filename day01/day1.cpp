#include <bits/stdc++.h>

using namespace std;

ifstream fin("input.txt");
//ofstream fout("output.txt");
#define cin fin
//#define cout fout

int main() {
    // Get the input vectors.
    int a, b;
    vector<int> as, bs;
    while (cin >> a >> b) {
        as.push_back(a);
        bs.push_back(b);
    }

    // Sort them.
    sort(as.begin(), as.end());
    sort(bs.begin(), bs.end());

    // Part 1: Count the total absolute difference.
    unsigned long long total_dist = 0;
    for (int i = 0; i < as.size(); i++) {
        total_dist += abs(as[i] - bs[i]);
    }
    cout << "Part 1: " << total_dist << '\n';

    // Part 2: Count the total of the product of a[i] and count[a[i]].
    map<int, int> counts;
    for (auto i : bs) {
        counts[i]++;
    }
    unsigned long long total = 0;
    for (int i = 0; i < as.size(); i++) {
        total += as[i] * counts[as[i]];
    }
    cout << "Part 2: " << total << '\n';
}
