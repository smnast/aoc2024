#include <bits/stdc++.h>

using namespace std;

// ofstream fout("output.txt");
#define cin fin
// #define cout fout

#define int long long

// From one of my CSES solutions
int cross(complex<int> a, complex<int> b) {
    return imag(conj(a) * b);
}

int solve(complex<int> A, complex<int> B, complex<int> P) {
    // Solve using derived formula
    int tokens = 0;

    // Solve for B
    int num = cross(A, P);
    int den = cross(A, B);
    if (num % den == 0 && num / den >= 0) {
        int b = num / den;  // solved.

        // Solve for A
        int num_2 = P.real() - b * B.real();
        int den_2 = A.real();
        assert(num_2 % den_2 == 0); // tautology
        int a = num_2 / den_2; // solved.
        tokens += 3 * a + b;
    }

    return tokens;
}

// This one took me until the next day...
// I think I failed to realize we had a system of linear equations with 2 variables and 2 equations
// Unless one is a multiple of the other (not true for these inputs), we can solve "easily"
signed main() {
    FILE *file;
    file = fopen("input.txt", "r");

    // Get input (using scanf (thank god))
    vector<pair<complex<int>, complex<int>>> buttons;
    vector<complex<int>> targets;
    int ax, ay, bx, by, px, py;
    while (fscanf(file, "Button A: X+%lld, Y+%lld\n", &ax, &ay) != EOF) {
        fscanf(file, "Button B: X+%lld, Y+%lld\n", &bx, &by);
        fscanf(file, "Prize: X=%lld, Y=%lld\n", &px, &py);
        buttons.push_back({{ax, ay}, {bx, by}});
        targets.push_back({px, py});
    }

    int tokens_pt1 = 0, tokens_pt2 = 0;
    for (int i = 0; i < targets.size(); i++) {
        complex<int> pt2_offset = {(int)1e13, (int)1e13};
        tokens_pt1 += solve(buttons[i].first, buttons[i].second, targets[i]);
        tokens_pt2 += solve(buttons[i].first, buttons[i].second, targets[i] + pt2_offset);
    }

    cout << "Part 1: " << tokens_pt1 << '\n';
    cout << "Part 2: " << tokens_pt2 << '\n';
}
