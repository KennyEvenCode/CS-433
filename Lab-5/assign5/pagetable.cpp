/*********************************************************
* Course: CS433 (Operating Systems)
* Programming Assignment 5 - Analysis of Page Replacement Algorithms
* Developers: Kenneth Campbell, Maribel Lopez, Alex Spathopoulos
* Compiler: g++
* Last Edit: 2022-05-11
*
* File name: 
*      pagetable.cpp
*
* File type: 
*      C++ Implementation File 
*
* Description: 
*       
*       
*       
*       
* 
************************************************************/

#include <iostream>
#include "pagetable.h"

using namespace std;

// Purpose: defualt constructor
PageTable::PageTable() {
    pagesize = 0;	// the size of each page
    memorysize = 0;	// the total memory size
    current_line = 0; // counter (used in fifo algorithm)
    line_position = 0; // counter (used in lru algorithm)
    total_pageFaults = 0; // number of pages not found in Page Table
    total_references = 0; // number of entries processed
    flush_count = 0; // number of victim pages marked 'dirty'
    total_time = 0.00;	// total (run)time 
}

// Purpose: Initializes a Page Table with empty pages
// Parameters: size = how large the table should be
void PageTable::init(int size) {
    page_table.clear();
	for (int i = 0; i < size; i++) {
		PageEntry e;
		e.frame_num = 0;
		e.last_bit = 0;
		e.dirty = false;
		e.valid = false;

		page_table.push_back(e);
	}
}

// Purpose: Searches for a page in the Page Table
// Parameters: page = value to search for
// Return: index if the page is found, else -1
int PageTable::find(int page)
{
	for (int i = 0; i < page_table.size(); i++) {
		if (page_table[i].frame_num == page) {
			return i;
		}
	}
    return -1;
}

// Purpose: Prints the Program Report / information.
void PageTable::print_report() {
	cout << "\n\nProgram Report:  \n" << endl;
	cout << "\t Total memory references: " << total_references << endl;
	cout << "\t Total page faults: " << total_pageFaults << endl;
	cout << "\t Total of page replacements.: " << flush_count << endl;
	cout << "\t Total time it took the simulator to produce the results:  " << total_time << " ms\n\n" << endl;
}

// Purpose: This is the FIFO paging replacement algorithm.
// Parameters: &p = pageEntry object
void PageTable::FIFO(PageEntry &p)
{
	int victim = line_position;

	line_position++;
	line_position = line_position % page_table.size();

	page_table[victim] = p; // replacement

	// if victim page is dirty, increment total flushes count
	if (page_table[victim].dirty == true)
		flush_count++;
}

// Purpose: This is the Random paging replacement algorithm.
// Parameters: &p = pageEntry object
void PageTable::Random(PageEntry &p)
{
	int rand_num = rand() % page_table.size();
	int victim = rand_num;
	page_table[victim] = p;  // replacement

	// if victim page is dirty, increment total flushes count
	if (page_table[victim].dirty == true)
		flush_count++;
}

// Purpose: This is the LRU paging replacement algorithm.
// Parameters: &p = pageEntry object
void PageTable::LRU(PageEntry &p)
{
	int victim;

	// page is being used.
	if (page_table[line_position].valid == false) {
		victim = line_position;
		line_position++;
		line_position = line_position % page_table.size();
	}

	// its being used, so find an unused page.
	else {
		int index = line_position;
		int min = current_line;

		for (int i = 0; i < page_table.size(); i++) {
			if (page_table[i].last_bit < min) {
				index = i;
				min = page_table[i].last_bit;
			}
		}
		victim = index;
	}

	page_table[victim] = p; // replacement

	// if victim page is dirty, increment total flushes count
	if (page_table[victim].dirty == true)
		flush_count++;
}