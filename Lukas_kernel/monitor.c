// From JamesM's tutorial

/*
    Defines functions for writing to the monitor
*/
#include "monitor.h"

// some useful defines
#define SCREEN_WIDTH 80
#define MAX_HEIGHT 25
#define BLACK 0
#define WHITE 15

// memory buffer starts at 0xB8000
u16int *video_memory = (u16int *)0xb8000;

// Default cursor position
u8int cursor_x = 0;
u8int cursor_y = 0;

static char checkTheBits(u8int);
static char *hex_to_string(u32int);
static char *dec_to_string(u32int);

static void move_cursor() {
    // Updates the hardware cursor

    // The screen is 80 characters wide
    u16int cursorLocation = cursor_y * SCREEN_WIDTH + cursor_x;
    outb(0x3D4, 14);                  // Tell the VGA board we are setting the high cursor byte.
    outb(0x3D5, cursorLocation >> 8); // Send the high cursor byte.
    outb(0x3D4, 15);                  // Tell the VGA board we are setting the low cursor byte.
    outb(0x3D5, cursorLocation);      // Send the low cursor byte.
}

static void scroll() {
    // Scrolls the text on the screen up by one line
    u8int bgColor = BLACK;
    u8int frColor = WHITE;

    u8int  attributeByte = (bgColor << 4) | (frColor & 0x0F);
    u16int blank = 0x20 | ((attributeByte) << 8); //space

    // Row 25 is the end, this means we need to scroll up
    if(cursor_y >= MAX_HEIGHT) {

        // Move the current text chunk that makes up the screen
        // back in the buffer by a line
        int i;
        for (i = 0; i < (MAX_HEIGHT- 1)*SCREEN_WIDTH; i++) {
            video_memory[i] = video_memory[i+SCREEN_WIDTH];
        }

        // The last line should now be blank
        for (i = (MAX_HEIGHT - 1)*SCREEN_WIDTH; i < MAX_HEIGHT*SCREEN_WIDTH; i++) {
            video_memory[i] = blank;
        }

        // The cursor should now be on the last line
        cursor_y = MAX_HEIGHT - 1;
    }
}

void monitor_put(char c) {
    // Writes a single character out to the screen

    u8int bgColor = BLACK;
    u8int frColor = WHITE;

    u8int  attributeByte = (bgColor << 4) | (frColor & 0x0F);
    u16int blank = 0x20 | ((attributeByte) << 8); //space

    u16int *location;

    // Handle a backspace, by moving the cursor back one space
    if (c == 0x08 && cursor_x) {
        cursor_x--;
    }

    // Handle a tab by increasing the cursor's X, but only to a point
    // where it is divisible by 8.
    else if (c == 0x09) {
        cursor_x = (cursor_x+8) & ~(8-1);
    }

    // Handle carriage return
    else if (c == '\r'){
        cursor_x = 0;
    }

    // Handle newline by moving cursor back to left and increasing the row
    else if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    }
    // Handle any other printable character.
    else if(c >= ' ') {
        location = video_memory + (cursor_y*SCREEN_WIDTH + cursor_x);
        *location = c | (attributeByte << 8); // top 8 bits have to be attributes
        cursor_x++;
    }

    // Check if we need to insert a new line because we have reached the end
    // of the screen.
    if (cursor_x >= SCREEN_WIDTH) {
        cursor_x = 0;
        cursor_y ++;
    }

    // Scroll the screen if needed.
    scroll();

    // Move the hardware cursor.
    move_cursor();

}

void monitor_clear() {
    // Clears the screen by putting spaces in it

    u8int bgColor = BLACK;
    u8int frColor = WHITE;
    u8int  attributeByte = (bgColor << 4) | (frColor & 0x0F);
    u16int blank = 0x20 | ((attributeByte) << 8); //space
    video_memory[0] = 'L';
    int i;
    for (i = 0; i < SCREEN_WIDTH*MAX_HEIGHT; i++) {
        video_memory[i] = blank;
    }

    // Move the hardware cursor back to the start.
    cursor_x = 0;
    cursor_y = 0;
    move_cursor();
}

// Outputs a null-terminated ASCII string to the monitor.
void monitor_write(char *c) {
    int i = 0;
    while (c[i]){
        monitor_put(c[i++]);
    }
}
void monitor_write_hex(u32int value){
    char *converted = hex_to_string(value);
    monitor_write("0x");
    monitor_write(converted);
    monitor_put('\n');
}
void monitor_write_dec(s32int number){
    char *converted = dec_to_string(number);
    monitor_write(converted);
    monitor_put('\n');
}

static char *dec_to_string(u32int n){
    // Let's build number from the bottom backwards

    // temporary values
    s32int temp_num = n;
    char temp_char;
    int counter = 0, size; // counter is needed to count how many elements are in the number

    int modulo; // this will hold the modulo
    char *number_string_reverse, *number_string; // will hold the number string
    while (temp_num/10 > 0){ // the condition because there is no way to get the size of the number
        // let's extract the decimal value of the position
        modulo = temp_num % 10;
        // get the string value of the decimal
        temp_char = checkTheBits(modulo);

        // append the reverse string and increment the counter
        number_string_reverse[counter++] = temp_char;

        //update the temp number to avoid infinite loop
        temp_num /= 10;
    }
    // after the loop there should be one more value needed to be added
    temp_char = checkTheBits(temp_num);
    number_string_reverse[counter] = temp_char;

    size = counter; // that's how many elements we have in our string

    // now let's rearrange to the normal order
    for (; counter > -1; counter--){
        number_string[size - counter] = number_string_reverse[counter];
    }
    number_string[size + 1] = '\0'; // make sure that it ends with null byte to avoid bugs
    return number_string;
}
static char *hex_to_string(u32int n){
    // output hexadecimal chars
    unsigned char* location = &n; //get the location of the hex number
    char value[9] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}; // init null byte array

    // array has 9 values, because last one reserved for null terminator
    unsigned char temp_char_l, temp_char_h; // temporary vars

    int i = 0, j = 7; // start at 7, because tested on little endian machine

    while(location[i]){
        // get lower 4 bits
        temp_char_l = location[i] & 0x0f;
        if(!(value[j--] = checkTheBits(temp_char_l))) break; // shouldn't happen if hex number given

        // get upper 4 bits
        temp_char_h = location[i++] >> 4;
        if(!(value[j--] = checkTheBits(temp_char_h))) break; // shouldn't happen if hex number given

        if ( i == 4 || j <= 0) break;
    }
    // readjust the array, since we start at the end of the array because of the endianness
    j = 0;
    while(!j){
        if (!value[j]){
            for(i = 0; i < 8; i++){
                value[i] = value[i+1];
            }
        } else { break;}
    }
    return (char *)value;
}

char checkTheBits(u8int hByte){
    /*
    Might not be the most efficient algorithm to find the hex
    value, but that's the way I thought to do it
    */

    // check and print the half byte value
    switch(hByte){
        case 0x01:
            return '1';
        case 0x02:
            return '2';
        case 0x03:
            return '3';
        case 0x04:
            return '4';
        case 0x05:
            return '5';
        case 0x06:
            return '6';
        case 0x07:
            return '7';
        case 0x08:
            return '8';
        case 0x09:
            return '9';
        case 0x0A:
            return 'A';
        case 0x0B:
            return 'B';
        case 0x0C:
            return 'C';
        case 0x0D:
            return 'D';
        case 0x0E:
            return 'E';
        case 0x0F:
            return 'F';
        case 0x00:
            return '0';
        default:
            // should never happen
            return 0;
    }
}


