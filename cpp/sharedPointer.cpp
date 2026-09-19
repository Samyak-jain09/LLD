#include <bits/stdc++.h>
using namespace std;


template <typename T>
class SharedPointer{
    private:
        T* obj_;
        int* cnt_;
        void __cleanup() {
            if (cnt_ != nullptr) {
                --(*cnt_);

                if (*cnt_ == 0) {
                    delete obj_;
                    delete cnt_;
                }
            }
        }
    public:
        SharedPointer(){
            obj_ = nullptr;
            cnt_ = nullptr;
        }
        SharedPointer(T* val) : obj_(val), cnt_(new int(1)){}
        ~SharedPointer(){
            __cleanup();
        }
        //copy constructor
        SharedPointer (const SharedPointer& other){
            this->obj_ = other.obj_;
            this->cnt_ = other.cnt_;
            if(cnt_ != nullptr)
                (*cnt_)++;
        }
        //copy assignment
        SharedPointer& operator=(const SharedPointer& other){
            if(this == &other)
                return *this;
            __cleanup();
            this->obj_ = other.obj_;
            this->cnt_ = other.cnt_;
            if(cnt_!=nullptr)
                (*cnt_)++;
            return *this;
        }
        //move constructor
        SharedPointer (SharedPointer&& other) noexcept{
            this->obj_ = other.obj_;
            this->cnt_ = other.cnt_;
            other.cnt_ = nullptr;
            other.obj_ = nullptr;
        }
        //move assignment
        SharedPointer& operator=(SharedPointer&& other) noexcept{
            if(this ==  &other)
                return *this;
            __cleanup();
            this->obj_ = other.obj_;
            this->cnt_ = other.cnt_;
            other.cnt_ = nullptr;
            other.obj_ = nullptr;
            return *this;
        }
        T& operator*(){
            return *obj_;
        }
        T* operator->(){
            return obj_;
        }
        bool operator() const{
            if(obj_ == nullptr)
                return false;
            return true;
        }
        T* get() const{
            return obj_;
        }
        void reset(){
            __cleanup();
            obj_ = nullptr;
            cnt_ = nullptr;
        }
        void reset(T* ptr){
            __cleanup();
            obj_ = ptr;
            cnt_ = new int(1);
        }
};