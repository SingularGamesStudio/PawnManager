#pragma once
#include "general.h"

namespace dlib {

    template<typename T>
    class BlockingQueue {// queue, safe for multithread operations
    private:
        std::mutex mx;
        std::queue<T> c;

    public:
        BlockingQueue() = default;
        BlockingQueue& operator=(const BlockingQueue<T>& that) = delete;
        BlockingQueue(const BlockingQueue<T>& that) = delete;
        ~BlockingQueue() { clear(); }
        T& front() {
            std::scoped_lock lock_guard(mx);
            return c.front();
        }

        void push(const T& x) {
            std::scoped_lock lock_guard(mx);
            c.push(x);
        }

        void pop() {
            std::scoped_lock lock_guard(mx);
            c.pop();
        }

        auto size() {
            std::scoped_lock lock_guard(mx);
            return c.size();
        }

        bool empty() {
            std::scoped_lock lock_guard(mx);
            return c.empty();
        }
        void clear() {
            std::scoped_lock lock_guard(mx);
            while (!c.empty()) c.pop();
        }
    };
}// namespace dlib