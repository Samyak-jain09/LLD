#include <bits/stdc++.h>
using namespace std;

template <typename T>
class Vector{
    private:
        T* data_;
        size_t size_;
        size_t capacity_;
        void __cleanup(){
            if(data_!=nullptr){
                delete[] data_;
                data_ = nullptr;
                size_ = 0;
                capacity_ = 0;
            }
        }
    public:
        Vector(){
            data_ = nullptr;
            size_ = 0;
            capacity_ = 0;
        }
        Vector(size_t size){
            data_ = new T[size];
            size_ = size;
            capacity_ = size;
        }
        ~Vector(){
            __cleanup();
        }
        Vector(const Vector &other){
            size_ = other.size_;
            capacity_ = other.capacity_;
            data_ = new T[capacity_];
            for(size_t i = 0 ; i<size_; ++i){
                *(data_+i) = *(other.data_+i);
            }
        }
        Vector& operator=(const Vector &other){
            if(this==&other)
                return *this;
            __cleanup();
            size_ = other.size_;
            capacity_ = other.capacity_;
            data_ = new T[capacity_];
            for(size_t i = 0 ; i<size_; ++i){
                *(data_+i) = *(other.data_+i);
            }
            return *this;
        }
        Vector (Vector&& other) noexcept{
            size_ = other.size_;
            capacity_= other.capacity_;
            data_ = other.data_;
            other.size_ = 0;
            other.capacity_ = 0;
            other.data_ = nullptr;
        }
        Vector& operator=(Vector &&other) noexcept{
            if(this == &other)
                return *this;
            __cleanup();
             size_ = other.size_;
            capacity_= other.capacity_;
            data_ = other.data_;
            other.size_ = 0;
            other.capacity_ = 0;
            other.data_ = nullptr;
            return *this;
        }
        void push_back(const T& value) {
            if (size_ == capacity_) {
                size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
                T* new_data = new T[new_capacity];
                for (size_t i = 0; i < size_; ++i) {
                    new_data[i] = data_[i];
                }
                delete[] data_;
                data_ = new_data;
                capacity_ = new_capacity;
            }
            data_[size_] = value;
            size_++;
        }
        size_t size() const{
            return this->size_;
        }
        size_t capacity() const{
            return this->capacity_;
        }
        T& operator[](size_t index){
            if(index>=size_){
                throw out_of_range("index out of range");
            }
            return *(data_+index);
        }
        void reserve(size_t new_capacity){
            if(capacity_>=new_capacity)
                return;
            T* buffer = new T[new_capacity];
            for(size_t i = 0 ; i<size_ ; ++i){
                buffer[i] = data_[i];
            }
            delete[] data_;
            capacity_ = new_capacity;
            data_ = buffer;
        }
        bool empty() const {
            return size_ == 0;
        }
        T& front() {
            return data_[0];
        }

        T& back() {
            return data_[size_ - 1];
        }
        void pop_back() {
            if (size_ > 0)
                --size_;
        }
        void clear() {
            // remove all elements
            size_ = 0;
        }
        void resize(size_t new_size) {
            if (new_size == size_)
                return;

            if (new_size < size_) {
                size_ = new_size;
                return;
            }

            if (new_size > capacity_) {
                T* new_data = new T[new_size];
                for (size_t i = 0; i < size_; ++i) {
                    new_data[i] = data_[i];
                }
                delete[] data_;
                data_ = new_data;
                capacity_ = new_size;
            }
            size_ = new_size;
        }
};

//raw
#include <bits/stdc++.h>
using namespace std;

template <typename T>
class Vector {
private:
    T* data_;
    size_t size_;
    size_t capacity_;

    T* allocate(size_t capacity) {
        if (capacity == 0)
            return nullptr;

        return static_cast<T*>(
            ::operator new(sizeof(T) * capacity)
        );
    }

    void destroy_elements() {
        for (size_t i = 0; i < size_; ++i) {
            data_[i].~T();
        }
        size_ = 0;
    }

    void cleanup() {
        destroy_elements();

        ::operator delete(data_);

        data_ = nullptr;
        capacity_ = 0;
    }

public:

    // Default constructor
    Vector()
        : data_(nullptr),
          size_(0),
          capacity_(0) {}

    // Construct size elements
    Vector(size_t n)
        : data_(allocate(n)),
          size_(0),
          capacity_(n) {

        try {
            for (size_t i = 0; i < n; ++i) {
                new (data_ + i) T();
                ++size_;
            }
        }
        catch (...) {
            cleanup();
            throw;
        }
    }

    // Destructor
    ~Vector() {
        cleanup();
    }

    // Copy constructor
    Vector(const Vector& other)
        : data_(allocate(other.capacity_)),
          size_(0),
          capacity_(other.capacity_) {

        try {
            for (size_t i = 0; i < other.size_; ++i) {
                new (data_ + i) T(other.data_[i]);
                ++size_;
            }
        }
        catch (...) {
            cleanup();
            throw;
        }
    }

    // Copy assignment
    Vector& operator=(const Vector& other) {

        if (this == &other)
            return *this;

        Vector temp(other);

        swap(data_, temp.data_);
        swap(size_, temp.size_);
        swap(capacity_, temp.capacity_);

        return *this;
    }

    // Move constructor
    Vector(Vector&& other) noexcept
        : data_(other.data_),
          size_(other.size_),
          capacity_(other.capacity_) {

        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    // Move assignment
    Vector& operator=(Vector&& other) noexcept {

        if (this == &other)
            return *this;

        cleanup();

        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;

        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;

        return *this;
    }

    // push_back(const T&)
    void push_back(const T& value) {

        if (size_ == capacity_) {

            size_t new_capacity =
                capacity_ == 0 ? 1 : capacity_ * 2;

            T* new_data = allocate(new_capacity);

            size_t constructed = 0;

            try {
                for (; constructed < size_; ++constructed) {
                    new (new_data + constructed)
                        T(std::move_if_noexcept(data_[constructed]));
                }
            }
            catch (...) {

                for (size_t i = 0; i < constructed; ++i)
                    new_data[i].~T();

                ::operator delete(new_data);

                throw;
            }

            destroy_elements();
            ::operator delete(data_);

            data_ = new_data;
            capacity_ = new_capacity;
        }

        new (data_ + size_) T(value);
        ++size_;
    }

    // push_back(T&&)
    void push_back(T&& value) {

        if (size_ == capacity_) {

            size_t new_capacity =
                capacity_ == 0 ? 1 : capacity_ * 2;

            T* new_data = allocate(new_capacity);

            size_t constructed = 0;

            try {
                for (; constructed < size_; ++constructed) {
                    new (new_data + constructed)
                        T(std::move_if_noexcept(data_[constructed]));
                }
            }
            catch (...) {

                for (size_t i = 0; i < constructed; ++i)
                    new_data[i].~T();

                ::operator delete(new_data);

                throw;
            }

            destroy_elements();
            ::operator delete(data_);

            data_ = new_data;
            capacity_ = new_capacity;
        }

        new (data_ + size_) T(std::move(value));
        ++size_;
    }

    // pop_back
    void pop_back() {

        if (size_ == 0)
            return;

        --size_;
        data_[size_].~T();
    }

    // clear
    void clear() {

        for (size_t i = 0; i < size_; ++i)
            data_[i].~T();

        size_ = 0;
    }

    // reserve
    void reserve(size_t new_capacity) {

        if (new_capacity <= capacity_)
            return;

        T* new_data = allocate(new_capacity);

        size_t constructed = 0;

        try {
            for (; constructed < size_; ++constructed) {
                new (new_data + constructed)
                    T(std::move_if_noexcept(data_[constructed]));
            }
        }
        catch (...) {

            for (size_t i = 0; i < constructed; ++i)
                new_data[i].~T();

            ::operator delete(new_data);

            throw;
        }

        destroy_elements();
        ::operator delete(data_);

        data_ = new_data;
        capacity_ = new_capacity;
    }

    // resize
    void resize(size_t new_size) {

        if (new_size < size_) {

            for (size_t i = new_size; i < size_; ++i)
                data_[i].~T();

            size_ = new_size;
            return;
        }

        if (new_size > capacity_)
            reserve(new_size);

        while (size_ < new_size) {
            new (data_ + size_) T();
            ++size_;
        }
    }

    size_t size() const {
        return size_;
    }

    size_t capacity() const {
        return capacity_;
    }

    bool empty() const {
        return size_ == 0;
    }

    T& operator[](size_t index) {
        return data_[index];
    }

    const T& operator[](size_t index) const {
        return data_[index];
    }

    T& front() {
        return data_[0];
    }

    T& back() {
        return data_[size_ - 1];
    }
};