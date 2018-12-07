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
} input_data;

void print_struct(address_data data);
void input_file_data(char * filename);
vector<int> create_binary_vector(int data, int address_type);

int main(int argc, char **argv){
	input_file_data(argv[1]);
	int virtual_address;
	std::vector<int> physical_address;
	int pages = input_data.page_table.size();
	int pagenum;

	string keepgoing = "c";
	while(keepgoing == "c" || keepgoing == "C"){
		cout << "Please enter a virtual address in hex: ";
		cin >> hex >> virtual_address;
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
		}
		//at this point we know permission bit is 1, so now we check if valid = 0 and permission = 1
		//if this is the case, print DISK for problem 1 or utilize the clock replacement alg. for
		//problem 2
		else if(input_data.page_table[pagenum][0] == 0){
			#ifdef PROB1
				cout << "DISK\n";
			#else
				//do number 2
			#endif
		}
		else{//valid and permission bits both set so we good to calculate physical address
			vector<int> paddress = create_binary_vector(input_data.page_table[pagenum][2], 1);
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
			cout << "physical address with added offset: ";
			for(const auto i: paddress){
				cout << i;//TODO: convert to hex per assignment instructions
			}

		}


		cout << "\n\n" << "type 'q' to exit or 'c' to enter another virtual address in hex: ";
		cin >> keepgoing;
		pagenum = 0;
	}

	//just testing that file input was stored correctly here.
	//print_struct(input_data);

	return 0;
}

void print_struct(address_data data){
	for(auto i = data.address_info.begin(); i != data.address_info.end();
			++i){
		cout << *i << " ";
	}
	cout << endl;

	for(const auto i: data.page_table){
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
