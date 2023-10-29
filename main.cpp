#include <string>
#include <iostream>
#include <fstream>
#include <random>
#include <unordered_set>
#include <map>

#pragma GCC optimize("O3", "unroll-loops")

std::random_device rd;
std::mt19937 gen(rd());

void next(std::string& s) {
    for(int i = s.size(); i >= 0; --i) {
        if(s[i] == 'z') {
            s[i] = 'A';
            break;
        } else if (s[i] == 'Z') {
            s[i] = '0';
            break;
        } else if (s[i] == '9') {
            s[i] = '_';
            break;
        } else if (s[i] == '_') {
            s[i] = 'a';
        } else {
            s[i] += 1;
            break;
        }
    }
}

int main() {
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    std::hash<std::string> h;
    std::string str(15, 'a');
    int count;
    std::vector<std::string> ans;
    int posible_bucket = 976369;
    std::string filename = "../data/aboba0";
    for (int i = 0; i < most_posible_buckets.size(); i++) {
        count = 0;
        std::fstream fout(filename + std::to_string(i));
        while (count < 150) {
            if (h(str) % most_posible_buckets[i] == most_posible_buckets[i] - 1) {
                fout << aboba << '\n';
                count++;
            }
            next(str);
        }
    }
}
