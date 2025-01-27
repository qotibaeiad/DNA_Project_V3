#ifndef _INC_Pr1_1
#define _INC_Pr1_1

#include "Pr1.h"

string GetFileFasta(string fileName,string path)
{
    string line, dna = "";
    ifstream myfile(fileName);
    
    if (myfile.is_open())
    {
        while (getline(myfile, line))  // Read the file line by line
        {
            dna += line;  // Append each line to the dna string
        }
    }
    else
    {
        cout << "Unable to open file";
    }
    myfile.close();
    return dna;
}

void Prim1::make_map1(string dna1, int prsz1, string path)
{
	int i;
	string pattern;
//	map<string, int> mp1;
	pair<map<string, int>::iterator, bool> ret;
	ofstream outfile;

	for (i = 0; i <= dna1.size() - prsz1; i++)
	{
		pattern = dna1.substr(i, prsz1);
		ret = mp1_glob.insert(pair<string, int>(pattern, -1));
		if (ret.second == 0)
		{
			mp1_glob.at(pattern) = mp1_glob.at(pattern) + 1;
		}

	
		//		if (ret.second == 0)
		//		{
		//			mp.at(pattern) = mp.at(pattern) + 1;
		//		}

	}

	outfile.open(path+"\\"+"mp1_c");
	i = 1;
	for (auto& x : mp1_glob) {
		outfile << i << "," << x.first << ", " << x.second << endl;
		i++;
	}
	outfile.close();

//	return mp1;
}

void Prim1::find_words_in_mp1_glob(string dna2, int prsz1)
{
	int i;
	map<string, int>::iterator it;
	string pattern;
	ofstream outfile;

	for (i = 0; i <= dna2.size() - prsz1; i++)
	{
		pattern = dna2.substr(i, prsz1);
		it = mp1_glob.find(pattern);
		if (it != mp1_glob.end())
			it->second = 0;
		//mp1.erase(it);
	}
	/*
	outfile.open("mp2_c");
	i = 1;
	for (auto& x : mp1) {
		outfile << i << "," << x.first << ", " << x.second << endl;
		i++;
	}
	outfile.close();
	*/
}

vector <Prim_adr> Prim1::map_prim_adr1(map<string, int> mp1, string dna1, string dna2, int prsz1, string path)
{
	int i,j;
	map<string, int>::iterator it;
	string pattern;
	ofstream outfile;
	vector <Prim_adr> pr_mas;
	Prim_adr pa1;

	pr_mas.push_back(pa1);
	for (i = 0; i <= dna1.size() - prsz1; i++)
	{
		pattern = dna1.substr(i, prsz1);
		it = mp1.find(pattern);
		if (it->second == -1)
			continue;

		if (it->second == 0)
		{
			it->second = pr_mas.size();
			pa1.seq = pattern;
			pr_mas.push_back(pa1);
		}

		pr_mas[it->second].Nabour1.push_back(i);
	}

	for (i = 0; i <= dna2.size() - prsz1; i++)
	{
		pattern = dna2.substr(i, prsz1);
		it = mp1.find(pattern);
		if (it == mp1.end())
			continue;

		pr_mas[it->second].Nabour2.push_back(i);
	}

	outfile.open(path+"\\"+"mp2_c");
	for (i = 1; i < pr_mas.size(); i++)
	{
		outfile << i << "." << pr_mas[i].seq << "{";
		for (j = 0; j < pr_mas[i].Nabour1.size(); j++)
			outfile << pr_mas[i].Nabour1[j] << ",";
		outfile << "} {";
		for (j = 0; j < pr_mas[i].Nabour2.size(); j++)
			outfile << pr_mas[i].Nabour2[j] << ",";
		outfile << "}" << endl;
	}

	outfile.close();

	return pr_mas;

}


#endif  // _INC_Pr1_1
