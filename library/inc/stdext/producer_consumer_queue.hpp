#pragma once
#ifndef PCH
    #include <atomic>
    #include <condition_variable>
    #include <mutex>
    #include <optional>
    #include <queue>
    #include <stdexcept>
#endif

namespace stdext
{
    template <typename T>
    class producer_consumer_queue
    {
    public:
        explicit producer_consumer_queue(const std::size_t capacity): capacity_(capacity)
        {
            if (capacity == 0)
                throw std::invalid_argument("capacity must be greater than 0");
        }

        // Get the current size of the queue
        size_t size() const
        {
            std::lock_guard<std::mutex> lock(mutex_);
            return queue_.size();
        }

        // Check if the queue is empty
        bool empty() const
        {
            std::lock_guard<std::mutex> lock(mutex_);
            return queue_.empty();
        }

        // Check if the queue is full
        bool full() const
        {
            std::lock_guard<std::mutex> lock(mutex_);
            return queue_.size() == capacity_;
        }

        // Producer signals it starts adding items (increments active producer count)
        void begin_enqueue() noexcept { active_producers_.fetch_add(1, std::memory_order_relaxed); }

        // Producer signals it stops adding items (decrements active producer count)
        void end_enqueue()
        {
            active_producers_.fetch_sub(1, std::memory_order_relaxed);
            // Notify consumers in case the queue is empty and all producers are done
            std::lock_guard<std::mutex> lock(mutex_);
            not_empty_cv_.notify_all();
        }

        // Add an item to the queue, blocking if the queue is full
        void push(const T& item)
        {
            std::unique_lock<std::mutex> lock(mutex_);
            not_full_cv_.wait(lock, [this]() { return (queue_.size() < capacity_) || (active_producers_ != 0); });
            if (active_producers_ != 0)
            {
                queue_.push(item);
                not_empty_cv_.notify_one();
            }
        }

        // Add an item to the queue (move version), blocking if the queue is full
        void push(T&& item)
        {
            std::unique_lock<std::mutex> lock(mutex_);
            not_full_cv_.wait(lock, [this]() { return (queue_.size() < capacity_) || (active_producers_ != 0); });
            if (active_producers_ != 0)
            {
                queue_.push(std::move(item));
                not_empty_cv_.notify_one();
            }
        }

        // Remove and return an item from the queue, blocking if the queue is empty
        // Returns std::nullopt if all producers have finished and the queue is empty
        std::optional<T> try_pop()
        {
            std::unique_lock<std::mutex> lock(mutex_);
            not_empty_cv_.wait(lock, [this]() { return !queue_.empty() || (queue_.empty() && active_producers_ == 0); });

            // If queue is empty and no active producers, signal termination
            if (!queue_.empty() || (active_producers_ != 0))
            {
                auto item = std::move(queue_.front());
                queue_.pop();
                not_full_cv_.notify_one();
                return item;
            }

            return {};
        }

        template <typename _Container>
        std::pair<bool, bool> dequeue(_Container& container, const std::size_t max_amount)
        {
            std::unique_lock<std::mutex> lock(mutex_);
            not_empty_cv_.wait(lock, [this, max_amount]() { return queue_.size() >= max_amount || (active_producers_ == 0); });

            const auto count = std::min(max_amount, queue_.size());
            for (size_t i = 0; i < count; ++i)
            {
                container.push_back(std::move(queue_.front()));
                queue_.pop();
            }

            if (count > 0)
                not_full_cv_.notify_all(); // Notify all waiting producers since capacity has changed

            return {active_producers_ != 0, queue_.empty()};
        }

        // Dequeue all existing elements from the queue, non-blocking.
        template <typename _Container>
        void dequeue_all(_Container& container)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            while (!queue_.empty())
            {
                container.push_back(std::move(queue_.front()));
                queue_.pop();
            }

            not_full_cv_.notify_all(); // Notify waiting producers
        }

    private:
        std::queue<T> queue_;
        size_t capacity_;
        mutable std::mutex mutex_;
        std::condition_variable not_empty_cv_;
        std::condition_variable not_full_cv_;
        std::atomic_uint active_producers_; // Tracks the number of active producers
    };

}
