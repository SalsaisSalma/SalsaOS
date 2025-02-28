void kernel_main(void) {
    char *video_memory = (char*) 0xB8000; // VGA text buffer
    video_memory[0] = 'S';
    video_memory[1] = 0x07; // Light gray on black
    video_memory[2] = 'U';
    video_memory[3] = 0x07;

    video_memory[4] = 'C';
    video_memory[5] = 0x07; // Light gray on black
    video_memory[6] = 'A';
    video_memory[7] = 0x07;

}
