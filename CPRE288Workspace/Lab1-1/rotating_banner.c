#include "Timer.h"
#include "lcd.h"


int main (void) {

    timer_init(); // Initialize Timer, needed before any LCD screen functions can be called
                  // and enables time functions (e.g. timer_waitMillis)

    lcd_init();   // Initialize the LCD screen.  This also clears the screen.
    
    
    char msg[] = "?"; //len of 33
        
    char feed[42+strlen(msg)];
        //printf("%i", strlen(msg));
    char queue[21]; //twenty chars plus null terminator;
    int j;
    for(j = 0; j < 20; j++){ //fill the queue with blank space;
        queue[j] = ' ';
    }
    strcpy(feed, queue);
    strncat(feed,msg, strlen(msg));
    strncat(feed, queue, strlen(queue));
    //strncat(msg, queue, strlen(queue)); //msg is now msg + queue (post padding)
        
    //printf(feed);
    int cursor = 0;
    int fwd = 1; //incr across feed
        
    while(1){
        //lcd_printf("q");
        lcd_init();
        cursor = fwd;
        //queue builder
        int i;
        for(i = 0; i < 20; i++){
            queue[i] = feed[cursor];
            cursor++;
        }
        fwd++;
        //printf("%i", cursor);
        //printf(" ");
          
        //printf(queue);
        //printf("\n");
        lcd_printf(queue);
        timer_waitMillis(300);

        //probably need a way for the banner to 'repeat forever'

        if (fwd >= (strlen(feed)-20)){
            fwd = 1;
        }

        }

   
    // Print "Hello, world" on the LCD
    //lcd_puts("Hello, world");

    // lcd_puts("Hello, world"); // Replace lcd_printf with lcd_puts
        // step through in debug mode and explain to TA how it works

    // NOTE: It is recommended that you use only lcd_init(), lcd_printf(), lcd_putc, and lcd_puts from lcd.h.
       // NOTE: For time functions, see Timer.h

    return 0;
}
