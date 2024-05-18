/**
 * @file
 * This file implements functions to receive
 * and transmit CAN frames via SocketCAN.
 */

#ifndef MINGW

#include "SocketCAN.h"

#include <stdio.h>
// strncpy
#include <string.h>
// close
#include <unistd.h>
// socket
#include <sys/socket.h>
// SIOCGIFINDEX
#include <sys/ioctl.h>


SocketCAN::SocketCAN(std::function<void(can_frame_t*)> reception_handler)
   :CANAdapter(reception_handler),
    sockfd(-1),
    receiver_thread_id(0)
{
    adapter_type = ADAPTER_SOCKETCAN;
    printf("SocketCAN adapter created.\n");
}


SocketCAN::~SocketCAN()
{
    printf("Destroying SocketCAN adapter...\n");
    if (this->is_open())
    {
        this->close();
    }
}


void SocketCAN::open(const char* interface)
{

    // Setting can interface
    __can_conf_1 = "sudo ip link set " + interface + " type can bitrate 1000000";
    __can_conf_2 = "sudo ifconfig " + interface + " txqueuelen 65536";
    __can_conf_3 = "sudo ifconfig " + interface + " up";
    __can_conf_4 = "sudo ifconfig " + interface + " down";


    system(this->__can_conf_4.c_str());
    system(this->__can_conf_1.c_str());
    system(this->__can_conf_2.c_str());
    system(this->__can_conf_3.c_str());

    // Request a socket
    sockfd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (sockfd == -1)
    {
        printf("Error: Unable to create a CAN socket\n");
        return;
    }
    printf("Created CAN socket with descriptor %d.\n", sockfd);

    // Get the index of the network interface
    strncpy(if_request.ifr_name, interface, IFNAMSIZ);
    if (ioctl(sockfd, SIOCGIFINDEX, &if_request) == -1)
    {
        printf("Unable to select CAN interface %s: I/O control error\n", interface);

        // Invalidate unusable socket
        close();
        return;
    }
    printf("Found: %s has interface index %d.\n", interface, if_request.ifr_ifindex);

    // Bind the socket to the network interface
    addr.can_family = AF_CAN;
    addr.can_ifindex = if_request.ifr_ifindex;
    int rc = bind(
        sockfd,
        reinterpret_cast<struct sockaddr*>(&addr),
        sizeof(addr)
    );
    if (rc == -1)
    {
        printf("Failed to bind socket to network interface\n");

        // Invalidate unusable socket
        close();
        return;
    }
    printf("Successfully bound socket to interface %d.\n", if_request.ifr_ifindex);

    // Start a separate, event-driven thread for frame reception
    start_receiver_thread();
}

void SocketCAN::close()
{
    terminate_receiver_thread = true;

    if (!is_open())
        return;

    // Close the file descriptor for our socket
    ::close(sockfd);
    sockfd = -1;

    printf("CAN socket destroyed.\n");
}

bool SocketCAN::is_open()
{
    return (sockfd != -1);
}
