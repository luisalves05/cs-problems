#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <algorithm>

#define N 5

std::mutex forks[N];
std::mutex print_mu;

void think(int i) { 
    std::lock_guard<std::mutex> print_guard(print_mu);
    std::cout << "Philosopher #" << i << " - sleeping 3 seconds" << std::endl; 
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
}

void eat(int i) { 
    std::lock_guard<std::mutex> print_guard(print_mu);
    std::cout << "Philosopher #" << i << " - eating 5 seconds" << std::endl; 
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
}

void init(int i) {
    while (true) { 
        think(i);
        std::unique_lock<std::mutex> right_fork (
                                        forks[std::min(i, (i + 1) % N)]);
        std::unique_lock<std::mutex> left_fork (
                                        forks[std::max(i, (i + 1) % N)]);
        eat(i); // pizza
        left_fork.unlock();
        right_fork.unlock();
    }
}

int main() {
    std::thread phils[N];
    for (size_t i = 0; i < N; i++) {
        phils[i] = std::thread(init, i);
    }
    for (size_t i = 0; i < N; i++) {
        phils[i].join();
    }
    return 0;
}
