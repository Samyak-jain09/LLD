#include <bits/stdc++.h>
using namespace std;

class RateLimiter
{
private:
    double capacity_;
    double tokens_;
    double refill_rate_;
    std::chrono::steady_clock::time_point last_refill_;
    std::mutex mutex_;

public:
    RateLimiter(double capacity, double refill_rate)
        : capacity_(capacity),
          tokens_(capacity),
          refill_rate_(refill_rate),
          last_refill_(std::chrono::steady_clock::now())
    {
        if (capacity <= 0 || refill_rate <= 0)
            throw std::invalid_argument("Cannot be negative");
    }

    bool allow()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        auto now = std::chrono::steady_clock::now();
        auto elapsed = now - last_refill_;
        double elapsed_seconds =
            std::chrono::duration<double>(elapsed).count();
        double new_tokens =
            elapsed_seconds * refill_rate_;
        tokens_ = std::min(
            capacity_,
            tokens_ + new_tokens);
        last_refill_ = now;
        if (tokens_ >= 1.0)
        {
            tokens_ -= 1.0;
            return true;
        }
        return false;
    }
};