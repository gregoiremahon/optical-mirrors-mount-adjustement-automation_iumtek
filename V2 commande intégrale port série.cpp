#include "mbed.h"

// Précision moteur pas à pas 28BYJ-48 : 1 pas = 0.087890625°, 1° = 11.37 pas.

Serial    pc(USBTX, USBRX);

BusOut motor_out(D2, D3, D5, D6);
BusOut motor_out2(D8, D9, D14, D15);  // blue - pink - yellow - orange // PWM
DigitalOut myled(LED2);
int step_motor1 = 0;
bool dir = true; // direction
int stepCounter = 0 ;
int nbTourInterne = 0;
int nbTour = 0;
Timer timer;
float timerValue;
bool flag_isr = false;
int varCount = 0;
int Count = 0;
int nTour = 0;
bool bTour = false;
bool arretMoteur = false;

const int kMaxBufferSize = 100000;
char      buffer[kMaxBufferSize];
int       len = 0;
long int Steps=0; //val en pourcent
int ValRecue;
long int ValDeg;
int StepDeg = 0.087890625;

void degresCalc()
{
    ValDeg = (0.08789062*Steps);
    if (Steps==4096) ValDeg = 360;
}

void isr_rx()
{
    Count = Count +1;
    if (Count > Steps)  {
        wait_ms(1);
        //dir = !dir; // Faire le nombre de steps souhaité dans l'autre sens (boucle...)
        Steps = 0;
        nTour = nTour +1;
        bTour = true;
        Count = 0;

    }


    //device.printf("\nSTEP NUMBER :%d",Count);
    //device.printf("\nTOUR :%d",nTour);

    flag_isr = false;


}

void isr_tours()
{
    //nTour = nTour +1;
    //device.printf("\nTOUR:%d",nTour);
    pc.printf("\nDone...\n");
    //wait(15);
    bTour = false;
}



int main(int argc, char *argv[])
{
    pc.baud(921600);

    buffer[0] = '\0';
    pc.printf("Start...\nHow many steps ? (Number of steps + $)\n");
    while (true) {
        if(Steps > 0) {
            switch(step_motor1) {
                case 0:
                    motor_out = 0x1;
                    motor_out2 = 0x1;
                    flag_isr = true;
                    arretMoteur = false;
                    wait_ms(1);
                    break;  // 0001


                case 1:
                    motor_out = 0x3;
                    motor_out2 = 0x3;
                    flag_isr = true;
                    arretMoteur = false;
                    wait_ms(1);
                    break;  // 0011

                case 2:
                    motor_out = 0x2;
                    motor_out2 = 0x2;
                    flag_isr = true;
                    arretMoteur = false;
                    wait_ms(1);
                    break;  // 0010

                case 3:
                    motor_out = 0x6;
                    motor_out2 = 0x6;
                    flag_isr = true;
                    arretMoteur = false;
                    wait_ms(1);
                    break;  // 0110

                case 4:
                    motor_out = 0x4;
                    motor_out2 = 0x4;
                    flag_isr = true;
                    arretMoteur = false;
                    wait_ms(1);
                    break;  // 0100

                case 5:
                    motor_out = 0xC;
                    motor_out2 = 0xC;
                    flag_isr = true;
                    arretMoteur = false;
                    wait_ms(1);
                    break;  // 1100

                case 6:
                    motor_out = 0x8;
                    motor_out2 = 0x8;
                    flag_isr = true;
                    arretMoteur = false;
                    wait_ms(1);
                    break;  // 1000

                case 7:
                    motor_out = 0x9;
                    motor_out2 = 0x9;
                    flag_isr = true;
                    arretMoteur = false;
                    wait_ms(1);
                    break;  // 1001


                default:
                    motor_out = 0x0;
                    motor_out2 = 0x0;
                    flag_isr = true;
                    wait_ms(1);
                    arretMoteur = true;
                    break; // 0000

            }
        }
        if (arretMoteur == true) { // LED  LD2 carte NUCLEO F446 allumée si le moteur tourne
            myled = 1;
        } else {
            myled = 0;


        }


        if(flag_isr == true) isr_rx();
        if(bTour == true) isr_tours();

        if(step_motor1>7)step_motor1=0; // Boucle du switch(case)
        if(step_motor1<0)step_motor1=7;
        
        if(dir == true) step_motor1++; // Demi tour à chaque changement d'état de "dir"
        if (dir == false) step_motor1--;


        while (pc.readable()) {
            char new_char = pc.getc();
             if (buffer[0] == '-') {
                 dir = !dir;
                 len=0;
                 }
            if (new_char != '$') {
                buffer[len++] = new_char;
            } else {

                //pc.printf("%s",buffer);
                Steps = atoi(buffer);
                degresCalc();
                pc.printf("\nProcessing %d steps...\n",Steps);
                pc.printf("\nEnviron %d degres ...\n",ValDeg);
                memset(buffer, 0, kMaxBufferSize);
                len=0;

            }

        }
    }
}