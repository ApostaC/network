#ifndef _BUFFER_HPP_
#define _BUFFER_HPP_

#include <deque>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>

template<typename T> class SmallDeque;

template<typename T>
class Buffer
{
    public:
        T next();
        void emplace(T && t_);
        void push(const T & t);

        void setStopBound(size_t s){this->stop_bound = s;}
        size_t getStopBound(){return this->stop_bound;}

        void setStartBound(size_t s){this->start_bound = s;}
        size_t getStartBound(){return this->start_bound;} 
    private:

        size_t stop_bound = 65536;
        size_t start_bound = 4096;

        std::deque<T> data;
        
        std::mutex write_mutex;
        std::condition_variable write_cond;

        std::mutex read_mutex;
        std::condition_variable read_cond;
};

template<typename T>
T Buffer<T>::next()
{
    std::unique_lock<std::mutex> read_lock(this->read_mutex);
    read_cond.wait(read_lock,[this](){return this->data.size();});
    T ret{std::move(this->data.front())};
    this->data.pop_front();
    if(this->data.size()<start_bound) 
        std::cout<<"HERE!! "<<this->data.size()<<std::endl;
    return ret;
}

template<typename T>
void Buffer<T>::push(const T & t)
{
    /**
     * race condition here!
     * However, it's the single reader/writer Buffer
     * So we don't really care about the actual size
     * of data queue, so we use data.size() here
     * directly instead of using somehow locking 
     */
    if(this->data.size() > stop_bound)
    {
        std::unique_lock<std::mutex> write_lock(this->write_mutex);
        std::cout<<"reach stop bound!"<<std::endl;
        this->write_cond.wait(write_lock,
                [this](){return this->data.size() < this->start_bound;});

        std::cout<<"reach start bound!"<<std::endl;
    }
    this->data.push_back(t);
}


template<typename T>
void Buffer<T>::emplace(T && t)
{
    /**
     * race condition here!
     * just like in push();
     */
    if(this->data.size() > stop_bound)
    {
        std::unique_lock<std::mutex> write_lock(this->write_mutex);
        std::cout<<"reach stop bound!"<<std::endl;
        this->write_cond.wait(write_lock,
                [this](){
                    //std::cout<<"val = "<<this->data.size()<<" start_bound = "<<this->start_bound;
                    return false;
                    //return this->data.size() < this->start_bound;
                });

        std::cout<<"reach start bound!"<<std::endl;
    }
    this->data.emplace_back(t);
}
#endif
