import os

class JPEGRecovery:
    def __init__(self, input_filename):
        self.input_filename = input_filename
        self.buffer = bytearray(512)
        self.count = 0
        self.new_file = None

    def open_input_file(self):
        try:
            self.file = open(self.input_filename, "rb")
        except FileNotFoundError:
            print(f"Unable to open file: {self.input_filename}")
            exit(1)

    def close_input_file(self):
        self.file.close()

    def create_output_file(self):
        if self.new_file is not None:
            self.new_file.close()

        # Generate a new filename
        filename = f"{self.count:03d}.jpg"
        self.new_file = open(filename, "wb")

        if not self.new_file:
            print(f"Failed to open file: {filename}")
            self.close_input_file()
            exit(1)

        self.count += 1

    def recover_jpegs(self):
        self.open_input_file()

        while self.file.readinto(self.buffer) == 512:
            # Look for the beginning of a JPEG file (0xff, 0xd8, 0xff, 0xe0)
            if (
                self.buffer[0] == 0xff
                and self.buffer[1] == 0xd8
                and self.buffer[2] == 0xff
                and (self.buffer[3] & 0xf0) == 0xe0
            ):
                self.create_output_file()

            if self.new_file is not None:
                # Write the data continuously stored in the memory card
                self.new_file.write(self.buffer)

        if self.new_file is not None:
            self.new_file.close()

        self.close_input_file()

if __name__ == "__main__":
    import sys

    if len(sys.argv) != 2:
        print("Improper Usage")
        exit(1)

    input_filename = sys.argv[1]

    recovery = JPEGRecovery(input_filename)
    recovery.recover_jpegs()
