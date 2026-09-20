#include <bits/stdc++.h>
using namespace std;

// CircularBuffer(size_t capacity);
// ~CircularBuffer();

// void push(const T& value);
// void pop();

// T& front();
// T& back();

// bool empty() const;
// bool full() const;
// size_t size() const;
// size_t capacity() const;

template <typename T>
class RingBuffer{
    private:
        T* data_;
        size_t size_;
        size_t capacity_;
        size_t head_;
    public:
        RingBuffer() = delete;
        RingBuffer(size_t capacity) : data_(nullptr), size_(0), capacity_(capacity), head_(0) {
            if(capacity == 0)
                throw invalid_argument("Capacity 0 is not allowed");
            data_ = new T[capacity];
        }
        ~RingBuffer() {
            delete[] data_;
        }
        RingBuffer(const RingBuffer& other){
            size_ = other.size_;
            capacity_ = other.capacity_;
            head_ = other.head_;
            data_ = new T[capacity_];
            for(int i = 0 ; i<capacity_ ; ++i){
                data_[i] = other.data_[i];
            }
        }
        RingBuffer& operator=(const RingBuffer& other){
            if(this == &other)
                return *this;
            delete[] data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            head_ = other.head_;
            data_ = new T[capacity_];
            for(int i = 0 ; i<capacity_ ; ++i){
                data_[i] = other.data_[i];
            }
            return *this;
        }
        RingBuffer(RingBuffer &&other) noexcept{
            size_ = other.size_;
            capacity_ = other.capacity_;
            head_ = other.head_;
            data_ = other.data_;
            other.size_ = 0;
            other.capacity_ = 0;
            other.head_ = 0;
            other.data_ = nullptr;
        }
        RingBuffer& operator=(RingBuffer &&other) noexcept{
            if(this == &other)
                return *this;
            delete[] data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            head_ = other.head_;
            data_ = other.data_;
            other.size_ = 0;
            other.capacity_ = 0;
            other.head_ = 0;
            other.data_ = nullptr;
            return *this;
        }
        void push(const T &value){
            size_t newIndex = (head_ + size_) % capacity_;
            data_[newIndex] = value;
            if(size_ == capacity_){
                head_ = (head_ + 1) % capacity_;
            }
            else{
                size_++;
            }
        }
        T& front(){
            if(size_ == 0)
                throw out_of_range("size of buffer is 0");
            return data_[head_];   
        }

        T& back() {
            if (size_ == 0)
                throw out_of_range("size of buffer is 0");

            size_t index = (head_ + size_ - 1) % capacity_;
            return data_[index];
        }

        void pop(){
            if(size_ == 0)
                throw out_of_range("size of buffer is 0");
            head_ = (head_+1)%capacity_;
            size_ = size_-1;
        }

        bool empty() const{
            return (size_== 0);
        }

        bool full() const{
            return (size_ == capacity_);
        }

        size_t size() const{
            return size_;
        }

        size_t capacity() const{
            return capacity_;
        }

        void clear(){
            head_ = 0;
            size_ = 0;
        }

        T& operator[](size_t index) {
            if (index >= size_)
                throw out_of_range("index out of range");

            size_t newIndex = (head_ + index) % capacity_;
            return data_[newIndex];
        }
        const T& operator[](size_t index) const {
            if (index >= size_)
                throw out_of_range("index out of range");

            size_t newIndex = (head_ + index) % capacity_;
            return data_[newIndex];
        }
};