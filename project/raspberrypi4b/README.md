### 1. Board

#### 1.1 Board Info

Board Name: Raspberry Pi 4B.

IIC Pin: SCL/SDA GPIO3/GPIO2.

GPIO Pin: RST/WAKE/INT GPIO27/GPIO22/GPIO17.

### 2. Install

#### 2.1 Dependencies

Install the necessary dependencies.

```shell
sudo apt-get install libgpiod-dev pkg-config cmake -y
```

#### 2.2 Configuration

```shell
sudo vi /boot/config.txt

# add or change
dtparam=i2c_arm=on,i2c_arm_baudrate=80000

# reboot the device
reboot
```

#### 2.3 Makefile

Build the project.

```shell
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

#### 2.4 CMake

Build the project.

```shell
mkdir build && cd build 
cmake .. 
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

Test the project and this is optional.

```shell
make test
```

Find the compiled library in CMake. 

```cmake
find_package(ccs811 REQUIRED)
```

### 3. CCS811

#### 3.1 Command Instruction

1. Show ccs811 chip and driver information.

   ```shell
   ccs811 (-i | --information)
   ```

2. Show ccs811 help.

   ```shell
   ccs811 (-h | --help)
   ```

3. Show ccs811 pin connections of the current board.

   ```shell
   ccs811 (-p | --port)
   ```

4. Run ccs811 register test.

   ```shell
   ccs811 (-t reg | --test=reg) [--addr=<0 | 1>]
   ```

5. Run ccs811 read test, num means test times.

   ```shell
   ccs811 (-t read | --test=read) [--addr=<0 | 1>] [--times=<num>]
   ```

6. Run ccs811 read test, path means bin file path.

   ```shell
   ccs811 (-t update | --test=update) [--addr=<0 | 1>] [--bin=<path>]
   ```

7. Run ccs811 interrupt test, low-threshold means eco2 low threshold, high-threshold means eco2  high threshold, hysteresis means hysteresis value, num means test times.

   ```shell
   cs811 (-t int | --test=int) [--addr=<0 | 1>] [--low=<low-threshold>] [--high=<high-threshold>] [--hysteresis=<hysteresis>] [--times=<num>]
   ```

8. Run ccs811 read function, num means read times.

   ```shell
   ccs811 (-e read | --example=read) [--addr=<0 | 1>] [--times=<num>]
   ```

9. Run ccs811 read raw function, num means read times.

   ```shell
   ccs811 (-e raw-data | --example=raw-data) [--addr=<0 | 1>] [--times=<num>]
   ```

10. Run ccs811 set environment function, rh means humidity percentage, degrees means temperature degrees.

       ```shell
       ccs811 (-e env | --example=env) [--addr=<0 | 1>] [--humidity=<rh>] [--temperature=<degrees>]
       ```
11. Run ccs811 set baseline, baseline means  set baseline.

       ```shell
       ccs811 (-e baseline | --example=baseline) [--addr=<0 | 1>] [--baseline=<baseline>]
       ```
12. Run ccs811 get hardware id function. 

       ```shell
       ccs811 (-e hardware-id | --example=hardware-id) [--addr=<0 | 1>]
       ```
13. Run ccs811 get hardware version function.

       ```shell
       ccs811 (-e hardware-version | --example=hardware-version) [--addr=<0 | 1>]
       ```
14. Run ccs811 get bootloader version function.

       ```shell
       ccs811 (-e bootloader-version | --example=bootloader-version) [--addr=<0 | 1>]
       ```
15. Run ccs811 get application version function.

       ```shell
       ccs811 (-e application-version | --example=application-version) [--addr=<0 | 1>]
       ```
16. Run ccs811 soft reset function.

       ```shell
       ccs811 (-e soft-reset | --example=soft-reset) [--addr=<0 | 1>]
       ```
17. Run ccs811 hard reset function.

       ```shell
       ccs811 (-e hard-reset | --example=hard-reset) [--addr=<0 | 1>]
       ```
18. Run ccs811 wake up function.

       ```shell
       ccs811 (-e wake-up | --example=wake-up) [--addr=<0 | 1>]
       ```
19. Run ccs811 power down function.

       ```shell
       ccs811 (-e power-down | --example=power-down) [--addr=<0 | 1>]
       ```
20. Run ccs811 status function.

       ```shell
       ccs811 (-e status | --example=status) [--addr=<0 | 1>]
       ```
21. Run ccs811 update function, path means bin file path.

       ```shell
       ccs811 (-e update | --example=update) [--addr=<0 | 1>] [--bin=<path>]
       ```
22. Run ccs811 interrupt function, low-threshold means eco2 low threshold, high-threshold means eco2  high threshold, hysteresis means hysteresis value, num means test times.

       ```shell
       ccs811 (-e int | --example=int) [--addr=<0 | 1>] [--low=<low-threshold>] [--high=<high-threshold>] [--hysteresis=<hysteresis>] [--times=<num>]
       ```
    
#### 3.2 Command Example

```shell
./ccs811 -i

ccs811: chip is AMS CCS811.
ccs811: manufacturer is AMS.
ccs811: interface is IIC.
ccs811: driver version is 1.0.
ccs811: min supply voltage is 1.8V.
ccs811: max supply voltage is 3.6V.
ccs811: max current is 54.00mA.
ccs811: max temperature is 85.0C.
ccs811: min temperature is -40.0C.
```

```shell
./ccs811 -p

ccs811: SCL connected to GPIO3(BCM).
ccs811: SDA connected to GPIO2(BCM).
ccs811: RST connected to GPIO27(BCM).
ccs811: WAKE connected to GPIO22(BCM).
ccs811: INT connected to GPIO17(BCM).
```

```shell
./ccs811 -t reg --addr=0

ccs811: chip is AMS CCS811.
ccs811: manufacturer is AMS.
ccs811: interface is IIC.
ccs811: driver version is 1.0.
ccs811: min supply voltage is 1.8V.
ccs811: max supply voltage is 3.6V.
ccs811: max current is 54.00mA.
ccs811: max temperature is 85.0C.
ccs811: min temperature is -40.0C.
ccs811: start register test.
ccs811: ccs811_set_addr_pin/ccs811_get_addr_pin test.
ccs811: set addr pin gnd.
ccs811: check addr pin ok.
ccs811: set addr pin vcc.
ccs811: check addr pin ok.
ccs811: ccs811_get_status test.
ccs811: status is 0x90.
ccs811: ccs811_set_mode/ccs811_get_mode test.
ccs811: set high constant power.
ccs811: check mode ok.
ccs811: set low power pulse heating.
ccs811: check mode ok.
ccs811: set pulse heating.
ccs811: check mode ok.
ccs811: set constant power.
ccs811: check mode ok.
ccs811: set idle.
ccs811: check mode ok.
ccs811: ccs811_set_interrupt_data_ready/ccs811_get_interrupt_data_ready test.
ccs811: enable interrupt data ready.
ccs811: check interrupt data ready ok.
ccs811: disable interrupt data ready.
ccs811: check interrupt data ready ok.
ccs811: ccs811_set_interrupt_threshold/ccs811_get_interrupt_threshold test.
ccs811: enable interrupt threshold.
ccs811: check interrupt threshold ok.
ccs811: disable interrupt threshold.
ccs811: check interrupt threshold ok.
ccs811: ccs811_set_environment_data test.
ccs811: set raw data 0xB0F2.
ccs811: check environment data ok.
ccs811: ccs811_set_eco2_threshold test.
ccs811: set raw data 0x5641.
ccs811: set hysteresis 0xA2.
ccs811: check eco2 threshold ok.
ccs811: ccs811_set_baseline/ccs811_get_baseline test.
ccs811: set baseline 0xFFFF.
ccs811: check baseline ok.
ccs811: ccs811_get_hardware_id test.
ccs811: hardware id is 0x81.
ccs811: ccs811_get_hardware_version test.
ccs811: hardware version is 0x12.
ccs811: ccs811_get_firmware_bootloader_version test.
ccs811: firmware bootloader version major is 0x01.
ccs811: firmware bootloader version minor is 0x10.
ccs811: firmware bootloader version trivial is 0x00.
ccs811: ccs811_get_firmware_application_version test.
ccs811: firmware application version major is 0x02.
ccs811: firmware application version minor is 0x20.
ccs811: firmware application version trivial is 0x01.
ccs811: ccs811_soft_reset test.
ccs811: check soft reset ok.
ccs811: ccs811_wake_up test.
ccs811: disable wake up.
ccs811: check wake up ok.
ccs811: enable wake up.
ccs811: check wake up ok.
ccs811: ccs811_hard_reset test.
ccs811: check hard reset ok.
ccs811: ccs811_environment_humidity_convert_to_register/ccs811_environment_humidity_convert_to_data test.
ccs811: set environment humidity 91.50%.
ccs811: check environment humidity 91.50%.
ccs811: ccs811_environment_temperature_convert_to_register/ccs811_environment_temperature_convert_to_data test.
ccs811: set environment temperature 29.30C.
ccs811: check environment temperature 29.30C.
ccs811: finish register test.
```

```shell
./ccs811 -t read --addr=0 --times=3

ccs811: chip is AMS CCS811.
ccs811: manufacturer is AMS.
ccs811: interface is IIC.
ccs811: driver version is 1.0.
ccs811: min supply voltage is 1.8V.
ccs811: max supply voltage is 3.6V.
ccs811: max current is 54.00mA.
ccs811: max temperature is 85.0C.
ccs811: min temperature is -40.0C.
ccs811: start read test.
ccs811: read test.
ccs811: eco2 ppm is 400 ppm.
ccs811: tvoc ppb is 0 ppb.
ccs811: eco2 ppm is 400 ppm.
ccs811: tvoc ppb is 0 ppb.
ccs811: eco2 ppm is 400 ppm.
ccs811: tvoc ppb is 0 ppb.
ccs811: read raw test.
ccs811: raw is 0x19A2.
ccs811: ua is 6uA.
ccs811: adc is 0.67V.
ccs811: raw is 0x19AB.
ccs811: ua is 6uA.
ccs811: adc is 0.69V.
ccs811: raw is 0x19B3.
ccs811: ua is 6uA.
ccs811: adc is 0.70V.
ccs811: finish read test.
```

```shell
./ccs811 -t update --addr=0 --bin=./bin/ccs811_firmware_app_v2.0.1.bin

ccs811: chip is AMS CCS811.
ccs811: manufacturer is AMS.
ccs811: interface is IIC.
ccs811: driver version is 1.0.
ccs811: min supply voltage is 1.8V.
ccs811: max supply voltage is 3.6V.
ccs811: max current is 54.00mA.
ccs811: max temperature is 85.0C.
ccs811: min temperature is -40.0C.
ccs811: start update test.
ccs811: finish update test.
```

```shell
./ccs811 -t int --addr=0 --low=200 --high=300 --hysteresis=2 --times=3

ccs811: chip is AMS CCS811.
ccs811: manufacturer is AMS.
ccs811: interface is IIC.
ccs811: driver version is 1.0.
ccs811: min supply voltage is 1.8V.
ccs811: max supply voltage is 3.6V.
ccs811: max current is 54.00mA.
ccs811: max temperature is 85.0C.
ccs811: min temperature is -40.0C.
ccs811: start interrupt test.
ccs811: eco2 ppm is 400 ppm, please check int gpio level.
ccs811: eco2 ppm is 400 ppm, please check int gpio level.
ccs811: eco2 ppm is 400 ppm, please check int gpio level.
ccs811: finish interrupt test.
```

```shell
./ccs811 -e read --addr=0 --times=3

ccs811: 1/3.
ccs811: co2 eq is 400 ppm.
ccs811: tvoc is 1 ppb.
ccs811: 2/3.
ccs811: co2 eq is 400 ppm.
ccs811: tvoc is 0 ppb.
ccs811: 3/3.
ccs811: co2 eq is 400 ppm.
ccs811: tvoc is 0 ppb.
```
```shell
./ccs811 -e raw-data --addr=0 --times=3

ccs811: 1/3.
ccs811: ua is 5uA.
ccs811: adc is 0.65V.
ccs811: 2/3.
ccs811: ua is 5uA.
ccs811: adc is 0.67V.
ccs811: 3/3.
ccs811: ua is 37uA.
ccs811: adc is 0.68V.
```
```shell
./ccs811 -e env --addr=0 --humidity=50.0 --temperature=25.0

ccs811: set humidity 50.00%.
ccs811: set temperature 25.00C.
```
```shell
./ccs811 -e baseline --addr=0 --baseline=100

ccs811: set baseline 100.
```
```shell
./ccs811 -e hardware-id --addr=0

ccs811: hardware id is 0x81.
```
```shell
./ccs811 -e hardware-version --addr=0

ccs811: hardware version is 0x12.
```
```shell
./ccs811 -e bootloader-version --addr=0

ccs811: firmware bootloader version major is 0x01.
ccs811: firmware bootloader version minor is 0x10.
ccs811: firmware bootloader version trivial is 0x00.
```
```shell
./ccs811 -e application-version --addr=0

ccs811: firmware application version major is 0x02.
ccs811: firmware application version minor is 0x20.
ccs811: firmware application version trivial is 0x01.
```
```shell
./ccs811 -e soft-reset --addr=0

ccs811: soft reset.
```
```shell
./ccs811 -e hard-reset --addr=0

ccs811: hard reset.
```
```shell
./ccs811 -e wake-up --addr=0

ccs811: wake up.
```
```shell
./ccs811 -e power-down --addr=0

ccs811: power down.
```
```shell
./ccs811 -e status --addr=0

ccs811: status is 0x90.
```
```shell
./ccs811 -e update --addr=0 --bin=./bin/ccs811_firmware_app_v2.0.1.bin

ccs811: start updating.
ccs811: end updating.
```
```shell
./ccs811 -e int --addr=0 --low=200 --high=300 --hysteresis=2 --times=3

ccs811: 1/3.
ccs811: co2 eq is 400 ppm.
ccs811: tvoc is 0 ppb.
ccs811: 2/3.
ccs811: co2 eq is 400 ppm.
ccs811: tvoc is 0 ppb.
ccs811: 3/3.
ccs811: co2 eq is 400 ppm.
ccs811: tvoc is 0 ppb.
```
```shell
./ccs811 -h

Usage:
  ccs811 (-i | --information)
  ccs811 (-h | --help)
  ccs811 (-p | --port)
  ccs811 (-t reg | --test=reg) [--addr=<0 | 1>]
  ccs811 (-t read | --test=read) [--addr=<0 | 1>] [--times=<num>]
  ccs811 (-t update | --test=update) [--addr=<0 | 1>] [--bin=<path>]
  ccs811 (-t int | --test=int) [--addr=<0 | 1>] [--low=<low-threshold>]
         [--high=<high-threshold>] [--hysteresis=<hysteresis>] [--times=<num>]
  ccs811 (-e read | --example=read) [--addr=<0 | 1>] [--times=<num>]
  ccs811 (-e raw-data | --example=raw-data) [--addr=<0 | 1>] [--times=<num>]
  ccs811 (-e env | --example=env) [--addr=<0 | 1>] [--humidity=<rh>] [--temperature=<degrees>]
  ccs811 (-e baseline | --example=baseline) [--addr=<0 | 1>] [--baseline=<baseline>]
  ccs811 (-e hardware-id | --example=hardware-id) [--addr=<0 | 1>]
  ccs811 (-e hardware-version | --example=hardware-version) [--addr=<0 | 1>]
  ccs811 (-e bootloader-version | --example=bootloader-version) [--addr=<0 | 1>]
  ccs811 (-e application-version | --example=application-version) [--addr=<0 | 1>]
  ccs811 (-e soft-reset | --example=soft-reset) [--addr=<0 | 1>]
  ccs811 (-e hard-reset | --example=hard-reset) [--addr=<0 | 1>]
  ccs811 (-e wake-up | --example=wake-up) [--addr=<0 | 1>]
  ccs811 (-e power-down | --example=power-down) [--addr=<0 | 1>]
  ccs811 (-e status | --example=status) [--addr=<0 | 1>]
  ccs811 (-e update | --example=update) [--addr=<0 | 1>] [--bin=<path>]
  ccs811 (-e int | --example=int) [--addr=<0 | 1>] [--low=<low-threshold>]
         [--high=<high-threshold>] [--hysteresis=<hysteresis>] [--times=<num>]

Options:
      --addr=<0 | 1>                 Set the chip address.([default: 0])
      --baseline=<baseline>          Set the baseline value.([default: 0])
      --bin=<path>                   Set the firmware binary file path.([default: update.bin])
  -e <read | raw-data | env | baseline | hardware-id | hardware-version | bootloader-version |
      application-version | soft-reset | hard-reset | wake-up | power-down | status | update | int>,
      --example=<read | raw-data | env | baseline | hardware-id | hardware-version | bootloader-version |
      application-version | soft-reset | hard-reset | wake-up | power-down | status | update | int>
                                     Run the driver example.
  -h, --help                         Show the help.
      --high=<high-threshold>        Set the high threshold.([default: 500])
      --hysteresis=<hysteresis>      Set the hysteresis.([default: 2])
      --humidity=<rh>                Set the relative humidity.([default: 50])
  -i, --information                  Show the chip information.
      --low=<low-threshold>          Set the low threshold.([default: 200])
  -p, --port                         Display the pin connections of the current board.
  -t <reg | read | update | int>, --test=<reg | read | update | int>
                                     Run the driver test.
      --temperature=<degrees>        Set the temperature in degrees celsius.([default: 25])
      --times=<num>                  Set the running times.([default: 3])
```


#### 3.3 Command Problem

There is some unknown problem in the iic interface of ccs811 on the raspberry board, one command may try many times to run successfully.
