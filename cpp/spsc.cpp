#include <bits/stdc++.h>
using namespace std;

template <typename T, size_t capacity>
class SPSCQueue{
private:
    T buffer_[capacity];
    alignas(64) std::atomic<size_t> head_;
    alignas(64) std::atomic<size_t> tail_;
    //alignas(std::hardware_destructive_interference_size)
    //std::atomic<size_t> head_{0};
public:
    SPSCQueue()
        : head_(0), tail_(0)
    {
    }
    bool push(const T&value){
        size_t current_tail = tail_.load(std::memory_order_relaxed);
        size_t current_head = head_.load(std::memory_order_acquire);
        if (current_tail - current_head == capacity)
                return false;
        buffer_[current_tail%capacity] = value;
        tail_.store(current_tail+1,memory_order_release);
        return true;   
    }
    bool pop(T &value){
        size_t current_head = head_.load(memory_order_relaxed);
        size_t current_tail = tail_.load(memory_order_acquire);
        if(current_head == current_tail)
            return false;
        value = std::move(buffer_[current_head%capacity]);
        head_.store(current_head + 1, std::memory_order_release);
        return true;
    }
};

int main() {
    SPSCQueue<int, 100> q;

    constexpr int N = 100000;

    thread producer([&]() {
        for (int i = 0; i < N; ) {
            if (q.push(i)) {
                ++i;
            }
        }
    });

    thread consumer([&]() {
        int value;

        for (int i = 0; i < N; ) {
            if (q.pop(value)) {
                if (value != i) {
                    cout << "ERROR: expected "
                         << i << ", got " << value << endl;
                    return;
                }
                ++i;
            }
        }
    });

    producer.join();
    consumer.join();

    cout << "Test completed\n";

    return 0;
}