#include <atomic>
#include <cstddef>
#include <stdexcept>
#include <thread>
#include <utility>

template <typename T, std::size_t Capacity>
class MPSCQueue {
    static_assert(Capacity > 0, "Capacity must be > 0");

private:
    struct Slot {
        T value;
        std::atomic<std::size_t> sequence;

        Slot() : sequence(0) {}
    };

    Slot buffer_[Capacity];

    // Consumer-owned.
    std::atomic<std::size_t> head_{0};

    // Multiple producers modify this.
    std::atomic<std::size_t> tail_{0};

public:
    MPSCQueue() {
        // Initially slot i is available for position i.
        for (std::size_t i = 0; i < Capacity; ++i) {
            buffer_[i].sequence.store(i, std::memory_order_relaxed);
        }
    }

    bool try_push(const T& value) {
        std::size_t pos = tail_.load(std::memory_order_relaxed);

        while (true) {
            Slot& slot = buffer_[pos % Capacity];

            std::size_t sequence =
                slot.sequence.load(std::memory_order_acquire);

            std::intptr_t difference =
                static_cast<std::intptr_t>(sequence) -
                static_cast<std::intptr_t>(pos);

            if (difference == 0) {
                // Slot is available for us.
                if (tail_.compare_exchange_weak(
                        pos,
                        pos + 1,
                        std::memory_order_relaxed,
                        std::memory_order_relaxed)) {

                    // We uniquely own this slot now.
                    slot.value = value;

                    // Publish the data.
                    slot.sequence.store(
                        pos + 1,
                        std::memory_order_release);

                    return true;
                }

                // CAS failed.
                // 'pos' has been updated with the current tail.
                continue;
            }

            if (difference < 0) {
                // Queue is full.
                return false;
            }

            // Someone else advanced tail.
            pos = tail_.load(std::memory_order_relaxed);
        }
    }

    bool try_pop(T& value) {
        std::size_t pos =
            head_.load(std::memory_order_relaxed);

        Slot& slot = buffer_[pos % Capacity];

        std::size_t sequence =
            slot.sequence.load(std::memory_order_acquire);

        std::intptr_t difference =
            static_cast<std::intptr_t>(sequence) -
            static_cast<std::intptr_t>(pos + 1);

        if (difference != 0) {
            // Data hasn't been published yet.
            return false;
        }

        // The producer has published the value.
        value = std::move(slot.value);

        // Mark the slot as available for the
        // next cycle through the ring.
        slot.sequence.store(
            pos + Capacity,
            std::memory_order_release);

        head_.store(
            pos + 1,
            std::memory_order_relaxed);

        return true;
    }
};