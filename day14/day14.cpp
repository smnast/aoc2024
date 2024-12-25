#include <bits/stdc++.h>

using namespace std;

// ifstream fin("input.txt");
// ofstream fout("output.txt");
// #define cin fin
// #define cout fout

int main() {
    // Get input
    FILE *file = fopen("input.txt", "r");
    vector<complex<int>> positions_pt1, velocities;
    int px, py, vx, vy;
    while (fscanf(file, "p=%d,%d v=%d,%d\n", &px, &py, &vx, &vy) != EOF)  {
        positions_pt1.push_back({px, py});
        velocities.push_back({vx, vy});
    }

    // Create backup vectors for part 2
    vector<complex<int>> positions_pt2 = positions_pt1;

    // Some input variables
    int n = positions_pt1.size();
    int room_width = 101, room_height = 103;

    // Move each robot for 100 seconds
    for (int j = 0; j < n; j++) {
        positions_pt1[j] += velocities[j] * 100;
        positions_pt1[j] = {
            ((positions_pt1[j].real() % room_width) + room_width) % room_width,
            ((positions_pt1[j].imag() % room_height) + room_height) % room_height,
        };
    }

    // Compute the scores for each quadrant
    vector<int> scores(4, 0);
    for (int j = 0; j < n; j++) {
        int quad = 0;

        // Add 1 bit if it's right
        quad += 1 * (positions_pt1[j].real() > room_width / 2);

        // Add 2 bit if it's up
        quad += 2 * (positions_pt1[j].imag() > room_height / 2);

        // Make sure it's not in the center
        if (positions_pt1[j].real() != room_width / 2 && positions_pt1[j].imag() != room_height / 2) {
            scores[quad]++;
        }
    }

    // Compute the product of all the scores
    int prod = 1;
    for (auto i : scores) {
        prod *= i;
    }
    cout << "Part 1: " << prod << '\n';

    // Very strange part 2. This is my second year in AOC (essentially first), so I didn't know
    // what this problem would be testing. I spent a few minutes staring at terminal inputs,
    // checking it manually...
    for (int seconds = 0; ; seconds++) {
        // For all the positions in the room, store whether there is a robot there
        vector<vector<int>> mat(room_height, vector<int>(room_width, 0));
        for (int j = 0; j < n; j++) {
            mat[positions_pt2[j].imag()][positions_pt2[j].real()] = 1;
        }

        // Check how many filled tiles are fully surrounded by other filled tiles
        int fully_surrounded = 0;
        for (int i = 1; i < room_width-1; i++) {
            for (int j = 1; j < room_height-1; j++) {
                // Make sure this one is filled
                if (!mat[i][j]) {
                    continue;
                }

                // Check how many filled tiles surround this one
                int surround = 0;
                for (int a = -1; a <= 1; a++) {
                    for (int b = -1; b <= 1; b++) {
                        surround += mat[i+a][j+b];
                    }
                }

                // If all the tiles around this one are filled, it is fully surrounded
                fully_surrounded += surround == 8;
            }
        }

        // Just randomly guess how many would be a good threshold. And it works first try...
        if (fully_surrounded > 5) {
            // Show the user which tiles are filled visually
            for (auto i : mat) {
                for (auto j : i) {
                    cout << (j ? '#' : '.');
                }
                cout << '\n';
            }

            // Print out some information
            cout << "I think this might be a christmas tree...\n";
            cout << fully_surrounded << " are fully surrounded.\n";
            cout << seconds << " seconds have passed.\n";
            
            // Get the user's feedback
            string feedback;
            getline(cin, feedback);
            if (feedback == "yep, that's definitely a christmas tree.") {
                cout << "OK...Part 2: " << seconds << '\n';
                break;
            }
        }

        // Update all the robot's positions for one second
        for (int j = 0; j < n; j++) {
            positions_pt2[j] += velocities[j];
            positions_pt2[j] = {
                ((positions_pt2[j].real() % room_width) + room_width) % room_width,
                ((positions_pt2[j].imag() % room_height) + room_height) % room_height,
            };
        }
    }
}
