#CS 450 Problem Set 4:
Team Members: Brandon Williams and Skyler Penna

Problem 1: Address translation [30 points]
Write a program in C or C++ that takes the following information (in this order) from an input file. The input filename should be specified as a command line argument.
Number of bits in virtual address
Number of bits in physical address
Number of BYTES in a page
The lines of a flat page table for the current process, in the following format:
Valid?
Permission (1 bit)
Physical page number
Use bit (see Problem 2)
If the page is invalid, the input file will still contain values for the PPN bits, which should of course be ignored.

Here is an example. We did an in-class activity similar to this translation activity:
7 8 32
0 0 0 0
1 1 2 1
1 1 6 1
1 1 1 0
You can assume that this file is formatted correctly and that it describes a valid memory system.
Your job is to write a program that takes virtual addresses in hex and prints:
The corresponding physical addresses in hex. 
When pages are not in physical memory but have a permission bit set to 1, print DISK.
If the permission bit is 0, print SEGFAULT.

 
Problem 2: Page replacement [30 points]
Extend the program from Problem 1 to implement a clock replacement algorithm (see these notes for background: http://www.cs.utexas.edu/users/witchel/372/lectures/16.PageReplacementAlgos.pdf). Let the clock hand initially pointed at virtual page 0. This means that when the user enters an address that is currently mapped to disk, you should use the clock replacement algorithm to choose which virtual page to kick out of memory (and thus, which physical page to use). Use local replacement. That is, you should kick out a page from the current process that is currently mapped to physical memory.

Your program should update the page table to reflect this replacement. Instead of printing DISK, it should print PAGE FAULT followed by the new physical address. The input and output are otherwise identical to Problem 1.

Your implementations for Problems 1 and 2 can, and probably should, share source code. You may want to use preprocessor directives to differentiate the two implementations. For example:


#ifdef PROB1
cout << "DISK" << endl;
#else
cout << "PAGE FAULT" << endl;
// Your page replacement code
// Get new address translation
cout << new_PA << endl;
#endif
Then you would compile p2.x normally, and you would put "-D PROB1" on the g++ command line to compile p1.x.
 
 
Problem 3: Filesystem operations [20 points]
Write a C program that does the following, in order:
Opens the file named as the command-line argument for reading
Reads a value from the file
Forks a child process that calls unlink() to delete the file
Waits on the child process
Tries to read the now-deleted file
In an initial comment, explain what happens when you run your program, and why.


Problem 4: Filesystem operations [20 points]
Do the following worksheet:
https://docs.google.com/document/d/1OeDaPjfYRiUOej0_gYkk83VMDlLQCq8eF74IO6JN0Vo/edit?usp=sharing
it is a step-by-step worksheet with a series of tasks intended to help you explore file system commands, the representation of the filesystem on disk, and how files are deleted. Respond to the questions in the worksheet and submit the PDF.
