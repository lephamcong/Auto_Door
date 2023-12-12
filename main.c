#include <msp430.h>

#define motor_1_open BIT2 // P2.2
#define motor_1_close BIT3 // P2.3
#define motor_2_open BIT4 // P2.4
#define motor_2_close BIT5 // P2.5
#define sensor_1_open BIT1 // P1.1
#define sensor_1_close BIT2 // P1.2
#define sensor_2_open BIT5 // P1.5
#define sensor_2_close BIT6 // P1.6
#define switch_1_open BIT3 // P1.3
#define switch_1_close BIT4 // P1.4
#define switch_2_open BIT7 // P1.7
#define switch_2_close BIT0 // P1.0

#define SDA BIT0 //P2.0
#define SCl BIT1 //P2.1


void setup() {
    WDTCTL = WDTPW | WDTHOLD; // Dừng Watchdog timer

    // Cấu hình chân GPIO
    P1DIR &= ~(sensor_1_open + sensor_1_close + switch_1_open + switch_1_close + sensor_2_open + sensor_2_close + switch_2_open + switch_2_close);
    P1REN |= (sensor_1_open + sensor_1_close + switch_1_open + switch_1_close + sensor_2_open + sensor_2_close + switch_2_open + switch_2_close);
    P1OUT |= (sensor_1_open + sensor_1_close + switch_1_open + switch_1_close + sensor_2_open + sensor_2_close + switch_2_open + switch_2_close);
    P2DIR |= (motor_1_open + motor_1_close + motor_2_open + motor_2_close);
    P2OUT &= ~(motor_1_open + motor_1_close + motor_2_open + motor_2_close);

    // Kích hoạt ngắt cho cảm biến
    P1IE |= (sensor_1_open + sensor_1_close + sensor_2_open + sensor_2_close);
    P1IES |= (sensor_1_open + sensor_1_close + sensor_2_open + sensor_2_close);
    P1IFG &= ~(sensor_1_open + sensor_1_close + sensor_2_open + sensor_2_close);

    __bis_SR_register(GIE); // Kích hoạt ngắt toàn cục
}

void openDoor(int door) {
    if (door == 1) {
        P2OUT |= motor_1_open; // Mở cửa 1
        while (!(P1IN & switch_1_open)) {} // Chờ đến khi cửa mở hoàn toàn
        P2OUT &= ~motor_1_open; // Tắt động cơ
    }
    if (door == 2) {
        P2OUT |= motor_2_open; // Mở cửa 2
        while (!(P1IN & switch_2_open)) {} // Chờ đến khi cửa mở hoàn toàn
        P2OUT &= ~motor_2_open; // Tắt động cơ
    }
}

void closeDoor(int door) {
    if (door == 1) {
        P2OUT |= motor_1_close; // Đóng cửa 1
        while (!(P1IN & switch_1_close)) {} // Chờ đến khi cửa đóng hoàn toàn
        P2OUT &= ~motor_1_close; // Tắt động cơ
    }
    if (door == 2) {
        P2OUT |= motor_2_close; // Đóng cửa 2
        while (!(P1IN & switch_2_close)) {} // Chờ đến khi cửa đóng hoàn toàn
        P2OUT &= ~motor_2_close; // Tắt động cơ
    }
}

#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void) {
    if (P1IFG & sensor_1_open) {
        openDoor(1); // Mở cửa 1 nếu có người chạm cảm biến mở cửa 1
    }
    if (P1IFG & sensor_1_close) {
        closeDoor(1); // Đóng cửa 1 nếu có người chạm cảm biến đóng cửa 1
    }
    if (P1IFG & sensor_2_open) {
        openDoor(2); // Mở cửa 2 nếu có người chạm cảm biến mở cửa 2
    }
    if (P1IFG & sensor_2_close) {
        closeDoor(2); // Đóng cửa 2 nếu có người chạm cảm biến đóng cửa 2
    }
    P1IFG &= ~(sensor_1_open + sensor_1_close + sensor_2_open + sensor_2_close); // Xóa cờ ngắt
}

void main(void) {

    setup(); // Thiết lập chân GPIO và ngắt

    while (1) {
        // Chương trình chính không cần làm gì ở đây vì việc điều khiển được thực hiện thông qua ngắt
    }
}
