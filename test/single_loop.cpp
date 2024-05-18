#include <ctime>
#include <ratio>
#include <unistd.h>
#include <iostream>
#include <chrono>
#include <thread>

#include "actuator.hpp"


/// motor indexes
#define m1  0x01
#define m2  0x02
#define m3  0x03
#define m4  0x04
#define m5  0x05
#define m6  0x06
#define m7  0x07
#define m8  0x08
#define m9  0x09
#define m10 0x0A
#define m11 0x0B
#define m12 0x0C

Actuator leg1("can0"), leg2("can2"), leg3("can3"), leg4("can1");

int main() {
    /// enable motors
    auto enableStart = std::chrono::high_resolution_clock::now();
    leg1.enable(m4);
    leg1.enable(m5);
    leg1.enable(m6);

    leg2.enable(m7);
    leg2.enable(m8);
    leg2.enable(m9);

    leg3.enable(m1);
    leg3.enable(m2);
    leg3.enable(m3);

    leg4.enable(m10);
    leg4.enable(m11);
    leg4.enable(m12);
    auto enableEnd = std::chrono::high_resolution_clock::now();
    std::cout<< "enable time: " << std::chrono::duration<double, std::milli>(enableEnd-enableStart).count() << " millis" << std::endl;

    /// delay before starting the main program
    sleep(1);


    double totalTime = 0;
    int loopCount = 0; 
    std::chrono::high_resolution_clock::time_point t_start;
    std::chrono::high_resolution_clock::time_point t_end;
    std::chrono::high_resolution_clock::time_point t_pre;

    t_pre = std::chrono::high_resolution_clock::now();
    auto loopStart = std::chrono::high_resolution_clock::now();
    while (std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now()-loopStart).count() < 1000) {
        t_start = std::chrono::high_resolution_clock::now();
        
        leg1.command(m4, 0,0,0,0,0);
        leg2.command(m7, 0,0,0,0,0);
        leg3.command(m1, 0,0,0,0,0);
        leg4.command(m10, 0,0,0,0,0);

        leg1.command(m5, 0,0,0,0,0);
        leg2.command(m8, 0,0,0,0,0);
        leg3.command(m2, 0,0,0,0,0);
        leg4.command(m11, 0,0,0,0,0);

        leg1.command(m6, 0,0,0,0,0);
        leg2.command(m9, 0,0,0,0,0);
        leg3.command(m3, 0,0,0,0,0);
        leg4.command(m12, 0,0,0,0,0);

        t_end = std::chrono::high_resolution_clock::now();
        totalTime += std::chrono::duration<double, std::milli>(t_end-t_start).count();
        loopCount++;

        /// fixing each command loop to 2 millis by forcing it to sleep in an empty loop
        /// (presumably with correct hardware this loop can run in very short time) 
        // std::this_thread::sleep_for(std::chrono::microseconds(1900));    // const delay time
        while(std::chrono::duration<double, std::micro>(t_end-t_pre).count() < 2000) {    // dynamic delay time   
            t_end = std::chrono::high_resolution_clock::now();
        }
        t_pre = std::chrono::high_resolution_clock::now();
    } 

    /// results
    std::cout << "total time: " << std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now()-loopStart).count() << std::endl;
    std::cout << "tot: loops: " << loopCount << "\n1: total commands: " << loopCount*12 << std::endl;
    std::cout << "tot: average time for each loop: " << totalTime/loopCount << " millis" << std::endl;

    std::cout << "leg1 response count: " << leg1.responseCount << std::endl;
    std::cout << "leg2 response count: " << leg2.responseCount << std::endl;
    std::cout << "leg3 response count: " << leg3.responseCount << std::endl;
    std::cout << "leg4 response count: " << leg4.responseCount << std::endl;
    std::cout << "total response count: " << leg1.responseCount+leg1.responseCount+leg1.responseCount+leg1.responseCount << std::endl;

    /// disable motors
    leg1.disable(m4);
    leg1.disable(m5);
    leg1.disable(m6);
    leg2.disable(m7);
    leg2.disable(m8);
    leg2.disable(m9);
    leg3.disable(m1);
    leg3.disable(m2);
    leg3.disable(m3);
    leg4.disable(m10);
    leg4.disable(m11);
    leg4.disable(m12);

    return 0;
}