#include <bits/stdc++.h>
using namespace std;

template <typename T>
class SharedPtr{
    private:
        T* ptr = nullptr;
        int* count = nullptr;
        void cleanup(){
            if(count == nullptr)
                return;
            (*count)--;
            if((*count) == 0){
                if(ptr!=nullptr){
                    delete ptr;
                }
                delete count;
            }
        }
    public:
        SharedPtr(){
            this->ptr = nullptr;
            count = new int(0);
        }
        SharedPtr(T* ptr){
            if(ptr == nullptr)
                return; 
            this->ptr = ptr;
            count = new int(1);
        }
        ~SharedPtr(){
            cleanup();
        }
        SharedPtr(const SharedPtr& other){
            this->ptr = other.ptr;
            this->count = other.count;
            if(other.ptr!=nullptr){
                (*this->count)++;
            }
        }
        SharedPtr& operator=(const SharedPtr& other){
            cleanup();
            this->ptr = other.ptr;
            this->count = other.count;
            if(other.ptr!=nullptr){
                (*this->count)++;
            }
            return *this;
        }
        SharedPtr(SharedPtr&& other){
            this->ptr = other.ptr;
            this->count = other.count;
            other.ptr = nullptr;
            other.count = nullptr;
        }
        SharedPtr& operator=(SharedPtr&& other){
            cleanup();
            this->ptr = other.ptr;
            this->count = other.count;
            other.ptr = nullptr;
            other.count = nullptr;
            return *this;
        }
        T& operator*(){
            return this->ptr;
        }
        T* operator->(){
            return this->ptr;
        }
        T* get() const{
            return this->ptr;
        }
};

