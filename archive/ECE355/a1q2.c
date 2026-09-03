#define PBDIR (volatile unsigned char *) 0xFFFFFFF5
#define CTCON (volatile unsigned char *) 0xFFFFFFD8
#define CTSTAT (volatile unsigned char *) 0xFFFFFFD9
#define CNTM (volatile unsigned int *) 0xFFFFFFD0
#define IVECT (volatile unsigned int *) (0x20)
#define PBOUT (volatile unsigned char *) 0xFFFFFFF4
#define PBIN (volatile unsigned char *) 0xFFFFFFF3

// 0 = Increment DISABLED (D was pressed last).
//0 = Increment DISABLED (D was pressed last).
unsigned char increment_enabled = 0;
unsigned char digit = 0; /* Digit to be displayed */

interrupt void intserv();

int main(){
    *PBDIR = 0b11110000;    /*set port B direction(pins 2 and 3 take nothing as input, while 4-7 are output)*/
    *CTCON = 0b10;          /*stop timer*/
    *CTSTAT = 0b0;          /*Clear reached 0 flag*/
    *CNTM = 100000000;      /*Initialize timer to 100MHz*/
    *IVECT = (unsigned int *) &intserv; /*interrupt vector for 0x20*/
    *CTCON = 0b00100000;    
    asm(“MoveControl PSR,#0x40”); /* CPU responds to IRQ */

    while(1){
        *PBOUT = (*PBOUT & 0x0F) | (digit << 4);
    }
}

interrupt void intserv(){
    unsigned char input = *PBIN;

    if (!(input & 0x01)) {
        increment_enabled = 0; // D was pressed last
    } else if (!(input & 0x02)) {
        increment_enabled = 1; // E was pressed last
    }
    if (*CTSTAT & 0b1) {
        // Only increment if E was pressed last (increment_enabled == 1)
        if (increment_enabled == 1) {
            digit = (digit + 1) % 10; /* Increment digit (0 to 9 wrap arond) */
        }
        *CTSTAT = 0b1; 
    }
    
}