#pragma once
#include "general.h"

namespace dlib {

    template<typename T>
    class MQueue {// queue, safe for multithread operations
    private:
        std::mutex mx;
        std::queue<T> c;

    public:
        MQueue() = default;
        MQueue& operator=(const MQueue<T>& that) = delete;
        MQueue(const MQueue<T>& that) = delete;
        ~MQueue() { clear(); }
        T& front() {
            std::scoped_lock lk(mx);
            return c.front();
        }

        void push(const T& x) {
            std::scoped_lock lk(mx);
            c.push(x);
        }

        void pop() {
            std::scoped_lock lk(mx);
            c.pop();
        }

        auto size() {
            std::scoped_lock lk(mx);
            return c.size();
        }

        bool empty() {
            std::scoped_lock lk(mx);
            return c.empty();
        }
        void clear() {
            std::scoped_lock lk(mx);
            while (!c.empty()) c.pop();
        }
    };
}// namespace dlib