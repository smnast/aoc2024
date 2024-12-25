// This is a PREVIOUS attempt at solving this problem that got me Part 1, but would never work
// for Part 2. The required number of moves to get to the end in Part 2 is WAY MORE than anything
// this computer could even run even in my lifetime. I'd love to know just how long it would
// actually take...

// See, I thought Part 2 would be like "haha! now add two more directional robots!!!"
// Then I open Part 2.... and... 25???? Yeah, this won't work before I die.

#include <bits/stdc++.h>

using namespace std;

ifstream fin("input.txt");
// ofstream fout("output.txt");
#define cin fin
// #define cout fout

// Some variables for this problem.
const char INVALID = 'X';
const char NOTHING = '.';
const char dir_input[2][3] = {
    {INVALID, '^', 'A'},
    {'<', 'v', '>'}
};
const char num_input[4][3] = {
    {'7', '8', '9'},
    {'4', '5', '6'},
    {'1', '2', '3'},
    {INVALID, '0', 'A'}
};
const char human_actions[5] = {'^', 'v', '<', '>', 'A'};

// Robot types.
typedef enum {
    DIR, NUM
} robot_type;
struct robot_state {
    robot_type type;
    int row, col;
};

// Comparator for map.
bool operator <(const robot_state &a, const robot_state &b) {
    if (a.type == b.type) {
        if (a.row == b.row) {
            return a.col < b.col;
        }
        return a.row < b.row;
    }
    return a.type < b.type;
}

// Full state.
typedef pair<int, vector<robot_state>> state;

// Print the robots for debugging purposes.
void print_robots(const vector<robot_state> &cur_robots) {
    for (auto robot : cur_robots) {
        cout << robot.type << ' ' << robot.row << ' ' << robot.col << '\n';
    }
}

// Send a command (as the human) to the robots.
char send_command(vector<robot_state> &cur_robots, char command) {
    for (auto &robot : cur_robots) {
        bool accepted = false;
        if (command == '^') {
            robot.row--;
        } else if (command == 'v') {
            robot.row++;
        } else if (command == '<') {
            robot.col--;
        } else if (command == '>') {
            robot.col++;
        } else if (command == 'A') {
            if (robot.type == DIR) {
                command = dir_input[robot.row][robot.col];
            } else if (robot.type == NUM) {
                return num_input[robot.row][robot.col];
            } else {
                assert(false); // You forget how to set robot type.
            }

            accepted = true;
        } else {
            assert(false); // This should never happen.
        }

        // If the current robot did not accept, then the next robots have absolutely no changes.
        if (!accepted) {
            break;
        }
    }

    return NOTHING;
}

// Check if the current list of robots is valid (i.e. not on the INVALID spot, and not outside the keypad)
bool are_valid(const vector<robot_state> &cur_robots) {
    for (auto robot : cur_robots) {
        int rows, cols;
        if (robot.type == DIR) {
            rows = 2;
            cols = 3;
            if (dir_input[robot.row][robot.col] == INVALID) {
                return false;
            }
        } else {
            rows = 4;
            cols = 3;
            if (num_input[robot.row][robot.col] == INVALID) {
                return false;
            }
        }

        if (robot.row < 0 || robot.row >= rows || robot.col < 0 || robot.col >= cols) {
            return false;
        }
    }

    return true;
}

// Solve for the current code.
int solve(string code) {
    // Create the list of robots.
    vector<robot_state> init_robots;
    for (int i = 0; i < 2; i++) {
        init_robots.push_back({DIR, 0, 2});
    }
    init_robots.push_back({NUM, 3, 2});

    // These are used for BFS.
    set<state> visited;
    map<state, int> dist;

    // BFS. This is really slow.
    queue<pair<int, state>> q;
    q.push({0, {0, init_robots}});
    while (q.size()) {
        auto top = q.front();
        q.pop();

        int d = top.first;
        state cur_state = top.second;
        int cur_count = cur_state.first;
        vector<robot_state> cur_robots = cur_state.second;

        // Don't visit the same state multiple times (standard BFS).
        if (visited.count(cur_state)) {
            continue;
        }
        visited.insert(cur_state);
        
        // If we reached the end before the heat death of the universe, return it.
        if (cur_count == code.length()) {
            return d;
        }
        dist[cur_state] = d;

        // Check all the possible human commands to see which we have yet to visit and are valid.
        for (int k = 0; k < 5; k++) {
            vector<robot_state> new_robots = cur_robots;
            char response = send_command(new_robots, human_actions[k]);
            if (!are_valid(new_robots)) {
                continue;
            }

            // Check if the response is invalid (inputs the current character, or nothing)
            if (response != NOTHING && response != code[cur_count]) {
                continue;
            }

            // If the response is valid, increase the count.
            int new_count = cur_count;
            if (response == code[cur_count]) {
                new_count++;
            }

            // Check if we have visited the new state. If not, add it to the BFS queue.
            state new_state = {new_count, new_robots};
            if (!visited.count(new_state)) {
                q.push({d+1, new_state});
            }
        }
    }
    
    assert(false); // Uh oh!
    return -1; // Something went really wrong (something did in fact go really wrong).
}

int main() {
    // Get the code inputs.
    vector<string> codes;
    string line;
    while (cin >> line) {
        codes.push_back(line);
    }

    // Compute the answer to Part 1.
    int answer_pt1 = 0;
    for (auto c : codes) {
        int num_part = stoi(c.substr(0, 3));
        int shortest = solve(c);
        answer_pt1 += num_part * shortest;
    }

    cout << "Part 1: " << answer_pt1 << '\n';
}
