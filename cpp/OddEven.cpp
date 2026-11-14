#include <bits/stdc++.h>
#include <thread>

using namespace std;

class OddEven{
    public:
        void printEven(){
            while(true){
                std::unique_lock<mutex> lc(mx);
                even_cv.wait(lc,[&](){
                    return (counter%2 == 0) || (counter>limit);
                });
                if(counter>limit)
                    break;
                cout<<counter<<" ";
                counter++;
                lc.unlock();
                odd_cv.notify_one();
            }
            odd_cv.notify_one();
        }
        void printOdd(){
            while(true){
                std::unique_lock<mutex> lc(mx);
                odd_cv.wait(lc,[&](){
                    return (counter%2 != 0) || (counter>limit);
                });
                if(counter>limit)
                    break;
                cout<<counter<<" ";
                counter++;
                lc.unlock();
                even_cv.notify_one();
            }
            even_cv.notify_one();

        }
    OddEven(int lim) : limit(lim), counter(1) {}
    private:
        int limit;
        condition_variable odd_cv;
        condition_variable even_cv;
        std::mutex mx;
        int counter;
};

int main(){
    OddEven obj(100);
    thread t1(&OddEven::printOdd,&obj);
    thread t2(&OddEven::printEven,&obj);
    t1.join();
    t2.join();
    return 0;
}