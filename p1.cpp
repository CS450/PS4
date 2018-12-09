/*
 *Authors: Brandon Williams and Skyler Penna
 *File/Project: p1.cpp/CS 450 Problem Set 4 number 1 and 2 
 *Date: 12/09/2018
 *Description: This file takes in a file from command line 
 *	       that gives information for converting a 
 *	       virtual address to a physical. It then asks
 *	       the user to enter a virtual address and will
 *	       display the corresponding physical address. 
 *	       Page replacement is handled by the Clock
 *	       Algorithm. See following link for information
 *	       about the Clock Algorithm. 
 *	       http://www.cs.utexas.edu/users/witchel/372/lectures/16.PageReplacementAlgos.pdf
 *	       input file structure:
 *	       Number of bits in virtual address
 * 	       Number of bits in physical address
 * 	       Number of BYTES in a page
 *	       The lines of a flat page table for the current process, in the following format:
 *			Valid?
 *			Permission (1 bit)
 *			Physical page number
 *			Use bit (see Problem 2)
 * 
*/

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <math.h>
#include <algorithm>
#include <bitset>

using namespace std;

struct address_data{
	vector<int> address_info;
	vector<vector<int>> page_table;
	vector<vector<int>> clock_table;
	int clock_pointer;

} input_data;

void print_struct(address_data data);
void input_file_data(char * filename);
vector<int> create_binary_vector(int data, int address_type);
int binaryVect_to_decimal(vector<int> v);
void clock_replacement(int virtual_page_to_replace);
void print_physical_address(int pagenum, int pages, vector<int> vaddress);
							
int main(int argc, char **argv){
	if(argc < 2){
		cout << "You need to specify the input file formatted as: [executable] [filename]\n";
		exit(EXIT_FAILURE);									
	}
	input_file_data(argv[1]);
	int virtual_address;
	std::vector<int> physical_address;
	int pages = input_data.page_table.size();
	int pagenum = 0;

	//string keepgoing = "c";
	while(1){
		cout << "Please enter a virtual address in hex, to exit press ctrl-c: \n";
		cout << ">> ";
		cin >> hex >> virtual_address;
		cin >> dec;
		//TODO: make sure this is a valid hex. 
		
		vector<int> vaddress = create_binary_vector(virtual_address, 0);

		cout << endl;

		for(int i = 0; i < log2(pages); i++){
			if(vaddress[log2(pages)-(i+1)] == 1){
				pagenum += exp2(i);
			}
		}

		//check permission bit:
		if(input_data.page_table[pagenum][1] == 0){
			cout << "SEGFAULT\n";
		}

		//at this point we know permission bit is 1, so now we check if valid = 0 and permission = 1
		//if this is the case, print DISK for problem 1 or utilize the clock replacement alg. for
		//problem 2
		else if(input_data.page_table[pagenum][0] == 0){
			#ifdef PROB1
			cout << "DISK\n";
			#else
			cout << "PAGE FAULT\n";
			clock_replacement(pagenum);
			print_physical_address(pagenum, pages, vaddress);
			#endif
		}
		else{//valid and permission bits both set so we good to calculate physical address
			print_physical_address(pagenum, pages, vaddress);
		}

		pagenum = 0;
		cout << endl;
	}

	return 0;
}

/*
 *Function Name: print_physical_address
 *Parameters: pagenum, which is the index from the virtual address into the page table. 
 *            pages, which is the total number of pages in the page table. 
 *            vaddress, a vector that represents the user inputted virtual address, stored
 *            as a vector of ints (which represent the binary version of the virtual address.  
 *Return Value: void
 *Description: This function is where we determine the physical address from a virtual address.  
*/
void print_physical_address(int pagenum, int pages, vector<int> vaddress){
	vector<int> paddress = create_binary_vector(input_data.page_table[pagenum][2], 1);
	
	paddress.insert(paddress.end(), vaddress.begin()+ log2(pages),  vaddress.end());
	
	if(paddress.size() < input_data.address_info[1]){
		int smallpaddress = paddress.size();
		for(int i = 0; i < (input_data.address_info[1]-smallpaddress); i++){
			paddress.insert(paddress.begin(), 0);
		}
	}
	cout << "Physical Address: ";

	cout << hex << binaryVect_to_decimal(paddress) << endl;
	//reset to dec
}

/*
 *Function Name: print_struct
 *Parameters: address_data which is a struct that holds information that was read in 
 *	      from an input file. Read top description of file to see how this input
 *	      file is structured.  
 *Return Value: void
 *Description: This function is was used for error testing and tracing through iterations
 * 	       of virtual address to physical address conversions. I simply prints out 
 * 	       the internal structure of the page table and our clock table, as well 
 * 	       as the initial information needed for converting virtual addresses to 
 * 	       physical.  
*/
void print_struct(address_data data){
	cout << "Address info\n";
	for(auto i = data.address_info.begin(); i != data.address_info.end();
			++i){
		cout << *i << " ";
	}
	cout << "\n\n";

	cout << "Page table\n";
	for(const auto i: data.page_table){
		for(const auto j: i){
			cout << j << " ";

		}
		cout << endl;
	}


	cout << "\n\nClock table\n";
	for(const auto i: data.clock_table){
		for(const auto j: i){
			cout << j << " ";

		}
		cout << endl;
	}
}

/*
 *Function Name: input_file_data
 *Parameters: filename, which is the file entered in at the command line. 
 *Return Value: void
 *Description: This function reads in the data from the input file, and structures it
 * 	       into a struct.  
*/
void input_file_data(char * filename){
	ifstream input;
	string line;
	int value;
	int lineCount = 0;

	input.open(filename);
	if(input.is_open()){
		while(getline(input, line)){
			vector<int> line_buffer;
			stringstream stream(line);
			if(lineCount == 0){
				while(stream >> value){
					input_data.address_info.push_back(value);
				}
				lineCount++;
			}
			else{
				while(stream >> value){
					line_buffer.push_back(value);
				}
				input_data.page_table.push_back(line_buffer);
			}
		}

		//setup clock_pointer and table
		input_data.clock_pointer = 0;


		for(int i = 0; i < input_data.page_table.size(); i++){
			if(input_data.page_table[i][2] > 0 && input_data.page_table[i][0] == 1){
				//ppn exists and is validhere so throw it in the clock table with the current i
				vector<int> page_data;
				page_data.push_back(input_data.page_table[i][3]);
				page_data.push_back(input_data.page_table[i][2]);
				page_data.push_back(i);
				input_data.clock_table.push_back(page_data);
			}
		}
	}
	else{
		cout << "Error opening " << filename << endl;
	}
	input.close();
}

/*
 *Function Name: create_binary_vector
 *Parameters: data, the decimal number that we are going to convert into binary
 *	      and store in a vector. 
 * 	      address_type, this is used to structure the way we build our vector
 *Return Value: vector of type int
 *Description: takes a decimal value, and stores the binary representation into a 
 * 	       vector of ints.  
*/
vector<int> create_binary_vector(int data, int address_type){
	vector<int> v; 

	if(address_type == 0){
		v.resize(input_data.address_info[0]);
		int i = 1;
		while(data > 0){
			fill_n(v.end()-i, 1, (data % 2));//add a try catch block?
			i++;
			data /= 2;
		}
	}
	else if(address_type == 1){
		int i = 1;
		while(data > 0){
			v.insert(v.begin(), (data%2));
			i++;
			data /= 2;
		}
	}
	return v;
}

/*
 *Function Name: binaryVect_to_decimal;
 *Parameters: v, which is a vector of ints that represent a binary number 
 *Return Value: integer
 *Description: This function converts an integer vector that is representing a
 *	       binary number into a decimal.  
*/
int binaryVect_to_decimal(vector<int> v){
	int decimal_value;
	for(int i = 0; i < v.size(); i++){
		if(v[v.size()-(i+1)] == 1){
			decimal_value += exp2(i);
		}
	}
	return decimal_value;
}

/*
 *Function Name: clock_replacement;
 *Parameters: a page number which is the virtual page that is invalid,  
 *Return Value: void
 *Description: This function utilizes the Clock Algorithm to map a 
 *	       physical page number to initially invalid page in the page table. 
*/
void clock_replacement(int virtual_page_to_replace){
	while(input_data.clock_table[input_data.clock_pointer][0] == 1){
		input_data.clock_table[input_data.clock_pointer][0] = 0;
			
		input_data.page_table[input_data.clock_table[input_data.clock_pointer][2]][3] = 0;
		
		input_data.clock_pointer++;

		if(input_data.clock_pointer >= input_data.clock_table.size()){
			input_data.clock_pointer = 0;
		}
	}						

	input_data.clock_table[input_data.clock_pointer][0] = 1;//change to in use because we are now mapping this to a new virtual resource
	input_data.page_table[input_data.clock_table[input_data.clock_pointer][2]][3] = 1;//change page tables use bit
												
	int physical_resource = input_data.clock_table[input_data.clock_pointer][1];//physical resource in our clock table stays the same
	
	int virtual_resource_to_delete = input_data.clock_table[input_data.clock_pointer][2];//this is our old virtual resource that we are booting out. 
	
	input_data.clock_table[input_data.clock_pointer][2] = virtual_page_to_replace;//change the virtual resource to point to our new one. 

	//add in the physical resource here and change valid = 1
	input_data.page_table[virtual_page_to_replace][0] = 1;
	input_data.page_table[virtual_page_to_replace][2] = physical_resource;
	input_data.page_table[virtual_page_to_replace][3] = 1;

	//we dont want to delete what we just newly mapped! so this is
	//used as a error check. 
	if(virtual_resource_to_delete != virtual_page_to_replace){
		//this stuff needs to be change valid = 0, its ppn to 0
		input_data.page_table[virtual_resource_to_delete][0] = 0;
		input_data.page_table[virtual_resource_to_delete][2] = 0;
		input_data.page_table[virtual_resource_to_delete][3] = 0;
	}
	
	
	input_data.clock_pointer += 1;//we found our resource to kick out, but still need to advance clock pointer. 
	if(input_data.clock_pointer > input_data.clock_table.size()){
		input_data.clock_pointer = 0;
	}
}
