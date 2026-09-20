#include <bits/stdc++.h>
using namespace std;

template <typename T>
class BlockingQueue{
    private:
        queue<T> queue_;
        std::mutex m_;
        std::condition_variable cv_;
    public:
        BlockingQueue() = default;
        void push(const T& val){
            {
                std::lock_guard<std::mutex> lc(m_);
                queue_.push(val);
            }
            cv_.notify_one();
        }
        void wait_and_pop(T& value){
            std::unique_lock<std::mutex> lock(m_);

            cv_.wait(lock, [&] {
                return !queue_.empty();
            });

            value = std::move(queue_.front());
            queue_.pop();
        }
        bool try_pop(T& value){
            std::lock_guard<std::mutex> lock(m_);
            if(queue_.empty())
                return false;
            value = std::move(queue_.front());
            queue_.pop();
            return true;
        }
};

#include <bits/stdc++.h>
using namespace std;

template <typename T>
class BlockingQueue {
private:
    queue<T> queue_;
    size_t capacity_;

    mutable std::mutex m_;
    std::condition_variable not_empty_;
    std::condition_variable not_full_;

public:
    BlockingQueue(size_t capacity)
        : capacity_(capacity)
    {
        if (capacity == 0)
            throw invalid_argument("Capacity must be greater than 0");
    }

    // Producer
    void push(const T& value) {
        {
            std::unique_lock<std::mutex> lock(m_);

            // Wait until there is space
            not_full_.wait(lock, [this] {
                return queue_.size() < capacity_;
            });

            queue_.push(value);
        }

        // Wake one waiting consumer
        not_empty_.notify_one();
    }

    // Consumer - blocks if queue is empty
    void wait_and_pop(T& value) {
        {
            std::unique_lock<std::mutex> lock(m_);

            // Wait until there is an item
            not_empty_.wait(lock, [this] {
                return !queue_.empty();
            });

            value = std::move(queue_.front());
            queue_.pop();
        }

        // We removed an item, so a producer may now proceed
        not_full_.notify_one();
    }

    // Consumer - does not block
    bool try_pop(T& value) {
        {
            std::lock_guard<std::mutex> lock(m_);

            if (queue_.empty())
                return false;

            value = std::move(queue_.front());
            queue_.pop();
        }

        // Space is now available
        not_full_.notify_one();

        return true;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(m_);
        return queue_.empty();
    }

    bool full() const {
        std::lock_guard<std::mutex> lock(m_);
        return queue_.size() == capacity_;
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(m_);
        return queue_.size();
    }

    size_t capacity() const {
        return capacity_;
    }
};