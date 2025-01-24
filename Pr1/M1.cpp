#ifndef _INC_M1
#define _INC_M1

#include "Pr1.h"

/*Includes */
#include<iostream>
#include<fstream>
#include <string>
#include <vector>
#include<map>
#include<ctime>

/* using namespace */
using namespace std;

/* #define */
#define DNA_Files_Number 239


int position = 1;
int range = 17;
char dna_chars[4] = { 'A','T','G','C' };

/* Global Data structures */

vector<string> spicific_mycoplasma = {"bovis","fermentans","hominis","hyorhinis","orale","salivarium","arginini"
									   ,"genitalium","pneumoniae","alcalescens","agalact","felis","bovirhinis"};

/*  Global files  */
//ofstream first_sentence_in_dna_File("first_sentence_in_dna_File.txt");
ofstream pair_of_words_file("pair_of_words_file.txt");

void DNA_File_correctness()
{
	cout << "\n\n Hello from theD NA_File_correctness() \n\n";

	char ch;
	for (int i = 1; i <= DNA_Files_Number; i++)
	{
		ifstream file("file" + to_string(i) + ".fna");
		ofstream file_to_write("DNA_File_" + to_string(i) + ".fna");
		while (file.get(ch))
		{
			if (ch == 'A' || ch == 'T' || ch == 'C' || ch == 'G')
				file_to_write << ch;			
		}
	}



}

void Prim1::find_if_word_in_all_the_files_z1()
{
	map<string, int> word_in_file1;// map to insert word in file 1, first field for DNA word, and the second for counter
	map<string, int>::iterator it_map = word_in_file1.begin();
	
	vector<string> dna_Files_name;
	dna_Files_name = fill_the_DNA_files();
	ifstream file1(dna_Files_name[0]);//open first file
	
	char ch;
	int rem;
	string DNA30;
	string word_of_DNA;
	string fileNumber;
	char ch_to_replace;
		cout << dna_Files_name[0];
		/*
		read all the CHARS AND INSERT IT TO STRING
		and compare them with the word in  map that we get from file 1
		stop when you arrive to TO STRUNG WITH LENGHT LESS THAN 30
		*/
		while (file1.get(ch))
		{
			if(ch=='A'||ch=='T'||ch=='C'||ch=='G')
			    word_of_DNA += ch;
		}
		cout << "\nFirst file commit \n";
		cout << word_of_DNA.size() << endl;
		int start;
		int j = word_of_DNA.length();
		for (string::iterator itr = word_of_DNA.begin(); itr != word_of_DNA.end(); itr++)
		{
			start = word_of_DNA.length() - j;
			j--;
			if (j >= range)
			{
				DNA30 = word_of_DNA.substr(start, range);
				it_map = word_in_file1.insert(it_map, { DNA30,0 });
			}
		}

		//for loop scan all the files, starting from second file to the last file.
		for (auto file_index = dna_Files_name.begin() + 1; file_index != dna_Files_name.end(); file_index++) {
			ifstream other_file(*file_index);
			string word_of_DNA_other;
			cout << *file_index;
			position++;
			/*
			read all the CHARS AND INSERT IT TO STRING
			and compare them with the word in  map that we get from file 1
			stop when you arrive to TO STRUNG WITH LENGHT LESS THAN 30
			*/

			while (other_file.get(ch))
			{
				if (ch == 'A' || ch == 'T' || ch == 'C' || ch == 'G')
				   word_of_DNA_other += ch;
			}
			int i = word_of_DNA_other.length();
			for (string::iterator itr1 = word_of_DNA_other.begin(); itr1 != word_of_DNA_other.end(); itr1++)
			{
				start = word_of_DNA_other.length() - i;
				i--;
				if (i >= range) {
					DNA30 = word_of_DNA_other.substr(start, range);
					if (word_in_file1.count(DNA30) > 0)//if the word exsit in other file 
					{
						it_map = word_in_file1.find(DNA30);
						/*fileNumber = *file_index;
						rem = fileNumber.length() - 4;
						//check if the counter of the word equal to the number(index) of the file -1,(counter start with 0)
						if (it_map->second == stoi(fileNumber.substr(fileNumber.length() - rem, rem)) - 2)
							it_map->second++;//raise the counter */
						if (it_map->second == position - 2)
						{
							it_map->second++;//raise the counter 
							//cout << "if " << it_map->second << endl;
						}
					}
					else {
						for (int i = 0; i < DNA30.length(); i++)
						{
							ch_to_replace = DNA30[i];
							for (int j = 0; j < 4; j++)//scan dna chars { 'A','T','G','C' }
							{		
								if ( ch_to_replace!= dna_chars[j]) //scan the string 
								{
									DNA30[i] = dna_chars[j];
									if (word_in_file1.count(DNA30) > 0)//if the word exsit in other file 
									{
										it_map = word_in_file1.find(DNA30);
										/*fileNumber = *file_index;
										rem = fileNumber.length() - 4;
										//check if the counter of the word equal to the number(index) of the file -1,(counter start with 0)
										if (it_map->second == stoi(fileNumber.substr(fileNumber.length() - rem, rem)) - 2)
											it_map->second++;//raise the counter */
										if (it_map->second == position - 2)
										{
											it_map->second++;//raise the counter 
										//cout <<"else "<< it_map->second << endl;
									}
									}
								}
							}//scan dna chars { 'A','T','G','C' }					
						}//scan the string 
					}//else
				}
			}
		}//for

	/*
	Write the mutual words to file
	-> The words with lenght 30
	-> The words that the counter they have equal to DNA_Files_Number-1(because we started with counter 0)
	*/
		for (auto it_print_map_to_file : word_in_file1)
		{
		//	cout << it_print_map_to_file.first << endl;
			if (it_print_map_to_file.second == (dna_Files_name.size() - 1) && it_print_map_to_file.first.length() == range)
			{
				//cout << "in"<<endl;
				pair_of_words_file << it_print_map_to_file.first << endl;
			}
		}

	
}

/*
This function unsert the files name into vector
and return this vector
after that we go forward to the files using the vector
*/
vector<string> Prim1::fill_the_DNA_files()
{
	vector<string> dna;
	string mycoplasma_type_checking;
	ifstream first_sentence_file("first_sentence_in_dna_File.txt");
	int index_of_type;
	string file_name;
	ofstream mycoplasma_we_need("mycoplasma_we_need.txt");
	cout << DNA_Files_Number<<endl;

	for (int i = 1; i <= DNA_Files_Number; i++)
	{
		//cout << DNA_Files_Number << endl;
	    getline(first_sentence_file,mycoplasma_type_checking);
		for (auto spicific_myco : spicific_mycoplasma)
		{
			
			index_of_type = mycoplasma_type_checking.find(spicific_myco);
			
			if (mycoplasma_type_checking[index_of_type-1]==' ' && mycoplasma_type_checking[index_of_type+spicific_myco.length()] == ' ')
			{

				 file_name = "file" + mycoplasma_type_checking.substr(6,mycoplasma_type_checking.find(')')-6) + ".fna";
				 mycoplasma_we_need << file_name+ "        " + spicific_myco<<endl;
				 dna.push_back(file_name);
			}
		}
		file_name = "";
	}

//	cout << "----------------------------";

	for (auto a : dna)
		cout << a << " ";
//	cout << "gggggggggggggg";
	mycoplasma_we_need.close();
	return dna;
}

/*
void main()
{
	clock_t start_time, end_time;
	start_time = clock();
//----------------------------------------------------------------------------------------------------------------
	cout << DNA_Files_Number<<endl;
	dna_Files_name = fill_the_DNA_files();
	cout << "we work with : "<<dna_Files_name.size() << " files    " << endl;
	pair_of_words_file << "Mutual words in all the files : "<< DNA_Files_Number<<" files\n\n";
	find_if_word_in_all_the_files();
	pair_of_words_file.close();
	//DNA_File_correctness();
//----------------------------------------------------------------------------------------------------------------
	cout << "\n\nCommit ! \n\n";
	end_time = clock();
	cout << "Time Taken" << (end_time - start_time) / CLOCKS_PER_SEC << endl;
}
*/

#endif  // _INC_M1