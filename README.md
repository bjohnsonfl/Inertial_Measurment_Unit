# IMU
Early workings of an IMU/AHRS
## About
An embedded program to read raw sensor data, process them via sensor fusion, and return the orientation of the device.

An Atmel Xmega 128a1u MCU communicates with a MPU 9250 inertial measturement unit over SPI. The IMU has 9 degrees of freedom from its 3 axis Accelerometer, Gyroscope, and Magnetometer, and while it does have its own sensor fusion module, I will bypass it due to wanting to implement my own. While ideally the output of the AHRS could be inputs to a PID control system, I am displaying them on my computer via UART. 

## Things to do

- [x] SPI interface
- [x] USART interface
- [ ] MPU_9250 Configuration
- [ ] MPU_9250 Read raw Accelerometer and Gyroscope data
- [ ] MPU_9250 Configure internal I2C Master
- [ ] MPU_9250 Read raw Magnetometer data from AK8963c via I2C Master
- [ ] MPU_9250 Interrupt based data collection
- [ ] Accelerometer and Gyroscope calibration
- [ ] Magnetometer hard iron calibration
- [ ] Magnetometer soft iron calibration

The next three overlap but follow the general progression
- [ ] Accelerometer + Gyroscope Sensor Fusion
- [ ] Accelerometer + Gyroscope + Magnetometer Sensor fusion
- [ ] [Madgwick Filter](https://courses.cs.washington.edu/courses/cse474/17wi/labs/l4/madgwick_internal_report.pdf) (Quaternion based) 
