/*********************************************************
* Course: CS433 (Operating Systems)
* Programming Assignment 5 - Analysis of Page Replacement Algorithms
* Developers: Kenneth Campbell, Maribel Lopez, Alex Spathopoulos
* Compiler: g++
* Last Edit: 2022-05-11
*
* File name: 
*      pagetable.h
*
* File type: 
*      C++ Header File 
*
* Description: 
*       
*       
*       
*       
* 
************************************************************/


#pragma once
#include <vector>

// Remember to add comments to your code

// A Page Table entry
class PageEntry {

	public:
		
		// Physical frame number for a given page
		int frame_num;
		
		// valid bit represents whether a page is in the physical memory
		bool valid = false;
		
		// dirty bit represents whether a page is changed
		bool dirty = false;

		// needed for lru algorithm
		int last_bit; 

	// defualt constructor
	PageEntry() {
		frame_num = 0;
		dirty = false;
		valid = false;
		last_bit = 0;
	}

	// constructor to create page entry
	PageEntry(int f, bool d, bool v, int l) {
		frame_num = f;
		dirty = d;
		valid = v;
		last_bit = l;
	}
};


/**
 * \brief A Page Table is like an array of page entries. The size of the Page Table should equal to the number of pages in logical memory
 */
class PageTable
{
	public:
		std::vector<PageEntry> page_table;	//a table for page entries
		int pagesize;	// the size of a page
		int memorysize;	// the total memory size
		int current_line; // counter used for fifo
		int line_position; // counter used in lru
		int total_pageFaults; // number of pages not found in Page Table
		int total_references; // number of references processed
		int flush_count; // number of victim pages marked as dirty
		double total_time;	// total time of simulation

	PageTable();
	void print_report();
	int find(int page);
	void init(int size);

	// paging algorithms
	void LRU(PageEntry &p);	// lru algorithm
	void FIFO(PageEntry &p);	// fifo alogirth
	void Random(PageEntry &p);   // random alogirth
};
