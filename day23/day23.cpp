#include <bits/stdc++.h>

using namespace std;

ifstream fin("input.txt");
// ofstream fout("output.txt");
#define cin fin
// #define cout fout

void find_cliques_bitwise(set<set<string>> &cliques, set<string> &universe, map<string, set<string>> &adj) {
    // Use a bitwise solution to find all cliques in the graph.
    for (auto s : universe) {
        // Check all 2^n bitmasks, where each bit represents whether each adjacent node is taken.
        int n = adj[s].size();
        for (int i = 0; i < (1<<n); i++) {
            // Construct the candidate set which might be a clique.
            set<string> candidate = {s};
            int idx = 0;
            for (auto t : adj[s]) {
                // If the bit is set, include this node. Otherwise, don't.
                if ((1<<idx) & i) {
                    candidate.insert(t);
                }
                idx++;
            }

            // Check if this set of nodes is a clique.
            bool clique = true;
            for (auto a : candidate) {
                for (auto b : candidate) {
                    if (a != b && !adj[a].count(b)) {
                        clique = false;
                        break;
                    }
                }

                // Exit early.
                if (!clique) {
                    break;
                }
            }

            if (clique) {
                cliques.insert(candidate);
            }
        }
    }
}

// https://en.wikipedia.org/wiki/Bron%E2%80%93Kerbosch_algorithm
void bron_kerbosch(set<set<string>> &cliques, map<string, set<string>> &adj, set<string> &r, set<string> &p, set<string> &x) {
    if (p.empty() && x.empty()) {
        cliques.insert(r);
        return;
    }

    set<string> prev_p = p;
    for (auto node : prev_p) {
        set<string> new_r = r;
        new_r.insert(node);
        set<string> new_p, new_x;
        for (auto i : adj[node]) {
            if (p.count(i)) {
                new_p.insert(i);
            }
            if (x.count(i)) {
                new_x.insert(i);
            }
        }
    
        bron_kerbosch(cliques, adj, new_r, new_p, new_x);

        p.erase(node);
        x.insert(node);
    }
}

// https://en.wikipedia.org/wiki/Bron%E2%80%93Kerbosch_algorithm
// With pivoting.
// Not a noticable performance improvement.
void bron_kerbosch_pivot(set<set<string>> &cliques, map<string, set<string>> &adj, set<string> &r, set<string> &p, set<string> &x) {
    if (p.empty() && x.empty()) {
        cliques.insert(r);
        return;
    }

    string pivot;
    if (p.size()) {
        pivot = *p.begin();
    } else {
        pivot = *x.begin();
    }

    set<string> searchable = p;
    for (auto i : adj[pivot]) {
        if (searchable.count(i)) {
            searchable.erase(i);
        }
    }

    for (auto node : searchable) {
        set<string> new_r = r;
        new_r.insert(node);
        set<string> new_p, new_x;
        for (auto i : adj[node]) {
            if (p.count(i)) {
                new_p.insert(i);
            }
            if (x.count(i)) {
                new_x.insert(i);
            }
        }
    
        bron_kerbosch_pivot(cliques, adj, new_r, new_p, new_x);

        p.erase(node);
        x.insert(node);
    }
}

void find_cliques_bron_kerbosch(set<set<string>> &cliques, set<string> &universe, map<string, set<string>> &adj) {
    set<string> r, p = universe, x;
    bron_kerbosch_pivot(cliques, adj, r, p, x);
}

int main() {
    // Get input. The universe is all the nodes in the entire graph. The adjacency map is as described in the input.
    set<string> universe;
    map<string, set<string>> adj;
    string line;
    while (cin >> line) {
        // Lines are like XX-YY, so the first two are the first node, the last two are the second node.
        string a = line.substr(0, 2);
        string b = line.substr(3, 2);
        adj[a].insert(b);
        adj[b].insert(a);

        universe.insert(a);
        universe.insert(b);
    }

    // Simple bruteforce for Part 1.
    int answer_pt1 = 0;
    for (auto s : universe) {
        for (auto t : adj[s]) {
            for (auto p : adj[t]) {
                if (adj[s].count(p)) {
                    // One of the three must have a name starting with 't'.
                    if (s[0] == 't' || t[0] == 't' || p[0] == 't') {
                        answer_pt1++;
                    }
                }
            }
        }
    }

    // Note that the answer overcounts by 6x, because we do not care about order.
    // For every set of three nodes X Y Z, there are 3! = 6 ways to arrange them such that
    // we have the same three computers albeit in a different order (which doesn't matter to us).
    answer_pt1 /= 6;
    cout << "Part 1: " << answer_pt1 << '\n';

    // Find all the cliques in this graph.
    set<set<string>> cliques;
    find_cliques_bron_kerbosch(cliques, universe, adj);

    // Find the maximum size of a clique in this graph.
    int max_size = 0;
    for (auto c : cliques) {
        max_size = max(max_size, (int)c.size());
    }

    // Find the clique with maximum size and report it.
    cout << "Part 2: ";
    for (auto c : cliques) {
        if (c.size() == max_size) {
            string answer;
            for (auto d : c) {
                answer += d + ",";
            }
            answer.pop_back();
            cout << answer;
            break;
        }
    }
    cout << '\n';
}
