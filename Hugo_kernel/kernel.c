void main()
{	
	/*
	 * Create a pointer to a char, and point it to the first text cell of
	 * video memory (the top left of the screen)
	 */
	char* video_mem = (char*) 0xb8000;
	
	/*
	 * At the address pointed to by video_memory, store the character 'X'
	 */
	*video_mem = 'X';	
}
