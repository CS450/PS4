#include <iostream>
#include <vector> 
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

struct address_data{
	//int virtual_bits;
	//int physical_bits;
	//int num_bytes;
	vector<int> address_info;
	vector<vector<int>> page_table;
} input_data;

void print_struct(address_data data);

int main(int argc, char **argv){
	ifstream input;
	string line; 
	int value; 
	int lineCount = 0; 

	input.open(argv[1]);
	if(input.is_open()){
		while(getline(input, line)){
			vector<int> line_buffer; 
			stringstream stream(line);
			if(lineCount == 0){
				while(stream >> value){
					input_data.address_info.push_back(value);					}
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
		cout << "Error opening " << argv[1] << endl;
	}

	print_struct(input_data);

	
	input.close();
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
	/*
	for(auto i = data.page_table.begin(); i != data.page_table.end(); ++i){
		for(auto j = i.begin(); j != i.end(); ++j){
			cout << *j << " ";
		}
		cout << endl;
	}
	*/
}
