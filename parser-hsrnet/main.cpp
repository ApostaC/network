#include <iostream>
#include <stdio.h>
#include "src/Threadpool.hpp"
#include "src/Buffer.hpp"

using namespace std;
using TP = Aposta::FixedThreadPool;

int main(int argc, char *argv[])
{
    TP pool(2);
    Buffer<int> buf;
    auto writer = [&buf](){
        int i = 0;
        while(true){
            buf.emplace(i++); 
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

    pool.barrier();

    return 0;
}
