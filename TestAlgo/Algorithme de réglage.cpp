#include "mbed.h"

// Précision moteur pas à pas 28BYJ-48 : 1 pas = 0.087890625°, 1° = 11.37 pas.

Serial    pc(USBTX, USBRX);
BusOut motor_out(D2, D3, D5, D6);
BusOut motor_out2(D8, D9, D14, D15);  // blue - pink - yellow - orange // PWM


DigitalOut myled(LED2);
bool dir; // direction
int step_motor1 = 0;
int nbTour = 0;
bool flag_isr = false;
int Count = 0;
int nTour = 0;
bool bTour = false;
bool arretMoteur = false;
float Az = 4;
float Bz = 4;
float Cz = 4;
float Dz = 4;
float A;
float B;
float C;
float D;

int i;

const int kMaxBufferSize = 100000;
char      buffer[kMaxBufferSize];
int       len = 0;
long int Steps;//val en pourcent
int ValRecue;
long int ValDeg;
int StepDeg = 0.087890625;

void motorsOn()
{
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

    if (arretMoteur == true) { // LED  LD2 carte NUCLEO F446 allumée si le moteur tourne
        myled = 1;
    } else {
        myled = 0;
    }

    if(flag_isr == true) {
        Count++; // Compteur de pas à chaque step du switch.
        if (Count > Steps)  {
            wait_ms(1);
            //dir = !dir; // Faire le nombre de steps souhaité dans l'autre sens (boucle...)
            Steps = 0;
            nTour = nTour +1; // compteur de tours // 1 tour tous les 4096 pas.
            bTour = true;
            Count = 0; // Remise à 0 du compteur de pas à la fin de la commande.
        }
        if(bTour == true) {
            pc.printf("\nDone...\n");
            bTour = false;
        }

        if(step_motor1>7)step_motor1=0; // Boucle du switch(case)
        if(step_motor1<0)step_motor1=7;

        if(dir == false) step_motor1++; // Demi tour à chaque changement d'état de "dir"
        if (dir == true) step_motor1--;
    }
}
void degresCalc()
{
    ValDeg = (0.08789062*Steps);
    if (Steps==4096) ValDeg = 360;
}


float Calculx(float A, float B, float C, float D)
{

    float res =(((B+D) - (A+C)) / (A+B+C+D));
    return res;
}

void updateABCD() // update valeurs photodiode
{
    int xz = Calculx(Az, Bz, Cz, Dz);
    A = (Az*(1+((-1)^i)*4/(i+1)));
    B = (Bz*(1-((-1)^i)*4/(i+1)));
    C = (Cz*(1+((-1)^i)*4/(i+1)));
    D = (Dz*(1-((-1)^i)*4/(i+1)));
}

void Algo()
{
    i = 0;
    float xz = (((Bz+Dz) - (Az+Cz))/(Az+Bz+Cz+Dz));
    int nbPas = 2048; // nombre de pas initial
    updateABCD();
    float x = Calculx(A, B, C, D);
    bool dir0 = ((x-xz)>0);

    float Epsilon = 1/1000;
    while(((abs(x-xz)) > Epsilon)&& i < 100000000) {
        dir = dir0;
        Steps = nbPas;
        motorsOn();
        updateABCD();
        Calculx(A, B, C, D);
        dir0 = (x-xz)>0;
        if (dir != dir0) {
            nbPas = nbPas/2;
        }
        i++;
    }
}




int main()
{
    pc.baud(921600);

    // buffer[0] = '\0';
    // pc.printf("Start...\nHow many steps ? (Number of steps + $)\n");
    //  Calculx();
    Algo();
    while (1) {

    }

}
