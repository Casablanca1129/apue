#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include "performance_testing.h"

#define BUFFER_SIZE 128

void copy_system(char *in_file, char *out_file) {
  char c;
  int in, out;

  in = open(in_file, O_RDONLY);
  if (in == -1) {
    perror("Failed to open input file");
  }

  out = open(out_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  if (out == -1) {
    perror("Failed to open output file");
    close(in);
  }

  while (read(in, &c, 1) == 1) {
    if (write(out, &c, 1) == -1) {
      perror("Write error");
      break;
    }
  }

  close(in);
  close(out);
}

void copy_block(char *in_file, char *out_file) {
  char buffer[BUFFER_SIZE];
  int in, out;
  ssize_t bytes_read;

  in = open(in_file, O_RDONLY);
  if (in == -1) {
    perror("Failed to open input file");
  }

  out = open(out_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  if (out == -1) {
    perror("Failed to open output file");
  }

  while ((bytes_read = read(in, buffer, sizeof(buffer))) > 0) {
    ssize_t bytes_written = write(out, buffer, bytes_read);
    if (bytes_written == -1) {
      perror("Write error");
    }
  }

  if (bytes_read == -1) {
    perror("Read error");
  }

  close(in);
  close(out);
}

void copy_stdio(char *in_file, char *out_file) {
  char buffer[BUFFER_SIZE];
  size_t bytes_read;
  FILE *in,*out;

  in = fopen(in_file, "rb");
  if (in == NULL) {
    perror("Failed to open input file");
  }

  out = fopen(out_file, "wb");
  if (out == NULL) {
    perror("Failed to open output file");
  }

  while ((bytes_read = fread(buffer, 1, sizeof(buffer), in)) > 0) {
    size_t bytes_written = fwrite(buffer, 1, bytes_read, out);
    if (bytes_written != bytes_read) {
      perror("Write error");
    }
  }

  if (ferror(in)) {
    perror("Read error");
  }

  fclose(in);
  fclose(out);
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
    exit(1);
  }
  TEST_FUNCTION_PERFORMANCE(copy_system(argv[1], argv[2]));
  TEST_FUNCTION_PERFORMANCE(copy_block(argv[1], argv[2]));
  TEST_FUNCTION_PERFORMANCE(copy_stdio(argv[1], argv[2]));
  exit(0);
}
