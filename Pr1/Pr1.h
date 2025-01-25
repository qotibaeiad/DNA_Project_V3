#ifndef _INC_Pr1
#define _INC_Pr1


#include <iostream>
#include <fstream>
#include <string>
#include<conio.h>
#include <iterator> 
#include <map>
#include <vector>
#include <algorithm> // Fix for remove_if
#include <cctype>    // Fix for isspace



using namespace std;

struct Prim_adr
{
	vector <int> Nabour1;
	vector <int> Nabour2;
	string seq;
};

struct ListFasta
{
	string dna;
	string header;
};

struct Prim_in_map
{
	string seq;
	int num;
	bool l;
	bool r;
	int tm;
	int pos;
};

struct prim_pair
{
	string seq1;
	string seq2;
	string seq3;
	int pos1;
	int pos2;
	int Tm1;
	int Tm2;
	int Tm3;
	int cons1;
	int cons2;
	int pos3;
	int cons3;
	int pleng3;
};


string GetFileFasta(string,string); //reading of the DNA sequence from the file in FASTA format 

class Prim1
{
public:
	map<string, int> mp1_glob;
	vector<Prim_in_map> map_list1;
	vector<prim_pair> prim_pair_list1;
	map<string, int>::iterator it_glob;

	vector<string> fill_the_DNA_files();
	
	void make_map1(string, int, string); //creates mapp of all words of the given size, calculates their amounts
	void find_words_in_mp1_glob(string, int);
	void find_words_read_in_mp1_glob(string, int);
	vector <Prim_adr> map_prim_adr1(map<string, int>, string, string, int, string);
	vector<ListFasta> GetFileListFasta(string,string);
	void read_map1(string,string);

	bool prim_filter1(string); //checks primer properties
	bool prim_3_check(string); //checks primer properties to be right
	bool prim_5_check(string); //checks primer properties to be left
	int prime_Tm(string); //calculate Tm of the primer

	void select_pairs(string, int, string); //select pairs and after this  triplets of primers

	void find_if_word_in_all_the_files_z1();
	void conserv_calc(const std::string& input_file_path);
 //calculation of the conservation of the given primers
	int find_closest_dist(string, string); //find minimal distance from pattern to genom
	string polindrom(string); //buuld polindromic sequence
	vector<string> seq_glob1;
};




#endif  // _INC_Pr1