#include <bits/stdc++.h>
using namespace std;

template <typename T>
class UniquePtr{
    private:
        T* ptr;
        void __cleanup(){
            if(ptr!=nullptr)
                delete ptr;
        }
    public:
        UniquePtr() : ptr(nullptr) {}
        UniquePtr(T* val) : ptr(val) {}
        ~UniquePtr(){
            __cleanup();
        }
        UniquePtr (const UniquePtr& other) = delete;
        UniquePtr& operator=(const UniquePtr& other) = delete;
        UniquePtr (UniquePtr&& other) noexcept{
            this->ptr = other.ptr;
            other.ptr = nullptr;
        }
        UniquePtr& operator=(UniquePtr&& other) noexcept{
            if(this == &other)
                return *this;
            __cleanup();
            this->ptr = other.ptr;
            other.ptr = nullptr;
            return *this;
        }
        T& operator*() const{
            return *ptr;
        }
        T* operator->() const{
            return ptr;
        }
        T* get() const{
            return ptr;
        }
        explicit operator bool() const {
            return ptr != nullptr;
        }
        T* release() {
            T* temp = ptr;
            ptr = nullptr;
            return temp;
        }
        void reset(){
            __cleanup();
            ptr = nullptr;
        }
        void reset(T* ptr){
            if(this->ptr == ptr)
                return;
            __cleanup();
            this->ptr = ptr;
        }
};