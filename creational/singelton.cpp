#include <bits/stdc++.h>
using namespace std;

class EagerInitialization{
    private:
        int data;
        static EagerInitialization instance;
        EagerInitialization() : data(10){}
    public:
        EagerInitialization (const EagerInitialization&) = delete;
        EagerInitialization& operator=(const EagerInitialization& ) = delete;
        EagerInitialization (EagerInitialization &&) = delete;
        EagerInitialization& operator=(EagerInitialization&& ) = delete;

        static EagerInitialization& getInstance(){
            return instance;
        }
};

class LazyInitialization{
    private:
        int data;
        LazyInitialization() : data(10) {}
    public:
        LazyInitialization (const LazyInitialization&) = delete;
        LazyInitialization& operator= (const LazyInitialization&) = delete;
        LazyInitialization (LazyInitialization&&) = delete;
        LazyInitialization& operator=(LazyInitialization&&) = delete;

        static LazyInitialization& getInstance() {
            static LazyInitialization instance;
            return instance;
        }
};

class Singleton {

private:

    Singleton() {}

public:

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static Singleton& getInstance()
    {
        static Singleton instance;
        return instance;
    }

    void display()
    {
        cout << "Singleton Instance\n";
    }
};

class Singleton_pointer {

private:

    static Singleton_pointer* instance;

    Singleton_pointer() {}

public:

    Singleton_pointer(const Singleton_pointer&) = delete;
    Singleton_pointer& operator=(const Singleton_pointer&) = delete;

    static Singleton_pointer* getInstance()
    {
        if (instance == nullptr)
            instance = new Singleton_pointer();

        return instance;
    }

    void display()
    {
        cout << "Singleton Instance\n";
    }
};

Singleton_pointer* Singleton_pointer::instance = nullptr;


int main(){
    EagerInitialization &eg = EagerInitialization::getInstance();
    EagerInitialization& eg2 = EagerInitialization::getInstance();
    return 0;
}

