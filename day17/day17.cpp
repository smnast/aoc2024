#include <bits/stdc++.h>

using namespace std;

// ifstream fin("input.txt");
// ofstream fout("output.txt");
// #define cin fin
// #define cout fout

// Lazy once again.
#define int long long

// This simulates the program execution as described in the problem statement.
vector<int> run_program(int reg_a, int reg_b, int reg_c, vector<int> program, bool verify_eq) {
    // Program output.
    vector<int> output;

    // Instruction pointer
    int reg_inst = 0;

    // While the program does not halt...
    while (reg_inst >= 0 && reg_inst < program.size()) {
        int opcode = program[reg_inst];
        int operand = program[reg_inst+1]; // Idk why we can read this every time.

        // Find the operands
        int literal_operand = operand;
        int combo_operand = literal_operand;
        if (operand > 3) {
            vector<int> registers = {reg_a, reg_b, reg_c};
            combo_operand = registers[combo_operand - 4];
        }

        // Stores whether we will jump two positions after executing the operation.
        bool jumping_after = true;

        // Division operation.
        if (opcode == 0 || opcode == 6 || opcode == 7) {
            int num = reg_a, den = (1<<combo_operand);
            int res = num / den;
            if (opcode == 0) {
                reg_a = res;
            } else if (opcode == 6) {
                reg_b = res;
            } else {
                reg_c = res;
            }
        // Xor literal operation.
        } else if (opcode == 1) {
            reg_b = reg_b ^ literal_operand;
        // Mod 8 operation.
        } else if (opcode == 2) {
            reg_b = combo_operand % 8;
        // Jump operation.
        } else if (opcode == 3) {
            if (reg_a != 0) {
                reg_inst = literal_operand;
                jumping_after = false;
            }
        // Xor C operation.
        } else if (opcode == 4) {
            reg_b = reg_b ^ reg_c;
        // Output operation.
        } else if (opcode == 5) {
            output.push_back(combo_operand % 8);
            // If we are trying to verify whether this output is correct,
            // we can exit early if what we have is already incorrect.
            if (verify_eq) {
                if (output.size() > program.size() || program[output.size()-1] != output.back()) {
                    return {}; // Just return empty vector so checking is quickest.
                }
            }
        } else {
            assert(false); // This shouldn't (and doesn't) happen.
        }
    
        // Jump two positions ahead if we did not execute a jump operation.
        if (jumping_after) {
            reg_inst += 2;
        }
    }

    return output;
}

// Funky branch & bound or something...
// Basically, I was doing this manually for a while and then realized it was too hard to do manually
// so I just wrote basically the same thing as an function.
void try_find(vector<int> &solutions, const vector<int> &program, int lower_bound, int upper_bound, int tail_found) {
    // If this search space is small enough, we can use a linear search.
    if (upper_bound - lower_bound < 100000) {
        for (int i = lower_bound; i <= upper_bound; i++) {
            if (program == run_program(i, 0, 0, program, true)) {
                // This is a valid solution. (i.e. program == output)
                solutions.push_back(i);
            }
        }
        return;
    }

    // In the current search space, we choose `count` evenly-spaced solutions to check.
    int count = 100;
    int per = max(1ll, (upper_bound-lower_bound)/count);

    // If first == -1 or last == -1, we are not currently in a region where the tail is correct.
    int first = -1, last = -1;
    for (int i = lower_bound; i <= upper_bound; i += per) {
        // Execute the program, and it should have the correct size.
        vector<int> output = run_program(i, 0, 0, program, false);
        assert(output.size() == program.size());

        // Check if the program output matches the tail for one more position than previously found.
        bool match_all = true;
        for (int j = output.size()-1; j >= output.size()-1-tail_found; j--) {
            if (output[j] != program[j]) {
                match_all = false;
            }
        }

        // If that was the case, then we are in a region we should search.
        if (match_all) {
            if (first == -1) {
                first = i;
            }
            last = i;
        } else {
            // Check if we were previously in some region that we should search.
            if (first != -1 && last != -1) {
                // We need to subtract/add `per` because the region
                // is not necessarily bounded by the `first` and `last` positions.
                try_find(solutions, program, first-per, last+per, tail_found+1);
            }

            // Reset the region.
            first = last = -1;
        }
    }
}

// If `include_eq`, this function finds the first position where the output size is the program size.
// Otherwise, it finds the first position where that is not the case.
int bin_search_size(const vector<int> &program, bool include_eq) {
    int l = 0, r = 4e18, best = -1;
    while (l <= r) {
        int mid = l+(r-l)/2;
        vector<int> output = run_program(mid, 0, 0, program, false);
        if (output.size() > program.size() || (include_eq && output.size() == program.size())) {
            best = mid;
            r = mid-1;
        } else {
            l = mid+1;
        }
    }
    return best;
}

signed main() {
    // Get input (I should just use Python at this point...)
    FILE *file = fopen("input.txt", "r");
    int reg_a, reg_b, reg_c;
    fscanf(file, "Register A: %lld\n", &reg_a);
    fscanf(file, "Register B: %lld\n", &reg_b);
    fscanf(file, "Register C: %lld\n", &reg_c);

    char program_line[100];
    fscanf(file, "Program: %s\n", program_line);

    istringstream iss;
    iss.str(program_line);
    vector<int> program;
    int v;
    while (iss >> v) {
        program.push_back(v);
        iss.ignore(1);
    }

    // Find program output for Part 1.
    vector<int> output = run_program(reg_a, reg_b, reg_c, program, false);
    cout << "Part 1: ";
    for (int i = 0; i < output.size(); i++) {
        cout << output[i];
        if (i+1 != output.size()) {
            cout << ',';
        }
    }
    cout << '\n';

    // The solutions are bounded by the first and last positions where output size == program size.
    // Don't ask me why that is.
    int lower_bound = bin_search_size(program, true);
    int upper_bound = bin_search_size(program, false);

    vector<int> solutions;
    try_find(solutions, program, lower_bound, upper_bound, 0);

    // Find the minimum solution (there can be multiple, so we have to find the minumum among them).
    sort(solutions.begin(), solutions.end());
    cout << "Part 2: " << solutions[0] << '\n';
}
