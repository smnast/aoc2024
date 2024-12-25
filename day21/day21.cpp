#include <bits/stdc++.h>

using namespace std;

ifstream fin("input.txt");
// ofstream fout("output.txt");
#define cin fin
// #define cout fout

// Lazy once again (I mean... efficient!!!)
#define int long long

// A bunch of random variables that I came up with.
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

// Robot type can be either DIR: uses a direction input keypad; or NUM: uses a numeric keypad.
typedef enum {
    DIR, NUM
} robot_type;

// States of the recursive function.
struct state {
    string command;
    int dir_robots;
    robot_type type;
};

// Define a comparator for the DP map.
bool operator <(const state &a, const state &b) {
    if (a.command == b.command) {
        if (a.dir_robots == b.dir_robots) {
            return a.type < b.type;
        }
        return a.dir_robots < b.dir_robots;
    }
    return a.command < b.command;
}

// Check if the command is legal (i.e. does not pass through the invalid spot).
bool is_legal(string command, int row, int col, robot_type type) {
    for (auto c : command) {
        switch(c) {
            case '<':
                col--;
                break;
            case '>':
                col++;
                break;
            case '^':
                row--;
                break;
            case 'v':
                row++;
                break;
        }
    
        // Check if the robot is in the invalid spot.
        if (type == NUM && num_input[row][col] == INVALID) {
            return false;
        } else if (type == DIR && dir_input[row][col] == INVALID) {
            return false;
        }
    }

    return true;
}

// This is the recursive function.
// We use the fact that all the previous robots must be at the ACCEPT button, because they must
// have just sent some input to progress the current robot to the next command.
// There is one button to press, and there are two ways to get there with the minimal number of
// commands: either go horizontally then vertically, or go vertically then horizontally.
// We check both of them with the previous robot and take the one that takes less inputs for the human.
// Also, this uses a *very* simple DP map that maps from `state`s (struct defined above) to integers,
// representing the number of inputs required by the human for this robot to execute the
// given set of commands (`commands` string).
int num_commands(map<state, int> &dp, string commands, int dir_robots, robot_type type) {
    // This is the human.
    if (dir_robots == 0) {
        return commands.size();
    }

    // Construct the state struct and check if we've already calculated the value for it.
    state cur_state = {commands, dir_robots, type};
    if (dp.count(cur_state)) {
        return dp[cur_state];
    }

    // Find the current row and column (we know it must be at the ACCEPT button, because the previous
    // command always ends with it. Here, I can even assert that for you:
    assert(commands.back() == 'A');
    int row, col;
    if (type == DIR) {
        row = 0;
        col = 2;
    } else {
        row = 3;
        col = 2;
    }

    // Compute the number of inputs required by the human for the robot to execute the given commands.
    int local_answer = 0;
    for (auto c : commands) {
        // Find where the previous robot must instruct the current robot to go.
        int req_row = -1, req_col = -1;
        if (type == DIR) {
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 3; j++) {
                    if (dir_input[i][j] == c) {
                        req_row = i, req_col = j;
                    }
                }
            }
        } else {
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 3; j++) {
                    if (num_input[i][j] == c) {
                        req_row = i, req_col = j;
                    }
                }
            }
        }
        assert(req_row != -1 && req_col != -1); // This always passes fine.

        // Construct strings with the required horizontal and vertical commands to move the robot
        // to the required row and column position.
        string horizontal_commands, vertical_commands;
        if (req_row > row) {
            horizontal_commands = string(req_row - row, 'v');
        } else if (req_row < row) {
            horizontal_commands = string(row - req_row, '^');
        }
        if (req_col > col) {
            vertical_commands = string(req_col - col, '>');
        } else if (req_col < col) {
            vertical_commands = string(col - req_col, '<');
        }

        // There are always (unless they are the same) two possible ways to input these commands:
        // either do the horizontal commands first, then the vertical commands (`commands_1`),
        // or the other way around (`commands_2`). They all must end with the ACCEPT button.
        string commands_1 = horizontal_commands + vertical_commands + "A";
        string commands_2 = vertical_commands + horizontal_commands + "A";

        // If the current robot is a directional robot, then there is one less directional robot
        // remaining.
        int new_dir_robots = dir_robots - (type == DIR);
        
        // Compute the required inputs from the human for each of these two commands.
        int commands_1_cost = num_commands(dp, commands_1, new_dir_robots, DIR);
        int commands_2_cost = num_commands(dp, commands_2, new_dir_robots, DIR);

        // Here, we check if the two commands are even legal (i.e. don't move over the invalid button).
        const int inf = 4e18;
        if (!is_legal(commands_1, row, col, type)) {
            commands_1_cost = inf;
        }
        if (!is_legal(commands_2, row, col, type)) {
            commands_2_cost = inf;
        }

        // This always passes, because there must be one valid command.
        assert(!(commands_1_cost == inf && commands_2_cost == inf));

        // Use the minimum among the two to add to the current solution.
        // We can consider these in isolation because the previous robot is now at the ACCEPT button,
        // so the current state of ALL the previous robots can be assumed to be that specific button.
        local_answer += min(commands_1_cost, commands_2_cost);

        // Move the current robot to the required location. Here, we MUST CONSIDER the robot's state,
        // but ONLY THIS STATE matters: all the previous robots are at the ACCEPT button.
        row = req_row, col = req_col;
    }

    // Memoization.
    dp[cur_state] = local_answer;
    return local_answer;
}

int solve(string code, int dir_robots) {
    map<state, int> dp;
    return num_commands(dp, code, dir_robots, NUM);
}

signed main() {
    // Get the input codes.
    vector<string> codes;
    string line;
    while (cin >> line) {
        codes.push_back(line);
    }

    // Compute the answer to both parts.
    int answer_pt1 = 0, answer_pt2 = 0;
    for (auto c : codes) {
        int num_part = stoi(c.substr(0, 3));
        int shortest_pt1 = solve(c, 2);
        int shortest_pt2 = solve(c, 25);
        answer_pt1 += num_part * shortest_pt1;
        answer_pt2 += num_part * shortest_pt2;
    }

    cout << "Part 1: " << answer_pt1 << '\n';
    cout << "Part 2: " << answer_pt2 << '\n';
}
