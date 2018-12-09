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
	print_struct(input_data);//just testing to make sure my clock table was init
	int virtual_address;
	std::vector<int> physical_address;
	int pages = input_data.page_table.size();
	int pagenum = 0;

	string keepgoing = "c";
	while(keepgoing == "c" || keepgoing == "C"){
		cout << "Please enter a virtual address in hex: ";
		cin >> hex >> virtual_address;
		//set cin back to dec?
		cin >> dec;
		//TODO: make sure this is a valid hex. 
		cout << "virtual_address = " << virtual_address << endl;
		vector<int> vaddress = create_binary_vector(virtual_address, 0);

		for(const auto i: vaddress){
			cout << i;
		}

		cout << endl;

		for(int i = 0; i < log2(pages); i++){
			if(vaddress[log2(pages)-(i+1)] == 1){
				pagenum += exp2(i);
			}
		}

		cout << "pagenum = " << pagenum << endl;
		cout << "log2(pages) = " << log2(pages) << endl;
		//now we can lookup the ppn in the page table and create the physical address.

		//check permission bit:
		if(input_data.page_table[pagenum][1] == 0){
			cout << "SEGFAULT\n\n";
			print_struct(input_data);//just testing to make sure my clock table was init
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
			
			//cout << dec;
			print_struct(input_data);//just testing to make sure my clock table was init

			#endif
		}
		else{//valid and permission bits both set so we good to calculate physical address
			print_physical_address(pagenum, pages, vaddress);
			print_struct(input_data);//just testing to make sure my clock table was init

		}


		cout << "\n\n" << "type 'q' to exit or 'c' to enter another virtual address in hex: ";
		cin >> keepgoing;
		pagenum = 0;
	}

	//just testing that file input was stored correctly here.
	//print_struct(input_data);

	return 0;
}

void print_physical_address(int pagenum, int pages, vector<int> vaddress){
	vector<int> paddress = create_binary_vector(input_data.page_table[pagenum][2], 1);
	
	//if the vector is bigger than the log2(pages) we need to cut off some bits. 
	
	/*
	while(paddress.size() > log2(pages)){
		paddress.erase(paddress.begin());	
	}
	*/

	cout << "physical address with just ppn: ";
	for(const auto i: paddress){
		cout << i;
	}
	cout << endl;
	paddress.insert(paddress.end(), vaddress.begin()+ log2(pages),  vaddress.end());
	
	if(paddress.size() < input_data.address_info[1]){
		int smallpaddress = paddress.size();
		for(int i = 0; i < (input_data.address_info[1]-smallpaddress); i++){
			paddress.insert(paddress.begin(), 0);
		}
	}
	cout << "physical address with added offset in hex: ";

	cout << hex << binaryVect_to_decimal(paddress) << endl << "and now in binary: ";
	//reset to dec
	cout << dec;
	for(const auto i: paddress){
		cout << i;
	}
	cout << endl;

}

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

int binaryVect_to_decimal(vector<int> v){
	int decimal_value;
	for(int i = 0; i < v.size(); i++){
		if(v[v.size()-(i+1)] == 1){
			decimal_value += exp2(i);
		}
	}
	return decimal_value;
}

void clock_replacement(int virtual_page_to_replace){
	cout << "in clock replacement: \n replacing virtual page: " << virtual_page_to_replace << endl;
	cout << "clock pointer is at: " << input_data.clock_pointer << endl;

	while(input_data.clock_table[input_data.clock_pointer][0] == 1){
		cout << "in page table while loop\n";
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
	

	//find this ppn in page table invalidate (given by our locationinpt)
	//now we found our page to kick. 
}
