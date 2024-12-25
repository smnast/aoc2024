#include <bits/stdc++.h>

using namespace std;

ifstream fin("input.txt");
// ofstream fout("output.txt");
#define cin fin
// #define cout fout

int main() {
    // "implementation hell"..... i like these ones haha
    vector<string> mat;
    string line;
    while (getline(cin, line)) {
        mat.push_back(line);
    }

    int height = (int)mat.size();
    int width = (int)mat[0].size();

    string target = "XMAS";
    string target_rev = "SAMX";

    // Check cardinal directions.
    unsigned long long count_pt1 = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j < width-3) {
                // right and left
                string substr = mat[i].substr(j, 4);
                count_pt1 += (substr == target) + (substr == target_rev);
            }
            if (i < height-3) {
                // up and down
                string substr;
                for (int k = 0; k < 4; k++) {
                    substr.push_back(mat[i+k][j]);
                }
                count_pt1 += (substr == target) + (substr == target_rev);
            }
            if (i < height-3 && j < width-3) {
                // diagonal up left and down right
                string substr;
                for (int k = 0; k < 4; k++) {
                    substr.push_back(mat[i+k][j+k]);
                }
                count_pt1 += (substr == target) + (substr == target_rev);
            }
            if (i < height-3 && j >= 3) {
                // diagonal up left and down right
                string substr;
                for (int k = 0; k < 4; k++) {
                    substr.push_back(mat[i+k][j-k]);
                }
                count_pt1 += (substr == target) + (substr == target_rev);
            }
        }
    }

    cout << "Part 1: " << count_pt1 << '\n';

    // Check for 'x' shape.
    string target1 = "MAS", target2 = "SAM";
    unsigned long long count_pt2 = 0;
    for (int i = 0; i < height-2; i++) {
        for (int j = 0; j < width-2; j++) {
            string d1, d2;
            for (int k = 0; k < 3; k++) {
                d1.push_back(mat[i+k][j+k]);
                d2.push_back(mat[i+k][j+2-k]);
            }

            if ((d1 == target1 || d1 == target2) && (d2 == target1 || d2 == target2)) {
                count_pt2++;
            }
        }
    }

    cout << "Part 2: " << count_pt2 << '\n';
}
