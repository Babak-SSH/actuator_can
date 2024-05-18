#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <linux/can.h>
#include <string>

#include <lcm/lcm-cpp.hpp>

#include "SocketCAN.h"

#include "actuatorlcm/actuator_response_t.hpp"


class Actuator {
    private:
        SocketCAN* adapter;
        lcm::LCM lcm;
        actuatorlcm::actuator_response_t DATA;
    public:
        int responseCount;
        double timeout;
        float *result;
        int s;
        int __data[8] = {0,0,0,0,0,0,0,0};
        float __pose_shift;
        struct can_frame frame;
        struct can_filter rfilter[1];
        Actuator(std::string can_index);
        void rx_handler(can_frame_t*);
        void enable(int id);
        void disable(int id);

        void zero(int id);
        void command(int id,double p_d,double v_d,double kp,double kd,double ff);
        float * unpack_data(can_frame_t frame);
        float * pack_data(double p_d,double v_d,double kp,double kd,double ff);
        void static decToBinary(int n, int* binaryNum);
        int static binaryToDec(int* binaryNum);
};

#endif