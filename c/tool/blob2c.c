// Required headers
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Where the Z80 blob is stored
#define MAX_SIZE 0x2000
uint8_t blob[MAX_SIZE];
size_t blob_size;

//***************************************************************************
// Program entry point
//***************************************************************************

int main(int argc, char **argv) {
   // Check number of arguments
   if (argc != 3) {
      fprintf(stderr, "Usage: %s <input> <output>\n", argv[0]);
      return EXIT_FAILURE;
   }

   // Open input file
   FILE *file = fopen(argv[1], "rb");
   if (file == NULL) {
      fputs("Error: couldn't open input file\n", stderr);
      return EXIT_FAILURE;
   }

   // Read the blob into memory and get its filesize
   // Use ferror() to check for failure since most likely we will be
   // attempting to read more data than is actually in the blob
   blob_size = fread(blob, 1, MAX_SIZE, file);
   if (ferror(file) && !feof(file)) {
      fputs("Error: couldn't read input file\n", stderr);
      fclose(file);
      return EXIT_FAILURE;
   }

   // Done with input file
   fclose(file);

   // Open output file
   file = fopen(argv[2], "w");
   if (file == NULL) {
      fputs("Error: couldn't open output file\n", stderr);
      return EXIT_FAILURE;
   }

   // Start the table
   fputs("static uint8_t echo_blob[] = {", file);

   // Output the blob data as a C array
   // Pretty formatting too!
   unsigned i;
   for (i = 0; i < blob_size; i++) {
      fprintf(file, "%s%3u%s",
         i % 0x10 == 0 ? "\n   " : "", blob[i],
         i == blob_size-1 ? "" : ",");
   }

   // End the table
   fputs("\n};\n", file);

   // Check if there was an error (to output an error message)
   if (ferror(file)) {
      fputs("Error: couldn't write output file\n", stderr);
      fclose(file);
      return EXIT_FAILURE;
   }

   // Done with output file
   fclose(file);

   // Done
   return EXIT_SUCCESS;
}
