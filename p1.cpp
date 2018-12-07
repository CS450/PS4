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
	//cout << "the size of the page table = " << input_data.page_table.size() << endl;
	int pages = input_data.page_table.size();
	int pagenum;
	//	int temp = input_data.address_info[0];
	//const size_t v_address_size = const_cast<const int(temp);
	//	int v_address_size = input_data.address_info[0];

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
			pagenum += exp2(vaddress[log2(pages)-i]);
		}
		cout << "log2(pages) = " << log2(pages) << endl;
		//now we can lookup the ppn in the page table and create the physical address.
		vector<int> paddress = create_binary_vector(input_data.page_table[pagenum][2], 1);
		cout << "physical address: ";
		for(const auto i: paddress){
			cout << i;
		}
		cout << endl;
		//vector<int> paddress;
		paddress.insert(paddress.end(), vaddress.begin()+ pagenum,  vaddress.end());
		//fill(v.end()-i, v.end()-i, (data % 2));
		cout << "physical address: ";
		for(const auto i: paddress){
			cout << i;
		}
		//bitset<8> vaddress(virtual_address);

		cout << "\n\n" << "type 'q' to exit or 'c' to enter another virtual address in hex: ";
		cin >> keepgoing;
	}

	//converting the above decimal value to a binary stored in int vector.


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
		//std::vector<int> v (input_data.address_info[0]);
		v.resize(input_data.address_info[0]);
		int i = 1;
		while(data > 0){
			//fill(v.end()-i, v.end()-i, (data % 2));//add a try catch block?
			fill_n(v.end()-i, 1, (data % 2));//add a try catch block?
			i++;
			data /= 2;
			//v.push_back(data%2);
		}
	}
	else if(address_type == 1){
		//v.resize(3);
		//std::vector<int> v (input_data.address_info[1]);
		//v.resize(input_data.address_info[1]);
		int i = 1;
		while(data > 0){
			//fill(v.end()-i, v.end()-i, (data % 2));//add a try catch block?
			//fill_n(v.end()-i, 1, (data % 2));//add a try catch block?
			//v.push_back(data % 2);
			v.insert(v.begin(), (data%2));
			i++;
			data /= 2;
			//v.push_back(data%2);
		}
	}
	/*
	   if(v.size() < input_data.address_info[0]){
//this means that our read in hex value is not the same bit length
//as the virtual address length. so we pad with 0s to match
int difference = input_data.address_info[0] - v.size();
fill(v.begin(), v.begin() + difference, 0);
}
*/
return v;
}
