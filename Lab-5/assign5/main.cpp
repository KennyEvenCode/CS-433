/*********************************************************
* Course: CS433 (Operating Systems)
* Programming Assignment 5 - Analysis of Page Replacement Algorithms
* Developers: Kenneth Campbell, Maribel Lopez, Alex Spathopoulos
* Compiler: g++
* Last Edit: 2022-05-11
*
* File name: 
*      main.cpp
*
* File type: 
*      C++ Driver File 
*
* Description: 
*       
*       
*       
*       
* 
************************************************************/

#include <chrono>
#include <string>
#include <fstream>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include "pagetable.h"

using namespace std;

// Check if an integer is power of 2
bool isPowerOfTwo(unsigned int x) {
	/* First x in the below expression is for the case when x is 0 */
	return x && (!(x & (x - 1)));
}

int main(int argc, char* argv[]) {
	//Print basic information about the program
	cout << "===============================================================================================" << endl;
	cout << "CS 433 Programming assignment 5" << endl;
	cout << "Author: Kenneth Campbell, Maribel Lopez, and Alex Spathopoulos" << endl;
	cout << "Date: 5/12/2022" << endl;
	cout << "Course: CS433 (Operating Systems)" << endl;
	cout << "Description : Program to simulate different page replacement algorithms (Random, FIFO, and LRU)" << endl;
	cout << "================================================================================================\n" << endl;

	if (argc != 3) { // changed from (argc < 3) to (argc != 3) for stricter evaluation of user input with argc being 1 plus the number of arguments given via command prompt (this change now prevents the entry of too many parameters)
		// user did not enter enough parameters
		cout << "You have entered too few parameters to run the program.  You must enter" << endl
			<< "two command-line arguments:" << endl
			<< " - page size (in bytes): between 256 and 8192, inclusive" << endl
			<< " - physical memory size (in megabytes): between 4 and 64, inclusive" << endl;
		exit(1);
	}
	
	// Page size and Physical memory size 
	// Their values should be read from command-line arguments, and always a power of 2
	unsigned int page_size = atoi(argv[1]);
	if(!isPowerOfTwo(page_size) || page_size < 256 || page_size > 8192) 	{
		cout << "You have entered an invalid parameter for page size (bytes)" << endl
			<< "  (must be an even number between 256 and 8192, inclusive)." << endl;
		return 1;
	}

	unsigned int phys_mem_size_mb = atoi(argv[2]);
	/* Convert from MB to bytes
	 * KB = B >> 10
	 * MB = KB >> 10 = B >> 20
	 * GB = MB >> 10 = KB >> 20 = B >> 30
	 */
	unsigned int phys_mem_size_b = phys_mem_size_mb << 20; 
	if(!isPowerOfTwo(phys_mem_size_b) || phys_mem_size_mb < 4 || phys_mem_size_mb > 64) 	{
		cout << "You have entered an invalid parameter for physical memory size (MB)" << endl
			<< "  (must be a power of 2 between 4 and 64, inclusive)." << endl;
		return 1;
	}

	// calculate number of pages and frames

		// 27-bit logical memory (128 MB logical memory assumed by the assignment)
		int logic_mem_bits = 27;		
		
		// Num of bits for physical memory addresses, calculated from physical memory size, e.g. 24 bits for 16 MB memory
		int phys_mem_bits = log2(phys_mem_size_b);		
		
		// Num of bits for page offset, calculated from page size, e.g. 12 bits for 4096 byte page
		int page_offset_bits = log2(page_size);				

		// Number of pages in logical memory = 2^(logic_mem_bits - page_bit)
		int num_pages = 1 << (logic_mem_bits - page_offset_bits);

		// Number of free frames in physical memory = 2^(phys_mem_bits - page_offset_bits)
		int num_frames = 1 << (phys_mem_bits - page_offset_bits);

	
	// display sizes, pages, and frames 
	
		cout << "Page size = " << page_size << " bytes" << endl;

		cout << "Physical Memory size = " << phys_mem_size_b << " bytes (" << phys_mem_size_mb << " MB)" << endl;

		cout << "Number of pages = " << num_pages << endl; 

		cout << "Number of physical frames = " << num_frames << endl;


	// Test 1: Read and simulate the small list of logical addresses from the input file "small_refs.txt"
	cout <<"\n================================Test 1==================================================\n";
	// TODO: Add your code here for test 1 that prints out logical page #, frame # and whether page fault for each logical address
	

	// Calculate number of page faults using FIFO replacement algorithm	
	cout << "****************************Simulate FIFO replacement****************************" << endl;

	PageTable pagetablef1;

	// open file 
	ifstream file_sf1("small_refs.txt");

	// initialize Page Table
	pagetablef1.init(phys_mem_size_b / page_size);

	// start timer
	auto beginf1 = chrono::high_resolution_clock::now(); 

	string line;

	while (file_sf1 >> line) {
		int value = stoi(line);
		int lastdigit = value % 10;	// extract the last digit from value.
		bool dirty;

		//check if valid 
		bool valid = true;

		// check the last digit of the line
		if (lastdigit % 2 == 1)	//if last digit is odd, set dirty bit.
			dirty = true;
		else 
			dirty = false; // last digit is even.

		// search for value in Page Table
		int index = pagetablef1.find(value); 

		// if the location returned is not -1, then it exists in the Page Table.
		if (index >= 0) {
			pagetablef1.page_table[index].dirty = dirty;
			pagetablef1.page_table[index].last_bit = pagetablef1.current_line;
		} 

		// the value was not found, so generate fault, and place page into Page Table.
		else { 
			PageEntry entry(value - dirty, dirty, valid, pagetablef1.current_line);
			pagetablef1.total_pageFaults++;
			pagetablef1.FIFO(entry);				
		}

		// how many lines have been parsed
		pagetablef1.total_references++;	
	} // end of while

	// end the timer
	auto endf1 = chrono::high_resolution_clock::now();

	//calculate total time 
	pagetablef1.total_time = chrono::duration_cast<chrono::milliseconds> (endf1 - beginf1).count();
		
	// Print the statistics and run-time
	pagetablef1.print_report();









	// Calculate number of page faults using LRU replacement algorithm
	cout << "****************************Simulate LRU replacement****************************" << endl;

	PageTable pagetablel1;

	// open file 
	ifstream file_sl1("small_refs.txt");

	// initialize Page Table
	pagetablel1.init(phys_mem_size_b / page_size);

	// start timer
	auto beginl1 = chrono::high_resolution_clock::now(); 

	

	while (file_sl1 >> line) {
		int value = stoi(line);
		int lastdigit = value % 10;	// extract the last digit from value.
		bool dirty;

		//check if valid 
		bool valid = true;

		// check the last digit of the line
		if (lastdigit % 2 == 1)	//if last digit is odd, set dirty bit.
			dirty = true;
		else 
			dirty = false; // last digit is even.

		// search for value in Page Table
		int index = pagetablel1.find(value); 

		// if the location returned is not -1, then it exists in the Page Table.
		if (index >= 0) {
			pagetablel1.page_table[index].dirty = dirty;
			pagetablel1.page_table[index].last_bit = pagetablel1.current_line;
		} 

		// the value was not found, so generate fault, and place page into Page Table.
		else { 
			PageEntry entry(value - dirty, dirty, valid, pagetablel1.current_line);
			pagetablel1.total_pageFaults++;
			pagetablel1.LRU(entry);				
		}

		// how many lines have been parsed
		pagetablel1.total_references++;	
	} // end of while

	// end the timer
	auto endl1 = chrono::high_resolution_clock::now();

	//calculate total time 
	pagetablel1.total_time = chrono::duration_cast<chrono::milliseconds> (endl1 - beginl1).count();
		
	// Print the statistics and run-time
	pagetablel1.print_report();









	// Calculate number of page faults using Random replacement algorithm
	cout << "****************************Simulate Random replacement****************************" << endl;

	PageTable pagetabler1;

	// open file 
	ifstream file_sr1("small_refs.txt");

	// initialize Page Table
	pagetabler1.init(phys_mem_size_b / page_size);

	// start timer
	auto beginr1 = chrono::high_resolution_clock::now(); 

	

	while (file_sr1 >> line) {
		int value = stoi(line);
		int lastdigit = value % 10;	// extract the last digit from value.
		bool dirty;

		//check if valid 
		bool valid = true;

		// check the last digit of the line
		if (lastdigit % 2 == 1)	//if last digit is odd, set dirty bit.
			dirty = true;
		else 
			dirty = false; // last digit is even.

		// search for value in Page Table
		int index = pagetabler1.find(value); 

		// if the location returned is not -1, then it exists in the Page Table.
		if (index >= 0) {
			pagetabler1.page_table[index].dirty = dirty;
			pagetabler1.page_table[index].last_bit = pagetabler1.current_line;
		} 

		// the value was not found, so generate fault, and place page into Page Table.
		else { 
			PageEntry entry(value - dirty, dirty, valid, pagetabler1.current_line);
			pagetabler1.total_pageFaults++;
			pagetabler1.Random(entry);				
		}

		// how many lines have been parsed
		pagetabler1.total_references++;	
	} // end of while

	// end the timer
	auto endr1 = chrono::high_resolution_clock::now();

	//calculate total time 
	pagetabler1.total_time = chrono::duration_cast<chrono::milliseconds> (endr1 - beginr1).count();
		
	// Print the statistics and run-time
	pagetabler1.print_report();


























	


	// Test 2: Read and simulate the large list of logical addresses from the input file "large_refs.txt"
	cout <<"\n================================Test 2==================================================\n";

	// Calculate number of page faults using FIFO replacement algorithm	
	cout << "****************************Simulate FIFO replacement****************************" << endl;

	PageTable pagetablef2;

	// open file 
	ifstream file_sf2("small_refs.txt");

	// initialize Page Table
	pagetablef2.init(phys_mem_size_b / page_size);

	// start timer
	auto beginf2 = chrono::high_resolution_clock::now(); 

	while (file_sf2 >> line) {
		int value = stoi(line);
		int lastdigit = value % 10;	// extract the last digit from value.
		bool dirty;

		//check if valid 
		bool valid = true;

		// check the last digit of the line
		if (lastdigit % 2 == 1)	//if last digit is odd, set dirty bit.
			dirty = true;
		else 
			dirty = false; // last digit is even.

		// search for value in Page Table
		int index = pagetablef2.find(value); 

		// if the location returned is not -1, then it exists in the Page Table.
		if (index >= 0) {
			pagetablef2.page_table[index].dirty = dirty;
			pagetablef2.page_table[index].last_bit = pagetablef2.current_line;
		} 

		// the value was not found, so generate fault, and place page into Page Table.
		else { 
			PageEntry entry(value - dirty, dirty, valid, pagetablef2.current_line);
			pagetablef2.total_pageFaults++;
			pagetablef2.FIFO(entry);				
		}

		// how many lines have been parsed
		pagetablef2.total_references++;	
	} // end of while

	// end the timer
	auto endf2 = chrono::high_resolution_clock::now();

	//calculate total time 
	pagetablef2.total_time = chrono::duration_cast<chrono::milliseconds> (endf2 - beginf2).count();
		
	// Print the statistics and run-time
	pagetablef2.print_report();









	// Calculate number of page faults using LRU replacement algorithm
	cout << "****************************Simulate LRU replacement****************************" << endl;

	PageTable pagetablel2;

	// open file 
	ifstream file_sl2("small_refs.txt");

	// initialize Page Table
	pagetablel2.init(phys_mem_size_b / page_size);

	// start timer
	auto beginl2 = chrono::high_resolution_clock::now(); 

	

	while (file_sl2 >> line) {
		int value = stoi(line);
		int lastdigit = value % 10;	// extract the last digit from value.
		bool dirty;

		//check if valid 
		bool valid = true;

		// check the last digit of the line
		if (lastdigit % 2 == 1)	//if last digit is odd, set dirty bit.
			dirty = true;
		else 
			dirty = false; // last digit is even.

		// search for value in Page Table
		int index = pagetablel2.find(value); 

		// if the location returned is not -1, then it exists in the Page Table.
		if (index >= 0) {
			pagetablel2.page_table[index].dirty = dirty;
			pagetablel2.page_table[index].last_bit = pagetablel2.current_line;
		} 

		// the value was not found, so generate fault, and place page into Page Table.
		else { 
			PageEntry entry(value - dirty, dirty, valid, pagetablel2.current_line);
			pagetablel2.total_pageFaults++;
			pagetablel2.LRU(entry);				
		}

		// how many lines have been parsed
		pagetablel2.total_references++;	
	} // end of while

	// end the timer
	auto endl2 = chrono::high_resolution_clock::now();

	//calculate total time 
	pagetablel2.total_time = chrono::duration_cast<chrono::milliseconds> (endl2 - beginl2).count();
		
	// Print the statistics and run-time
	pagetablel2.print_report();









	// Calculate number of page faults using Random replacement algorithm
	cout << "****************************Simulate Random replacement****************************" << endl;

	PageTable pagetabler2;

	// open file 
	ifstream file_sr2("small_refs.txt");

	// initialize Page Table
	pagetabler2.init(phys_mem_size_b / page_size);

	// start timer
	auto beginr2 = chrono::high_resolution_clock::now(); 

	while (file_sr2 >> line) {
		int value = stoi(line);
		int lastdigit = value % 10;	// extract the last digit from value.
		bool dirty;

		//check if valid 
		bool valid = true;

		// check the last digit of the line
		if (lastdigit % 2 == 1)	//if last digit is odd, set dirty bit.
			dirty = true;
		else 
			dirty = false; // last digit is even.

		// search for value in Page Table
		int index = pagetabler2.find(value); 

		// if the location returned is not -1, then it exists in the Page Table.
		if (index >= 0) {
			pagetabler2.page_table[index].dirty = dirty;
			pagetabler2.page_table[index].last_bit = pagetabler2.current_line;
		} 

		// the value was not found, so generate fault, and place page into Page Table.
		else { 
			PageEntry entry(value - dirty, dirty, valid, pagetabler2.current_line);
			pagetabler2.total_pageFaults++;
			pagetabler2.Random(entry);				
		}

		// how many lines have been parsed
		pagetabler2.total_references++;	
	} // end of while

	// end the timer
	auto endr2 = chrono::high_resolution_clock::now();

	//calculate total time 
	pagetabler2.total_time = chrono::duration_cast<chrono::milliseconds> (endr2 - beginr2).count();
		
	// Print the statistics and run-time
	pagetabler2.print_report();








	

















	
		

























	
	return 0; // close / exit program successfully. 
} // end of main