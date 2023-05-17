#pragma once
#include "general.h"

namespace dlib {

    template<typename T, typename U>
    class BlockingMap {// map, safe for multithread operations
    private:
        std::mutex mx;
        std::unordered_map<T, U> mp;
    public:
        class iterator {
            friend BlockingMap;
        private:
            std::mutex* m_ptr;
            typename std::unordered_map<T, U>::iterator it;
        public:
            using value_type = std::pair<const T, U>;
            using pointer = value_type*;
            using reference = value_type&;
            using iterator_category = std::bidirectional_iterator_tag;
            using difference_type = std::ptrdiff_t; // it just needs this

            iterator() {}
            iterator(std::mutex* nmx, typename std::unordered_map<T, U>::iterator nit) :
                m_ptr(nmx), it(nit) {}
            iterator& operator=(const iterator& that) { // NOLINT
                it = that.it;
                m_ptr = that.m_ptr;
                return *this;
            }
            iterator& operator++() {
                ++it;
                return *this;
            }
            iterator& operator--() {
                --it;
                return *this;
            }
            iterator operator++(int) {
                iterator tmp(*this);
                ++(*this);
                return tmp;
            }
            iterator operator--(int) {
                iterator tmp(*this);
                --(*this);
                return tmp;
            }

            bool operator==(const iterator& that) const {
                return (it == that.it);
            }

            reference operator*() {
                std::scoped_lock lock_guard(*m_ptr);
                return static_cast<reference>(*it);
            }
            pointer operator->() {
                std::scoped_lock lock_guard(*m_ptr);
                return &this->operator*();
            }
        };

        BlockingMap() = default;
        BlockingMap& operator=(const BlockingMap& that) = delete;
        BlockingMap(const BlockingMap& that) = delete;
        ~BlockingMap() { clear(); }
        U& operator[](const T& arg) {
            std::scoped_lock lock_guard(mx);
            return mp[arg];
        }
        const U& operator[](const T& arg) const {
            std::scoped_lock lock_guard(mx);
            return mp[arg];
        }
        void erase(iterator it) {
            std::scoped_lock lock_guard(mx);
            if(it.it != mp.end())
                mp.erase(it.it);
        }
        void clear() {
            std::scoped_lock lock_guard(mx);
            mp.clear();
        }
        iterator begin() {
            return iterator(&mx, mp.begin());
        }

        iterator end() {
            return iterator(&mx, mp.end());
        }
    };
}// namespace dlib