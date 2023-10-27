#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Creates a new variable BYTE that stores a BYTE of data
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Incorrect Number of CLA
    if (argc != 2) {
        printf("Improper Usage\n");
        return 1;
    }

    // Open the Memory Card and Initialize the file pointer pointing towards it ----- fread
    FILE *f = fopen(argv[1], "r");

    if (f == NULL) {
        printf("Unable to open file: %s\n", argv[1]);
        return 1;
    }

    // Initializing the buffer
    BYTE buffer[512];

    // Create a variable for filename
    int count = 0;
    char filename[8];

    FILE *new_file = NULL; // Initialize the new_file pointer to NULL

    // Read the input file in 512-byte chunks
    while (fread(buffer, 1, 512, f) == 512) {
        // Look for the beginning of a JPEG file (0xff, 0xd8, 0xff, 0xe0) ----- &0xf0
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0) {
            if (new_file != NULL) {
                fclose(new_file);
            }

            // Generate a new filename
            sprintf(filename, "%03i.jpg", count);
            new_file = fopen(filename, "w");

            if (new_file == NULL) {
                fprintf(stderr, "Failed to open file: %s\n", filename);
                fclose(f);
                return 1;
            }

            count++;
        }

        if (new_file != NULL) {
            // Writing the data continuously stored in the memory card
            fwrite(buffer, 1, 512, new_file);
        }
    }

    // Close the last JPEG file
    if (new_file != NULL) {
        fclose(new_file);
    }

    // Free resources
    fclose(f);

    return 0;
}

