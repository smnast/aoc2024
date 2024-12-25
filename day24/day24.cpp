#include <bits/stdc++.h>

using namespace std;

ifstream fin("input.txt");
// ofstream fout("output.txt");
// #define cin fin
// #define cout fout

// ..... this problem...
// haha i guess there's an easier way to do it that i didn't think of
// so instead, i decided to figure out exactly how the AST is put together
// and then call some nodes "sus" if they don't align with that specification
// only took 8-9 hours!!

// Different types of operations. NONE implies it's a literal (x__ or y__)
typedef enum {
    XOR,
    AND,
    OR,
    NONE
} OP;

// This struct represents an operation given in the input.
struct operation {
    OP op;
    string a, b, res;
};

// Label representing what part of the AST this is. Ex. orexp(23)
struct label {
    string type;
    int n;
};

// Node in the AST.
struct node {
    OP op;
    node *a_val, *b_val;
    string literal, memory;
    label lbl;
    int depth;
};

// Perform an operation. Used for Part 1.
int do_operation(OP op, int a_val, int b_val) {
    switch (op) {
        case XOR:
    return a_val ^ b_val;
        case OR:
            return a_val | b_val;
        case AND:
            return a_val & b_val;
        default:
            assert(false);
            return -1;
    }
}

// Solve for all the values based on the operations and provided values. Used for Part 1.
void solve_values(map<string, int> &values, vector<operation> &operations) {
    int prev_size = -1;
    while (values.size() != prev_size) {
        prev_size = values.size();
        for (auto i : operations) {
            if (values.count(i.a) && values.count(i.b)) {
                int res_value = do_operation(i.op, values[i.a], values[i.b]);
                values[i.res] = res_value;
            }
        }
    }
}

// Convert an operation to a string representation.
string op_to_string(OP op) {
    vector<string> op_strings = {"xor", "and", "or", "none"};
    string op_string = op_strings[op];
    return op_string;
}

// Convert a label to a string representation.
string label_to_string(label lbl) {
    return lbl.type + "(" + to_string(lbl.n) + ")";
}

// Convert a node to a string representation.
string node_to_info(node *node) {
    return "\"" + label_to_string(node->lbl) + "\" (" + node->memory + ")";
}

// Print the AST starting from this node, with `indent` tabs.
void print_node(node *node, int indent) {
    if (!node) {
        return;
    }

    // Print some cool information.
    string op_string = op_to_string(node->op);
    if (op_string != "none") {
        cout << string(indent, '\t') << op_string << '\n';
    }
    if (node->literal.size()) {
        cout << string(indent, '\t') << node->literal << '\n';
    }
    cout << string(indent, '\t') << node_to_info(node) << '\n';
    print_node(node->a_val, indent + 1);
    print_node(node->b_val, indent + 1);
}

// Print a concise expression that just shows the current node and its children as atomic units.
void print_concise(node *node) {
    cout << node_to_info(node) << '\n';
    cout << op_to_string(node->op) << '\n';
    cout << '\t' << node_to_info(node->a_val) << '\n';
    cout << '\t' << node_to_info(node->b_val) << '\n';
}

/* None of this is used in the final solution, but I thought it was interesting anyway.
// Check if the ASTs are the same.
bool check_same(node *a, node *b) {
    if (!a || !b) {
        return a == b;
    }

    if (a->op != b->op ||  a->literal != b->literal || a->depth != b->depth) {
        return false;
    }

    return check_same(a->a_val, b->a_val) && check_same(a->b_val, b->b_val);
}

// Try to fix this AST by replacing directly some subtree with another.
void try_fix(node *a, node *target, map<string, node*> &available_nodes) {
    if ((!a || !target) && a == target) {
        return;
    }

    bool match = true;
    if ((!a || !target) && a != target) {
        match = false;
    } else if (a->op != target->op || a->literal != target->literal) {
        match = false;
    } else if (!check_same(a->a_val, target->a_val) || !check_same(a->b_val, target->b_val)) {
        match = false;
    }

    if (!match) {
        for (auto i : available_nodes) {
            if (check_same(i.second, target)) {
                cout << "Replace " << a->memory << ' ' << i.first << '\n';
                swap(*available_nodes[a->memory], *available_nodes[i.first]);
                break;
            }
        }

        try_fix(a, target, available_nodes);
    }
}
*/

// Get a name with leading zeros. Like x02 or z23. Also works with longer strings like orexp03 if necessary.
string get_name(string c, int i) {
    if (i < 10) {
        return c + "0" + to_string(i);
    } else {
        return c + to_string(i);
    }
}

int main() {
    // Get the initial values from the input.
    map<string, int> values;
    string line;
    while (getline(fin, line) && !line.empty()) {
        string name = line.substr(0, 3);
        int value = line.back() - '0';
        values[name] = value;
    }

    // Get all the operations into a vector.
    vector<operation> operations;
    string a, b, c, d, e;
    while (fin >> a >> b >> c >> d >> e) {
        OP op;
        if (b == "XOR") {
            op = XOR;
        } else if (b == "OR") {
            op = OR;
        } else if (b == "AND") {
            op = AND;
        } else {
            assert(false);
        }
        
        operations.push_back({op, a, c, e});
    }

    // Solve for the values given the initial values and the operations.
    solve_values(values, operations);

    // Compute the answer to Part 1.
    long long answer_pt1 = 0;
    for (auto i : values) {
        if (i.first[0] == 'z' && values[i.first]) {
            int idx = stoi(i.first.substr(1, 2));
            answer_pt1 += (1ll<<idx);
        }
    }
    cout << "Part 1: " << answer_pt1 << '\n';

    // Part 2 begins...
    // Basically, we have a UI that allows us to check which nodes are "sus" and swap accordingly.
    // Every time we swap something, we have to update the nodes, so I just update the entire AST from scratch each time.
    vector<string> swaps;
    while (true) {
        // Get the initial x/y nodes.
        map<string, node*> nodes;
        for (int i = 0; i <= 44; i++) {
            string x_name = get_name("x", i);
            string y_name = get_name("y", i);
            node *x_lit = new node {NONE, 0, 0, x_name, x_name, {"x", i}, 0};
            node *y_lit = new node {NONE, 0, 0, y_name, y_name, {"y", i}, 0};
            nodes[x_lit->literal] = x_lit;
            nodes[y_lit->literal] = y_lit;
        }

        // Here, we perform all the operations until we have all the nodes.
        // During the process, we label each node based on its function in the AST.
        /* Briefly, we have:
         * 
         * xor(n) -> xor(x(n), y(n))
         * and(n) -> and(x(n), y(n))
         *
         * orexp(n):
         *   orexp(0) = and(0)
         *   orexp(n > 0) -> or(and(n), andexp(n))
         * andexp(n):
         *   andexp(0) = DNE
         *   andexp(1) = and(xor(1), and(0))... because orexp(0) = and(0)
         *   andexp(n > 1) = and(xor(n), orexp(n-1))
         * z(n):
         *   z(0) = DNE
         *   z(1) = xor(xor(1), and(0))     ... because orexp(0) = and(0)
         *   z(n) = xor(xor(n), orexp(n-1))
         */
        int prev_size = -1;
        while (nodes.size() != prev_size) {
            prev_size = nodes.size();
            for (auto i : operations) {
                if (nodes.count(i.a) && nodes.count(i.b)) {
                    if (nodes[i.a]->depth > nodes[i.b]->depth || nodes[i.a]->literal > nodes[i.b]->literal) {
                        swap(i.a, i.b);
                    }
                    int total_depth = max(nodes[i.a]->depth, nodes[i.b]->depth) + 1;
                    node *new_node = new node {i.op, nodes[i.a], nodes[i.b], "", i.res, {"default", -1}, total_depth};

                    label lbl;
                    if (total_depth == 1) {
                        switch (i.op) {
                            case XOR:
                                lbl = {"xor", nodes[i.a]->lbl.n};
                                break;
                            case AND:
                                lbl = {"and", nodes[i.a]->lbl.n};
                                break;
                            case OR:
                                lbl = {"or", nodes[i.a]->lbl.n};
                                break;
                            default:
                                assert(false);
                                break;
                        }
                    } else {
                        // For this chain in the AST, the label's value is just half the depth.
                        // This is because the pattern (of adding and & or to get to z(n+1)) has a period of 2.
                        switch (i.op) {
                            case XOR:
                                lbl = {"z", total_depth / 2};
                                break;
                            case AND:
                                lbl = {"andexp", total_depth / 2};
                                break;
                            case OR:
                                lbl = {"orexp", total_depth / 2};
                                break;
                            default:
                                assert(false);
                                break;
                        }
                    }

                    new_node->lbl = lbl;
                    nodes[i.res] = new_node;
                }
            }
        }

        // Now, we find all the "sus" nodes, which we store in `imposters`.
        // This is one of many real-life applications of "Among Us".
        vector<pair<int, node*>> imposters;
        for (auto i : nodes) {
            // Don't analyze literals.
            if (i.second->depth < 1) {
                cout << "Skipping " << i.first << '\n';
                continue;
            }

            // Make sure those exist, because we are about to use them extensively.
            assert(i.second->a_val && i.second->b_val);
            node *nd = i.second, *a = i.second->a_val, *b = i.second->b_val;

            // Find if it's sus...
            // I'm not about to write extensive comments about all these...
            // Just note they come from the definitions above.
            // We are just ensuring all the key details are consistent; otherwise, this node is "sus".
            bool sus = false;
            if (nd->lbl.type == "xor" || nd->lbl.type == "and") {
                sus = sus || a->lbl.type != "x" || b->lbl.type != "y";
                sus = sus || a->lbl.n != nd->lbl.n || b->lbl.n != nd->lbl.n;
                sus = sus || op_to_string(nd->op) != nd->lbl.type;
                sus = sus || nd->depth != 1;
            } else if (nd->lbl.type == "z") {
                sus = sus || nd->op != XOR;
                if (nd->lbl.n == 0) {
                    sus = true;
                } else if (nd->lbl.n == 1) {
                    sus = sus || a->lbl.type != "xor" || a->lbl.n != nd->lbl.n;
                    sus = sus || b->lbl.type != "and" || b->lbl.n != nd->lbl.n-1;
                } else {
                    sus = sus || a->lbl.type != "xor" || a->lbl.n != nd->lbl.n;
                    sus = sus || b->lbl.type != "orexp" || b->lbl.n != nd->lbl.n-1;
                }
            } else if (nd->lbl.type == "orexp") {
                sus = sus || nd->op != OR;
                if (nd->lbl.n == 0) {
                    sus = true;
                } else {
                    sus = sus || a->lbl.type != "and" || a->lbl.n != nd->lbl.n;
                    sus = sus || b->lbl.type != "andexp" || b->lbl.n != nd->lbl.n;
                }
            } else if (nd->lbl.type == "andexp") {
                sus = sus || nd->op != AND;
                if (nd->lbl.n == 0) {
                    sus = true;
                } else if (nd->lbl.n == 1) {
                    sus = sus || a->lbl.type != "xor" || a->lbl.n != nd->lbl.n;
                    sus = sus || b->lbl.type != "and" || b->lbl.n != nd->lbl.n-1;
                } else {
                    sus = sus || a->lbl.type != "xor" || a->lbl.n != nd->lbl.n;
                    sus = sus || b->lbl.type != "orexp" || b->lbl.n != nd->lbl.n-1;
                }
            } else {
                sus = true;
            }


            // Add the sus nodes.
            if (sus) {
                imposters.push_back({i.second->lbl.n, i.second});
            }
        }

        // Print all the sus nodes.
        sort(imposters.rbegin(), imposters.rend());
        for (auto i : imposters) {
            cout << "===== This one's sus (" << i.first << "): =====" << '\n';
            print_concise(i.second);
        }

        // This is my pretty little UI that I made because I didn't want to write an algorithm to do the same.
        while (true) {
            cout << ">>> ";
            string command;
            cin >> command;
            
            // Check the command.
            if (command == "continue") {
                break;
            } else if (command == "check") {
                // "check" command prints the concise AST for the given node.
                string node_name;
                cin >> node_name;
                if (nodes.count(node_name)) {
                    print_concise(nodes[node_name]);
                }
            } else if (command == "query") {
                // "query" command finds a node with the specified label and prints it.
                string node_label;
                cin >> node_label;
                for (auto i : nodes) {
                    if (label_to_string(i.second->lbl) == node_label) {
                        print_concise(i.second);
                        break;
                    }
                }
            } else if (command == "swap") {
                // Swap command swaps the two nodes wherever they appear in outputs in the commands.
                string mem_1, mem_2;
                cin >> mem_1 >> mem_2;
                for (auto &i : operations) {
                    if (i.res == mem_1) {
                        i.res = mem_2;
                    } else if (i.res == mem_2) {
                        i.res = mem_1;
                    }
                }
                
                // ...Then add them to the swap list.
                swaps.push_back(mem_1);
                swaps.push_back(mem_2);
            } else if (command == "list") {
                // Print all the swaps.
                if (!swaps.empty()) {
                    sort(swaps.begin(), swaps.end());
                    string answer;
                    for (auto i : swaps) {
                        answer += i + ",";
                    }
                    answer.pop_back();
                    cout << "Part 2: " << answer << '\n';
                }
            }
        }

    }
}

