#include "mbed.h"

BusOut motor_out(D2, D3, D5, D6);  // blue - pink - yellow - orange // PWM
DigitalOut myled(LED2);
int step = 0;
bool dir = false; // direction
Serial device(PA_2,PA_3);  // tx, rx //liaison 2, reliée au pc
int stepCounter = 0 ;
int nbTourInterne = 0;
int nbTour = 0;
Timer timer;
float timerValue;
bool flag_isr = false;
int varCount = 0;
int Count = 0;

void isr_rx()
{

    //device.printf("\n STEP NUMBER : %d",stepCounter);
    //device.printf("\n TOUR : %d",nbTour);
    Count = Count +1;
    if (Count > 4096)  {
        dir = !dir;
        Count = 0;
    }

    flag_isr = false;

}

int main()
{
    device.baud(38400);
    device.attach(&isr_rx,device.RxIrq);

    while(1) {
        switch(step) {

            case 0:
                motor_out = 0x1;
                flag_isr = true;
                wait_ms(1);
                break;  // 0001


            case 1:
                motor_out = 0x3;
                flag_isr = true;
                wait_ms(1);
                break;  // 0011

            case 2:
                motor_out = 0x2;
                flag_isr = true;
                wait_ms(1);
                break;  // 0010

            case 3:
                motor_out = 0x6;
                flag_isr = true;
                wait_ms(1);
                break;  // 0110

            case 4:
                motor_out = 0x4;
                flag_isr = true;
                wait_ms(1);
                break;  // 0100

            case 5:
                motor_out = 0xC;
                flag_isr = true;
                wait_ms(1);
                break;  // 1100

            case 6:
                motor_out = 0x8;
                flag_isr = true;
                wait_ms(1);
                break;  // 1000

            case 7:
                motor_out = 0x9;
                flag_isr = true;
                wait_ms(1);
                break;  // 1001


            default:
                motor_out = 0x0;
                flag_isr = true;
                wait_ms(1);
                break; // 0000

        }
        if (step == 1) { // LED  LD2 carte NUCLEO F446 allumée si le moteur tourne
            myled = 1;
        } else {
            myled = 0;
        }

        if(flag_isr == true) isr_rx(); // Routine d'interruption à chaque pas

        if(step>7)step=0; // Boucle du switch(case)
        if(step<0)step=7;
        
        if(dir == true) step++; // Demi tour à chaque changement d'état de "dir"
        if (dir == false) step--;
    }




    // wait_ms();  // speed

}
