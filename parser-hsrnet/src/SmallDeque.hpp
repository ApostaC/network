#ifndef _SMALLDEQUE_HPP_
#define _SMALLDEQUE_HPP_

#include <stdlib.h>
#include <stdexcept>

namespace{  // anonymous namespace

template<typename T>
struct stab
{
    public:
        /**
         * default slab size in BYTES
         * total 64KB
         * try to fit into L2 cache
         */
        static constexpr size_t ALLOCATION_SIZE = 1<<16;
        static constexpr size_t OBJECT_COUNT = ALLOCATION_SIZE/sizeof(T);

    private:
        bool is_allocated;
        size_t start_index;
        T * raw_pointer;

    public:
        stab(size_t sp, bool allocate_later = true) noexcept
            :is_allocated(false),start_index(sp),raw_pointer(nullptr)
        {
            /* allocate the memory */
            if(allocate_later == false)
                allocate();
        }

        void allocate()
        {
            if(is_allocated) return;
            is_allocated = true;
            raw_pointer = new T[OBJECT_COUNT];
        }

        /**
         * Methods:
         * operator[]
         * startIndex()
         */

        size_t startIndex(){return start_index;}
        T & operator[](size_t index)
        {
            if(index >= OBJECT_COUNT || index < 0)
                throw std::overflow_error("Index overflow inside ::stab");
            return raw_pointer[index];
        }

};

};// anonymous namespace

template<typename T>
class SmallDeque
{
    using stab = ::stab<T>;
    public:
        /**
         * front
         * pop_front
         * emplace_back
         * push_back
         * size
         */

    private:
        /**
         * allo:q`
         */
};





#endif
