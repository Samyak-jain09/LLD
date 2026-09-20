#include <bits/stdc++.h>
using namespace std;

template <typename T>
class ThreadSafeQueue{
    private:
        queue<T>q_;
        mutable mutex m_;
    public:
        ThreadSafeQueue() = default;
        void push(const T& val){
            std::lock_guard<std::mutex> lock(m_);
            q_.push(val);
        }
        bool pop(){
            std::lock_guard<std::mutex> lock(m_);
            if(q_.empty())
                return false;
            q_.pop();
            return true;
        }

        bool empty() const{
            std::lock_guard<std::mutex> lock(m_);
            return q_.empty();
        }

        size_t size() const{
            std::lock_guard<std::mutex> lock(m_);
            return q_.size();
        }
};