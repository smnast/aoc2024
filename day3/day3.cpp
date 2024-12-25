#include <bits/stdc++.h>

using namespace std;

ifstream fin("input.txt");
// ofstream fout("output.txt");
#define cin fin
// #define cout fout

int main() {
    // I assume I got this off of stackoverflow...
    // Just read the entirety of the file into a string.
    string code((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());

    // This is some regex method I stole from my programming language (https://github.com/smnast/synthscript)
    string total_regex = R"((do\(\))|(don\'t\(\))|(mul\(\d{1,3},\d{1,3}\)))";
    regex reg(total_regex);
    smatch match;

    unsigned long long sum_pt1 = 0, sum_pt2 = 0;
    bool doing = true; 

    // Basically, just keep reading the file until there are no more regex matches.
    auto start = code.cbegin(), end = code.cend();
    while (regex_search(start, end, match, reg)) {
        // Iterate over the capturing groups. (do, dont, mul)
        for (size_t i = 1; i <= 3; i++) {
            bool is_match_empty = match[i].str().empty();
            if (!is_match_empty) {
                if (i == 1) {
                    // do()
                    doing = true;
                } else if (i == 2) {
                    // don't()
                    doing = false;
                } else if (i == 3) {
                    // mul(1,2)
                    istringstream iss;
                    iss.str(match[0].str());

                    // Drop 'mul('.
                    iss.ignore(4);

                    // Get int 1.
                    int a;
                    iss >> a;

                    // Drop ','.
                    iss.ignore();
                    
                    // Get int 2.
                    int b;
                    iss >> b;

                    sum_pt1 += a * b;
                    if (doing) {
                        sum_pt2 += a * b;
                    }
                }

                start = match[i].second;
                break;
            }
        }
    }

    cout << "Part 1: " << sum_pt1 << '\n';
    cout << "Part 2: " << sum_pt2 << '\n';
}
