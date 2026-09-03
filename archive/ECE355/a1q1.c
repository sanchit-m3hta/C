#define PBDIR (volatile unsigned char *) 0xFFFFFFF5
#define CTCON (volatile unsigned char *) 0xFFFFFFD8
#define CTSTAT (volatile unsigned char *) 0xFFFFFFD9
#define CNTM (volatile unsigned int *) 0xFFFFFFD0
#define IVECT (volatile unsigned int *) (0x20)
#define PBOUT (volatile unsigned char *) 0xFFFFFFF4
#define PBIN (volatile unsigned char *) 0xFFFFFFF3

interrupt void intserv();

unsigned char digit = 0; /* Digit to be displayed */

int main(){
    *PBDIR = 0b11110000; /*set port B direction(pins 2 and 3 take nothing as input, while 4-7 are output)*/
    *CTCON = 0b10; /*stop timer*/
    *CTSTAT = 0b0; /*Clear reached 0 flag*/
    *CNTM = 100000000; /*Initialize timer to 100MHz*/
    *IVECT = (unsigned int *) &intserv; /*Set up interrupt vector*/
    *CTCON = 0b00010001; //turn on start and enable interrupt bits
    asm(“MoveControl PSR,#0x40”); /* CPU responds to IRQ */

    while(1){
        unsigned char input = *PBIN;
        if ((input & 0x03) == 0) {
            digit = 0;
        }

        *PBOUT = (*PBOUT & 0x0F) | (digit << 4); 
    }

}

interrupt void intserv(){
    unsigned char input = *PBIN;

    if ( !(input & 0x01) && (input & 0x02) ) {
        digit = (digit + 1)%10; /* Increment digit */
    }

    *CTSTAT = 0b1; 
}