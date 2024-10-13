#pragma once
#ifndef PCH
    #include <functional>
    #include <memory>
    #include <mutex>
    #include <queue>
#endif

namespace stdext
{
    template <typename T>
    class object_pool
    {
    public:
        using shared_ptr = std::shared_ptr<T>;
        using lock_guard = std::lock_guard<std::mutex>;

        object_pool() = default;
        object_pool(object_pool&&) = default;
        object_pool(const object_pool&) = default;
        object_pool(size_t n, std::function<shared_ptr()> create_func) { initialize(n, std::move(create_func)); }

        // Populate the queue_ with n objects
        void initialize(size_t n, std::function<shared_ptr()> create_func)
        {
            lock_guard lock(mutex_);
            for (; n > 0; --n)
            {
                queue_.push(create_func());
            }
        }

        // Get an object from the pool
        shared_ptr try_acquire()
        {
            lock_guard lock(mutex_);
            if (queue_.empty())
            {
                return {};
            }

            auto item = queue_.front();
            queue_.pop();
            return item;
        }

        // Get an object from the pool
        shared_ptr acquire()
        {
            lock_guard lock(mutex_);
            if (queue_.empty())
            {
                throw std::runtime_error("failed to aquire object from poll");
            }

            auto item = queue_.front();
            queue_.pop();
            return item;
        }

        // Return an object to the queue
        void release(shared_ptr obj)
        {
            lock_guard lock(mutex_);
            queue_.push(std::move(obj));
        }

        void operator=(const object_pool&) = delete;
        void operator=(object_pool&& item)
        {
            if (&item != this)
                queue_ = std::move(item.queue_);
        }

    private:
        std::queue<shared_ptr> queue_;
        mutable std::mutex mutex_;
    };
}
