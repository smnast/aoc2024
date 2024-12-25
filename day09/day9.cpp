#include <bits/stdc++.h>

using namespace std;

ifstream fin("input.txt");
// ofstream fout("output.txt");
#define cin fin
// #define cout fout

long long generate_answer_from_disk(vector<int> &disk) {
    // Multiply positions by id values
    long long answer = 0;
    for (int i = 0; i < disk.size(); i++) {
        answer += i * disk[i] * (disk[i] != -1);
    }
    return answer;
}

int main() {
    // Input is easy today, at least
    string spec;
    cin >> spec;

    // Load the disk based on spec
    vector<int> disk;
    for (int i = 0; i < spec.size(); i++) {
        int count = spec[i] - '0';
        int id = i / 2;
        if (i % 2 == 0) {
            for (int j = 0; j < count; j++) {
                disk.push_back(id);
            }
        } else {
            for (int j = 0; j < count; j++) {
                disk.push_back(-1); // -1 for empty disk space
            }
        }
    }

    vector<int> disk_part1 = disk, disk_part2 = disk;

    // Part 1: Use a simple two-pointers solution
    int left = 0, right = disk_part1.size() - 1;
    while (left < right) {
        // Move pointers until there is a valid swap (or the pointers cross)
        while (left < right && disk_part1[left] != -1) {
            left++;
            continue;
        }
        while (left < right && disk_part1[right] == -1) {
            right--;
            continue;
        }

        // Swap the memory
        swap(disk_part1[left], disk_part1[right]);
    }

    // Part 2: A similar two-pointers solution, but keep track of section size
    right = disk_part2.size() - 1;
    while (right >= 0) {
        // Move right pointer until a filled space is found
        while (right >= 0 && disk_part2[right] == -1) {
            right--;
            continue;
        }
        int region_size = 0;
        while (right - region_size >= 0 && disk_part2[right - region_size] == disk_part2[right]) {
            region_size++;
        }

        // Find a valid region to swap the filled space into
        int free_begin = 0;
        int free_size = 0;
        for (int i = 0; i < right; i++) {
            if (disk_part2[i] == -1) {
                free_size++;
                if (free_size == region_size) {
                    break;
                }
            } else {
                free_begin = i + 1;
                free_size = 0;
            }
        }

        // Check if we found a valid region
        if (free_size == region_size) {
            // Swap the memory
            for (int i = 0; i < region_size; i++) {
                swap(disk_part2[free_begin + i], disk_part2[right - i]);
            }
        }

        // Move to the next space
        right -= region_size;
    }

    long long answer_part1 = generate_answer_from_disk(disk_part1);
    long long answer_part2 = generate_answer_from_disk(disk_part2);
    cout << "Part 1: " << answer_part1 << '\n';
    cout << "Part 2: " << answer_part2 << '\n';
}
