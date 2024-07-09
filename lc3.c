/* INCLUDES */
#include <execinfo.h>
#include <stdio.h>
#include <stdint.h>
#include <signal.h>
/* unix only */
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/termios.h>
#include <sys/mman.h>

#include "lc3.h"

/* Memory storage */
uint16_t memory[MEMORY_MAX];  /* 65,536 locations */
/* Register storage */
uint16_t reg[R_COUNT];

/* Input buffering */
struct termios original_tio;

void disable_input_buffering() {
  tcgetattr(STDIN_FILENO, &original_tio);
  struct termios new_tio = original_tio;
  new_tio.c_lflag &= ~ICANON & ~ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
}

void restore_input_buffering() {
  tcsetattr(STDIN_FILENO, TCSANOW, &original_tio);
}

uint16_t check_key() {
  fd_set readfds;
  FD_ZERO(&readfds);
  FD_SET(STDIN_FILENO, &readfds);

  struct timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = 0;
  return select(1, &readfds, NULL, NULL, &timeout) != 0;
}

/* Handle interrupt signal */
void handle_interrupt(int signal) {
  void *array[10];
  size_t size;
  // get void*'s for all entries on stack
  size = backtrace(array, 10);
  // print frames to stderr
  fprintf(stderr, "Error: signal %d:\n", signal);
  backtrace_symbols_fd(array, size, STDERR_FILENO);

  restore_input_buffering();
  printf("\n");
  exit(-2);
}

/* Sign extend a `bit_count`-length signed num to a 16-bit signed num.
 * https://en.wikipedia.org/wiki/Two%27s_complement
 */
uint16_t sign_extend(uint16_t x, int bit_count) {
  /* If negative (sign bit is 1), fill in the left bits with 1s.
   * Otherwise just return the value.
   */
  if ((x >> (bit_count - 1)) & 1) {
    x |= (0xFFFF << bit_count);
  }
  return x;
}

/* Swap endian-ness */
uint16_t swap16(uint16_t x) {
  return (x << 8) | (x >> 8);
}

/* Update flags any time a value is written */
void update_flags(uint16_t r) {
  if (reg[r] == 0) {
    reg[R_COND] = FL_ZRO;
  } else if (reg[r] >> 15) { // a 1 in the left-most bit indicates negative
    reg[R_COND] = FL_NEG;
  } else {
    reg[R_COND] = FL_POS;
  }
}

/* Read an LC-3 image file into memory */
void read_image_file(FILE* file) {
  // The origin tells us where in memory to place the image
  uint16_t origin;
  fread(&origin, sizeof(origin), 1, file);
  origin = swap16(origin);

  // We know the maximum file size so we only need one fread
  uint16_t max_read = MEMORY_MAX - origin;
  uint16_t* p = memory + origin;
  size_t read = fread(p, sizeof(uint16_t), max_read, file);

  // Swap to little endian
  while (read-- > 0) {
    *p = swap16(*p);
    ++p;
  }
}

/* Read an LC-3 program, given the path to the image file */
int read_image(const char* image_path) {
  FILE* file = fopen(image_path, "rb");
  if (!file) { return 0; };
  read_image_file(file);
  fclose(file);
  return 1;
}

/* Memory access */
void mem_write(uint16_t address, uint16_t val) {
  memory[address] = val;
}
uint16_t mem_read(uint16_t address) {
  if (address == MR_KBSR) {
    if (check_key()) {
      memory[MR_KBSR] = (1 << 15);
      memory[MR_KBDR] = getchar();
    }
    else {
      memory[MR_KBSR] = 0;
    }
  }
  return memory[address];
}

/* Main loop (read and execute instructions) */
int main (int argc, const char* argv[]) {
  // Load arguments
  if (argc < 2) {
    // Show usage string
    printf("lc3 [image-file1] ...\n");
    exit(2);
  }
  for (int j=1; j<argc; ++j) {
    if (!read_image(argv[j])) {
      printf("failed to load image: %s\n", argv[j]);
      exit(1);
    }
  }

  // Setup
  signal(SIGINT, handle_interrupt);
  disable_input_buffering();

  // Exactly one condition flag should be set at any given time, so set ZRO.
  reg[R_COND] = FL_ZRO;

  // Set the PC to the starting position, default 0x3000.
  // This leaves space for trap routine code.
  enum { PC_START = 0x3000 };
  reg[R_PC] = PC_START;

  int running = 1;
  while (running) {
    /* Fetch */
    uint16_t instr = mem_read(reg[R_PC]++);
    uint16_t op = instr >> 12;  /* read opcode from the leftmost 4 bits */

    switch (op) {
      case OP_ADD:
        fprintf(stderr, "error: ADD is not implemented yet\n"); exit(1);
        break;
      case OP_AND:
        fprintf(stderr, "error: AND is not implemented yet\n"); exit(1);
        break;
      case OP_NOT:
        fprintf(stderr, "error: NOT is not implemented yet\n"); exit(1);
        break;
      case OP_BR:
        fprintf(stderr, "error: BR is not implemented yet\n"); exit(1);
        break;
      case OP_JMP:
        fprintf(stderr, "error: JMP is not implemented yet\n"); exit(1);
        break;
      case OP_JSR:
        fprintf(stderr, "error: JSR is not implemented yet\n"); exit(1);
        break;
      case OP_LD:
        fprintf(stderr, "error: LD is not implemented yet\n"); exit(1);
        break;
      case OP_LDI:
        fprintf(stderr, "error: LDI is not implemented yet\n"); exit(1);
        break;
      case OP_LDR:
        fprintf(stderr, "error: LDR is not implemented yet\n"); exit(1);
        break;
      case OP_LEA:
        fprintf(stderr, "error: LEA is not implemented yet\n"); exit(1);
        break;
      case OP_ST:
        fprintf(stderr, "error: ST is not implemented yet\n"); exit(1);
        break;
      case OP_STI:
        fprintf(stderr, "error: STI is not implemented yet\n"); exit(1);
        break;
      case OP_STR:
        fprintf(stderr, "error: STR is not implemented yet\n"); exit(1);
        break;
      case OP_TRAP:
        reg[R_R7] = reg[R_PC];  // Stash PC in R7.
        switch (instr & 0xFF) {  // trap code is in bits [7:0]
          case TRAP_HALT:
            fprintf(stderr, "error: HALT is not implemented yet\n"); exit(1);
            break;
          case TRAP_GETC:
            fprintf(stderr, "error: GETC is not implemented yet\n"); exit(1);
            break;
          case TRAP_OUT:
            fprintf(stderr, "error: OUT is not implemented yet\n"); exit(1);
            break;
          case TRAP_PUTS:
            fprintf(stderr, "error: PUTS is not implemented yet\n"); exit(1);
            break;
          case TRAP_IN:
            fprintf(stderr, "error: IN is not implemented yet\n"); exit(1);
            break;
          case TRAP_PUTSP:
            fprintf(stderr, "error: PUTSP is not implemented yet\n"); exit(1);
            break;
        }
        break;
      case OP_RES:
      case OP_RTI:
        abort();
      default:
        printf("BAD OPCODE\n");
        exit(1);
    }
  }
  /* Shutdown (restore terminal settings) */
  restore_input_buffering();
}
