#include <bits/stdc++.h>
#include <thread>

using namespace std;

class FizzBuzz{
    public:
        FizzBuzz(int lim) : limit(lim), counter(1)  {}
        void printfizzbuzz(){
            while(true){
                unique_lock<mutex> lc(mx);
                cv_.wait(lc, [&](){
                    return (counter%3 == 0 && counter%5 == 0) || counter>limit;
                });
                if(counter>limit)
                    break;
                cout<<"fizzbuzz"<<" ";
                counter++;
                lc.unlock();
                cv_.notify_all();
            }
            cv_.notify_all();
        }
        void printfizz(){
            while(true){
                unique_lock<mutex> lc(mx);
                cv_.wait(lc, [&](){
                    return (counter%3 == 0 && counter%5 != 0) || counter>limit;
                });
                if(counter>limit)
                    break;
                cout<<"fizz"<<" ";
                counter++;
                lc.unlock();
                cv_.notify_all();
            } 
            cv_.notify_all();
        }
        void printbuzz(){
            while(true){
                unique_lock<mutex> lc(mx);
                cv_.wait(lc, [&](){
                    return (counter%3 != 0 && counter%5 == 0) || counter>limit;
                });
                if(counter>limit)
                    break;
                cout<<"buzz"<<" ";
                counter++;
                lc.unlock();
                cv_.notify_all();
            } 
            cv_.notify_all();
        }
        void printnumber(){
            while(true){
                unique_lock<mutex> lc(mx);
                cv_.wait(lc, [&](){
                    return (counter%3 != 0 && counter%5 != 0) || counter>limit;
                });
                if(counter>limit)
                    break;
                cout<<counter<<" ";
                counter++;
                lc.unlock();
                cv_.notify_all();
            } 
            cv_.notify_all();
        }
    private:
        int limit;
        int counter;
        condition_variable cv_;
        mutex mx;
};

int main(){
    FizzBuzz obj(15);
    thread t1(&FizzBuzz::printbuzz, &obj);
    thread t2(&FizzBuzz::printfizz, &obj);
    thread t3(&FizzBuzz::printnumber, &obj);
    thread t4(&FizzBuzz::printfizzbuzz, &obj);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    return 0;
}