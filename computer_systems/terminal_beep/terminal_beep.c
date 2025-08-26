#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

#define BELL_CODE 0x07
struct termios saved_termios_struct;


void restore_input_mode(void);
void set_raw_input_mode(void);

/* rings the terminal bell n times with n taken from raw (non
   canonical) stdin */
int main() {
  char c;
  int n = 0;

  set_raw_input_mode();

  for (;;) {
    c = getchar();
    if (c == 0x03 || c == 0x04) // CTRL+C CTRL+D
      break;
    else if (c >= '1' && c <= '9') {
      n = c - 48;
      while (n-- > 0)
        putchar(BELL_CODE);
    }
  }
  return EXIT_SUCCESS;
}

void set_raw_input_mode(void) {
  /* save current termios_struct */
  tcgetattr(STDIN_FILENO, &saved_termios_struct);
  atexit(restore_input_mode);

  struct termios termios_struct;
  tcgetattr(STDIN_FILENO, &termios_struct);
  /* Disable canonical processing, read requests are satisfied
   * directly from the input queue. Disable echoing input characters
   * to the terminal */
  termios_struct.c_lflag &= ~(ICANON|ECHO);
  termios_struct.c_cc[VMIN] = 1;
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &termios_struct);
}

void restore_input_mode(void) {
  tcsetattr(STDIN_FILENO, TCSANOW, &saved_termios_struct);
}
