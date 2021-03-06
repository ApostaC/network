#include <iostream>
#include <cassert>
#include <stdio.h>
#include "src/Threadpool.hpp"
#include "src/SmallDeque.hpp"
#include "src/Buffer.hpp"
#include "src/Reader.h"

using namespace std;
using TP = Aposta::FixedThreadPool;

void testBuffer()
{
    Buffer<int> buf;
    TP pool(2);
    auto writer = [&buf](){
        int i = 0;
        while(true){
            if(i%2)
                buf.emplace(i++); 
            else buf.push(i++);
            i%=2000;
        }
    };
    auto reader = [&buf](){
        std::this_thread::sleep_for(1s);
        while(true){
            auto num = buf.next();
            printf("%d ",num);
            if(num % 200 == 0)
            {
                printf("\n");
                std::this_thread::sleep_for(1s);
            }
        }
    };

    buf.setStopBound(1000);
    buf.setStartBound(500);

    pool.enqueue(writer);
    pool.enqueue(reader);

    //pool.barrier();

}

void testSmallDeque()
{
    struct A
    {
        int id;
        char c;
        A(int i, char cc):id(i),c(cc){}
        A() = default;
        A(A && _) = default;
        A(const A &) = default;
    };

    SmallDeque<A> deq(100);
    for(int i=0;i<10;i++) deq.emplace_back(i,i+'0');
    for(int i=0;i<10;i++) deq.push_back(A(i,i+'a'));
    auto s = deq.size();
    for(size_t i=0;i<s;i++)
    {
        A a {std::move(deq.front())};
        deq.pop_front();
        std::cout<<a.c<<" "<<a.id<<std::endl; 
    }

}

REG_PCAP_READER_NO_PRED("test", 1);
extern BufferProvider provider;
void testReader(const char *filename)
{
    REG_NEW_BUFFER("test");
    TP pool(2);
    auto buf = provider.GetBufferByName("test");
    assert(buf);
    int count = 0;
    auto reader = [&](){
            while(buf->isRunning()){buf->next();}};
    pool.enqueue(reader);
    pool.enqueue(reader_1,filename);
}

int main(int argc, char *argv[])
{
    //testSmallDeque();
    //testBuffer();
    testReader(argv[1]);
}
