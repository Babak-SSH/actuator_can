# Actuator CAN
C++ Driver for **Mini Chetaah-type** actuators

sending commands to motors using SocketCAN interface which is natively supported in linux. (can be used with CAN to USB adapters)

send-reply frequency is set to 6KHz in each command loop (sending commands to 12 motors 3 on each actuator) in test programs, however depending on hardware capabilities it can work in faster rates.

## Dependencies
- linux SocketCAN
- [LCM](https://github.com/lcm-proj/lcm) (used to log and review sent and recived data)

## Build
```bash
mkdir build
cd build
cmake ..
make
sudo make install # optional
```

## Tests
- [single_loop](./test/single_loop.cpp): commands for all actuators are handled in one loop.
- [parallel_loops](./test/parallel_loops.cpp): command loop for each actuator runs in a parallel thread at same time.

to test it with virtual CAN [CANdevStudio](https://github.com/GENIVI/CANdevStudio?tab=readme-ov-file#candevstudio-without-can-hardware) can be used.