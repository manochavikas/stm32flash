# stm32flash
stm32 microcontroller flash update using bootrom code over uart or i2c

Stm32 bootcode supports flash update via uart/usart.
The max guaranteed speed is 115200 bps. Flashing stm32 at higher baud rate (higher than 115200 bps) is not supported by bootrom.

Reason being stm32 uart configuration at different baud rate. One way to overcome is to tune the host uart speed as per configured uart baud
rate.
