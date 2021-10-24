#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>

void Sum(int left, int right, const std::vector<std::vector<int>>& v, std::vector<int>& ans) {
    for (int i = left; i < right; ++i) {
        for (int j = 0; j < v.size(); ++j) {
            ans[i] += v[j][i];
        }
    }
}

int main(){
    int k = 10000; // Number of Arrays
    int n = 10000; // Length of Arrays

    std::vector<std::vector<int>> v(k,std::vector<int>(n));
    std::vector<int> ans(v[0].size());

    for (size_t i = 0; i < k; ++i) {
        for (size_t j = 0; j < n; ++j) {
            v[i][j] = 1;
        }
    }

    std::cout << "Enter number of threads:\n";
    int threads_num;
    std::cin >> threads_num;
    if (threads_num > n) {
        threads_num = n;
    }
    if (threads_num > 8) {
        threads_num = 8;
    }

    auto begin = std::chrono::steady_clock::now(); // Initial moment of time
    if (threads_num == 0) {
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < k; ++j) {
                ans[i] += v[j][i];
            }
        }
    } else {
        std::vector<std::thread> th;
        int left = 0;
        int delta = n / threads_num;
        int right = delta;
        th.reserve(threads_num);
        for (int i = 0; i < threads_num; ++i) {
            th.emplace_back(Sum, left, right,std::ref(v), std::ref(ans));
            left = right;
            right = right + delta;
            if (i == threads_num - 2) {
                right = n;
            }
        }
        for (int i = 0; i < threads_num; ++i) {
            th[i].join();
        }
    }
    auto end = std::chrono::steady_clock::now(); // End moment of time

    for (size_t i = 0; i < n; ++i) {

        std::cout << ans[i] << " ";

    }

    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << "\nThe time: " << elapsed_ms.count() << " ms\n";
}