#include <string>
#include <iostream>
#include <fstream>
#include <random>
#include <unordered_set>
#include <map>

#pragma GCC optimize("O3", "unroll-loops")

std::random_device rd;
std::mt19937 gen(rd());

int main() {
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    std::hash<std::string> h;
    char str[15];
    int count;
    std::vector<std::string> ans;
    std::vector<int> most_posible_buckets = {976369, 902483, 834181, 771049, 712697,
                                             658753, 608903, 562841, 520241};
    std::string filename = "/home/admin/data/aboba";
    for (int i = 0; i < most_posible_buckets.size(); i++) {
        count = 0;
        std::fstream fout(filename + std::to_string(i));
        while (count < 15) {
            for (int j = 0; j < 15; j++) {
                str[j] = gen() % 26 + 'a';
            }
            auto aboba = std::string(str, 15);
            if (h(aboba) % most_posible_buckets[i] == most_posible_buckets[i] - 1) {
                fout << aboba << '\n';
                count++;
            }
        }
    }
}
