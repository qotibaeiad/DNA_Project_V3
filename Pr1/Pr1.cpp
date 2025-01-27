#include "Pr1.h"

//#include "Pr1_1.cpp"
std::string remove_extension(const std::string& filename) {
    size_t last_dot = filename.find_last_of(".");
    if (last_dot == std::string::npos) return filename; 

std::cout << "removed last :-  " << filename.substr(0, last_dot) << std::endl;
    return filename.substr(0, last_dot);  // Return substring before the last dot
	
}

std::vector<std::string> extractDNASequences(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<std::string> dnaList;

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return dnaList;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || isdigit(line[0]) || line.find("THE BEST") != std::string::npos) {
            continue;
        }

        size_t pos = line.find_first_of("0123456789");  
        if (pos != std::string::npos) {
            std::string dna = line.substr(0, pos); 
            dna.erase(std::remove_if(dna.begin(), dna.end(), ::isspace), dna.end());
            dnaList.push_back(dna);
        }
    }

    file.close();
    return dnaList;
}


int main(int argc, char* argv[]) 
{
	if (argc < 3) {
    std::cerr << "Error: No file path provided." << std::endl;
    return 1;  // Exit early
}


// std::vector<std::string> dnaList = extractDNASequences("dna_request\\dna_1737799125621\\list_prim_tripl");
    
//     if (dnaList.empty()) {
//         std::cerr << "No DNA sequences found in the file." << std::endl;
//         return 0;
//     }

//     size_t num_sequences = dnaList.size();
//     std::vector<std::string> primss(2 * num_sequences);  

//     // Store extracted sequences and their complements
//     for (size_t i = 0; i < num_sequences; ++i) {
//         primss[i] = dnaList[i];                
//         primss[num_sequences + i] = dnaList[i]; 
//     }

//     // Print results
// 	 int cnt = 0;
//     std::cout << "Extracted DNA Sequences and Their Complements:\n";
//     for (size_t i = 0; i < 2 * num_sequences; ++i) {

//         std::cout << "prims[" << i << "] = \"" << primss[i] << "\";\n";
// 		if (cnt>10){
// 			return 0;
// 		}
// 		cnt++;
//     }







	const char* file_path1 = argv[1];  // Use file_path1
	const char* blast = argv[2];
    std::cout << "Processing DNA file NAME: " << file_path1 << std::endl;  // Correct variable name

	Prim1 pp;
	pp.conserv_calc(file_path1);
	// cout << "Processing file: " << file_path1 << endl;  // Make sure to print this

	int i,j, prsz1 = 20;
	string pattern;
	const char *file_path = argv[1];
	 std::cout << "Processing DNA file NAME: " << file_path << std::endl;
    // printf("Processing file: %s\n", file_path);
	string path1 = "dna_request\\" + remove_extension(file_path)+"\\";
	//string fname2, fname1 = path1 + "sc.fasta";
	
	string fname2, fname1 = path1 + file_path;
    std::cout << fname1 << std::endl;
	
	string dna2, dna1 = GetFileFasta(fname1,path1);
	pp.make_map1(dna1, prsz1, path1);
	//cerr << "dnd" << dna;
	fname2 = remove_extension(file_path);
	ofstream outfile(fname2);
	//outfile <<"dna "<< dna1.size()<< "\n" <<dna1;
	//outfile.close();

	pair<map<string, int>::iterator, bool> ret;
	map<string, int>::iterator it;
	
//	fname2 = path1 + "\\s3.fasta";
//	dna2 = GetFileFasta(fname2);
//fname2 = "C:\\Users\\qotib\\OneDrive\\Desktop\\DNA_Project_V3\\server\blast_results_20250125_191024.txt";
   // string blastpath = "./";
	fname2 = blast;
	pp.read_map1(fname2,path1);
	std::cout << "happ" << std::endl; 
	fname2 = "map_list1_c2";
	outfile.open(path1+"\\"+fname2);
	for (i = 0; i < pp.map_list1.size(); i++)
	{
		if (!pp.prim_filter1(pp.map_list1[i].seq)) pp.map_list1[i].num = -2;
		pp.map_list1[i].l = pp.prim_5_check(pp.map_list1[i].seq);
		pp.map_list1[i].r = pp.prim_3_check(pp.map_list1[i].seq);
		pp.map_list1[i].tm = pp.prime_Tm(pp.map_list1[i].seq);
		outfile << i << "," << pp.map_list1[i].seq << ", " << pp.map_list1[i].num << ", ";
		if (pp.map_list1[i].num == -1)
		{
			if (pp.map_list1[i].l)
				outfile << "l, ";
			if (pp.map_list1[i].r)
				outfile << "r, ";
		}
		outfile << pp.map_list1[i].tm <<"\n";

	}

	outfile.close();
	
	pp.select_pairs(dna1, prsz1,path1);
    std::cout << "DNA processing complete " << file_path << std::endl;
	return 0;
	//	fname2 = "c2";
//	outfile.open(fname2);
//	outfile << "dna " << dna2.size() << "\n" << dna2;
//	outfile.close();
	pp.make_map1(dna1, prsz1,path1);
//	find_words_in_map(mp1, dna2, prsz1);
//	vector <Prim_adr> pr_mas = map_prim_adr1(mp1, dna1, dna2, prsz1);

	//--------------------------------------------


	// fname2 = path1 + "seqdump.txt";
	// vector<ListFasta> dna_mas = pp.GetFileListFasta(fname2);
	// for (i = 41; i < dna_mas.size(); i++)
	// {
	// 	pp.find_words_in_mp1_glob(dna_mas[i].dna, prsz1);
	// }

	// outfile.open("mp2_c");
	// i = 1;
	// for (auto& x : pp.mp1_glob) {
	// 	outfile << i << "," << x.first << ", " << x.second << endl;
	// 	i++;
	// }
	
	// outfile.close();


	// fname1 = path1 + "hu\\GCF_000001405.39_GRCh38.p13_genomic.fna";
	// pp.find_words_read_in_mp1_glob(fname1, prsz1);

	// outfile.open("mp3_c");
	// i = 1;
	// for (auto& x : pp.mp1_glob) {
	// 	outfile << i << "," << x.first << ", " << x.second << endl;
	// 	i++;
	// }

	// outfile.close();
	// return(0);
}

void Prim1::conserv_calc(const std::string& input_file_path)
{
   //vector<string> prims = getListDNA("list_prime_triple");
//    std::vector<std::string> dnaList = extractDNASequences("dna_request\\dna_1737799125621\\list_prim_tripl");
    
//     if (dnaList.empty()) {
//         std::cerr << "No DNA sequences found in the file." << std::endl;
//         return ;
//     }

//     size_t num_sequences = dnaList.size();
//     std::vector<std::string> primss(2 * num_sequences);  
   
//     for (size_t i = 0; i < num_sequences; ++i) {
//         primss[i] = dnaList[i];              
//         primss[num_sequences + i] = dnaList[i]; 
//     }

// 	int cnt=0;
//     std::cout << "Extracted DNA Sequences and Their Complements:\n";
//     for (size_t i = 0; i < 2 * num_sequences; ++i) {
//         std::cout << "prims[" << i << "] = \"" << primss[i] << "\";\n";
// 		cnt++;
// 		if (cnt > 10) {
//             return;  
//         }
//     }
	string prims[6];
	prims[0] = "TGATGGCTTTATGGGTAGAATT";
	prims[1] = "TGTGCCTTTCAACTCTCATG";
	prims[2] = "CCAGTTGCGTCACCAAATGAATG";
	
	prims[3] = polindrom(prims[0]);
	prims[4] = polindrom(prims[1]);
	prims[5] = polindrom(prims[2]);

	int i,j,k,d,d1, prsz1;
//	string pattern, path1 = "C:\\programs\\Marcela\\Pr1\\SEQs\\";
	string pattern, path1 = "dna_request\\";
	string fname1 = path1 + remove_extension(input_file_path)+"\\"+input_file_path;
	string fname3 = path1 + remove_extension(input_file_path)+"\\";
//	string fname2 = path1 + "seqdump.txt";
	string fname2 = "SEQss\\New viruses for Zecharia1.txt";
	vector<ListFasta> dna_mas = GetFileListFasta(fname2,path1 + remove_extension(input_file_path)+"\\");
	vector<int> p[6];
	string dna1 = GetFileFasta(fname1,path1 + remove_extension(input_file_path)+"\\");

	ofstream outfile[6];

	for (k = 0; k < 6; k++)
	{
		outfile[k].open(fname3+prims[k]);

		prsz1 = prims[k].length();
		pattern = prims[k];
		d = 0;
//		for (i = 41; i < dna_mas.size(); i++)
		for (i = 0; i < dna_mas.size(); i++)
		{
			d1 = find_closest_dist(dna_mas[i].dna, pattern);
			d += d1;

			outfile[k] << i << "\t" << dna_mas[i].header << "\n" << d1 << "\n";
			for (j = 0; j < seq_glob1.size(); j++)
				outfile[k] << j << ")\t" << seq_glob1[j] << "\n";
		}
		p[k].push_back(d);

		outfile[k].close();

		continue;

		for (j = 0; j <= dna1.length() - prsz1; j = j+ prsz1)
		{
			cerr << k << " " << j << "\n";
			pattern = dna1.substr(j, prsz1);
			d = 0;
			//for (i = 41; i < dna_mas.size(); i++)
			for (i = 0; i < 41; i++)
			{
				d1 = find_closest_dist(dna_mas[i].dna, pattern);
				d += d1;
			}
			p[k].push_back(d);
		}
	}

	return;

	// for (k = 0; k < 3; k++)
	// {
	// 	outfile[k].open(prims[k]);
	// 	for (i = 0; i < p[k].size(); i++)
	// 		outfile[k] << i << "\t" << p[k][i] << "\n";
	// 	outfile[k].close();
	// }
}

int Prim1::find_closest_dist(string s1, string s2)
{
	seq_glob1.clear();
	int i,j,d,min = s2.length();
	for (i = 0; i <= s1.length() - s2.length(); i++)
	{
		d = 0;
		for (j = 0; j < s2.length(); j++)
		{
			if (s1[i + j] != s2[j])
			{
				d++;
//				if (d == min) break;
			}
		}

		if (d == 5)
			seq_glob1.push_back(s1.substr(i, s2.length()));

		if (d < min)
			min = d;

		if (min == 0) break;
			
	}
	return min;
}

string Prim1::polindrom(string s)
{
	int i;
	string ss = "";

	for (i = 0; i < s.length(); i++)
	{
		if (s[s.length() - i - 1] == 'A')
			ss = ss + 'T';
		if (s[s.length() - i - 1] == 'T')
			ss = ss + 'A';
		if (s[s.length() - i - 1] == 'C')
			ss = ss + 'G';
		if (s[s.length() - i - 1] == 'G')
			ss = ss + 'C';
	}

	return (ss);

}

