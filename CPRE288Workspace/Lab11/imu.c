/*
 * @file imu.c
 * @brief IMU specific functions for for utilizing the IMU
 * @author Michael Farmer
 * @date 4/29/2026
 */

#include "imu.h"
#include "timer.h"
#include  "lcd.h"

extern volatile char command_byte; // byte value for special character used as a command
extern volatile int command_flag; // flag to tell the main program a special command was received

char g_imu_command = 0;


/*
* Init IMU registers themselves, and pulse the reset line
* @author Michael Farmer
* @date 4/29/2026
*/
void imu_init() {
    i2c_init();
    timer_init(); //required

    //set_slave_address(0x29); //takes care of GPIO init

    GPIO_PORTB_DIR_R |= 0x40; //make PB6 output
    GPIO_PORTB_AFSEL_R &= ~0x40; // disable alt func for PB6
    GPIO_PORTB_DEN_R |= 0x40; //enable digital io

    GPIO_PORTB_DATA_R &= ~0x40; //set PB6 (RSTN) to 0 (RESET)

    timer_waitMillis(50);

    GPIO_PORTB_DATA_R |= 0X40;

    timer_waitMillis(650);

    ///Now do some writes to IMU
    imu_write_command(0x3E, 0x00); //PWR_MODE --> NORMAL
    imu_write_command(0x3D, 0x00); //OPR_MODE --> CONFIG

    //AXIS Remap to option P0 in the Datasheet
    imu_write_command(0x41, 0x21); //AXIS_REMAP_CONFIG --> See datasheet
    imu_write_command(0x42, 0x4); //AXIS_REMAP_SIGN --> invert X

    timer_waitMillis(50);
    imu_write_command(0x3D, 0x0C); //OPR_MODE --> NDOF [Nine Degrees of Freedom] ((DEFAULT))





}

/*
* Send a write command to IMU using I2C
* @author Michael Farmer
* @param address The IMU address to write to
* @param data The data byte to write
* @date 4/29/2026
*/
void imu_write_command(char address, char data) {
    char command[2];
    command[0] = address;
    command[1] = data;

    i2c_send_bytes(command, 2);
}

/*
* Get current heading in degrees of IMU
* @author Michael Farmer
* @date 4/29/2026
*/
float imu_get_heading_deg() {
    //should be in NDOF
    uint8_t data[2];
    i2c_imu_read_registers(data, 2, 0x1A);

    return ((float)data[0] + (float)(data[1] << 8)) / 16.0f;
}


/*
* Set OPR to COMPASS Mode and check calibration
* @author Michael Farmer
* @param calibrate_acc Whether to actually calibrate the Accelerometer, as opposed to skipping it
* @date 4/29/2026
*/
void imu_set_compass_mode(bool calibrate_acc) {
    imu_write_command(0x3D, 0x09); //OPR_MODE --> COMPASS
    uint8_t status = i2c_imu_read_register(0x35); //CALIB_STAT
    uint8_t required_status = 0b11110011 | (calibrate_acc*3 << 2);
    while((status & 0xFF) != required_status && ((status & 0xFF) != 0xFF)) {
             status = i2c_imu_read_register(0x35); //CALIB_STAT
             short acc_stat = ((status & 0b1100) == 0b1100);
             short gyro_stat = ((status & 0b110000) == 0b110000);
             short mag_stat = ((status & 0b11) == 0b11);
             short sys_stat = ((status & 0b11000000) == 0b11000000);
             lcd_printf("Calibration Status:\nACC: %s %s\nGYRO: %s\nMAG: %6s S:%s", acc_stat ? "OK" : "NOT OK", calibrate_acc ? "" : "(SKIP)", gyro_stat ? "OK" : "NOT OK", mag_stat ? "OK" : "NOT OK", sys_stat ? "OK" : "NOTOK");
             timer_waitMillis(1500);
         }
}

/*
* Set OPR to NDOF Mode and check calibration
* @author Michael Farmer
* @param calibrate_acc Whether to actually calibrate the Accelerometer, as opposed to skipping it
* @date 4/29/2026
*/
void imu_set_ndof_mode(bool calibrate_acc) {
    imu_write_command(0x3D, 0x0C); //OPR_MODE --> NDOF [Nine Degrees of Freedom]
    uint8_t status = i2c_imu_read_register(0x35); //CALIB_STAT
    uint8_t required_status = 0b11110011 | (calibrate_acc*3 << 2);
    while((status & 0xFF) != required_status && ((status & 0xFF) != 0xFF)) {
            status = i2c_imu_read_register(0x35); //CALIB_STAT
            short acc_stat = ((status & 0b1100) == 0b1100);
            short gyro_stat = ((status & 0b110000) == 0b110000);
            short mag_stat = ((status & 0b11) == 0b11);
            short sys_stat = ((status & 0b11000000) == 0b11000000);
            lcd_printf("Calibration Status:\nACC: %s %s\nGYRO: %s\nMAG: %6s S:%s", acc_stat ? "OK" : "NOT OK", calibrate_acc ? "" : "(SKIP)", gyro_stat ? "OK" : "NOT OK", mag_stat ? "OK" : "NOT OK", sys_stat ? "OK" : "NOTOK");
            timer_waitMillis(1500);
        }
}

/*
* Set the TM4C timer to call an interrupt ever 0.3 seconds 
* @author Michael Farmer
* @param ch The character to send on interrupt
* @date 4/29/2026
*/
void setup_imu_timer(char ch)
{
    SYSCTL_RCGCTIMER_R |= 0x010; //enable clock for timer 4

    TIMER4_CTL_R &= ~0x100; //disable timer 4B
    TIMER4_CFG_R = 0x04; //set timer to 16bit
    TIMER4_TBMR_R = 0x22; //periodic mode with match interrupt
    TIMER4_TBILR_R = 0x4987; //load 3/10 second as count down start (after 255x prescale)
    TIMER4_TBPR_R = 0xFF; //set 8bit prescaler to presca
    TIMER4_TBMATCHR_R = 0x00;
    TIMER4_TBPMR_R = 0x0;
    TIMER4_IMR_R |= 0x800; //Match interrupts enable

    NVIC_PRI17_R = (NVIC_PRI17_R & ~0xE0000000) | 0xA0000000; //interrupt 71 to priority 5
    NVIC_EN2_R |= 0x80; // interrupt 71

    g_imu_command = ch;

    IntRegister(INT_TIMER4B, TIMER4B_HANDLER);

    TIMER4_CTL_R |= 0x100; //enable timer
}

/*
* The Handler called on interrupt. Just sets command byte and flag.
* @author Michael Farmer
* @date 4/29/2026
*/
void TIMER4B_HANDLER() {
    TIMER4_ICR_R |= 0x800; //clear match interrupt

    command_byte = g_imu_command; //u command
    command_flag = 1;
}

