#include <bits/stdc++.h>
using namespace std;

class Scheduler{
    using Task = std::function<void()>;
    using TimePoint = std::chrono::steady_clock::time_point;
    private:
        struct ScheduledTask{
            size_t id;
            Task task;
            std::chrono::steady_clock::time_point deadline;
            ScheduledTask(size_t id, Task task, TimePoint deadline): id(id), task(std::move(task)), deadline(deadline) {}
        }; 
        struct Compare{
            bool operator()(const ScheduledTask &a, const ScheduledTask &b){
                return a.deadline>b.deadline;
            }
        };
        priority_queue<ScheduledTask, vector<ScheduledTask>, Compare> tasks_;
        std::mutex mutex_;
        std::condition_variable cv_;
        std::thread worker_;
        size_t nextId_ = 0;
        std::unordered_set<size_t> cancelled_;
        bool stopping_ = false;
        void workerLoop(){
            //sleep o condtional variable
            //if task is there, sleep til deadline is reached
            //execute task when deadline is reached
            //if another task got added in between, wait and execute it if deadline is less
            Task task;
            while(true){
                {
                    std::unique_lock<std::mutex> lc_(mutex_);
                    cv_.wait(lc_,[this](){
                        return stopping_ || !tasks_.empty();
                    });
                    if(stopping_)
                        return;
                    auto newDeadline = tasks_.top().deadline;
                    cv_.wait_until(lc_,newDeadline,[&,newDeadline](){
                        return stopping_ || (!tasks_.empty() && tasks_.top().deadline < newDeadline);
                    });
                    if(stopping_)
                        return;
                    if(tasks_.top().deadline<newDeadline)
                        continue;
                    task = std::move(tasks_.top().task);
                    size_t id = tasks_.top().id;
                    tasks_.pop();
                    if (cancelled_.count(id)) {
                        cancelled_.erase(id);
                        continue;
                    }
                }
                task();
            } 
        }
    public:
        Scheduler(){
            worker_ = std::thread(&Scheduler::workerLoop,this);
        }
        ~Scheduler(){
            {
                std::lock_guard<std::mutex> lc_(mutex_);
                stopping_ = true;
            }
            cv_.notify_one();
            if(worker_.joinable())
                worker_.join();
        }
        size_t schedule(Task task, std::chrono::milliseconds delay){
            //create a scheduled task
            //push task to queue
            //notify one
            auto now = std::chrono::steady_clock::now();
            auto newDeadline = now+delay;
            size_t id;
            {
                std::lock_guard<std::mutex> lc(mutex_);
                if(stopping_)
                    return;
                id = nextId_++;
                tasks_.emplace(id, std::move(task), newDeadline);
            }
            cv_.notify_one();
            return id;
        }

        bool cancel(size_t id) {
            std::lock_guard<std::mutex> lock(mutex_);
            if(stopping_)
                return false;
            cancelled_.insert(id);
            return true;
        }

};