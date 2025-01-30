#ifndef _INC_Pr1_4
#define _INC_Pr1_4

#include "Pr1.h"
void Prim1::select_pairs(string dna, int prsz1 ,string path ,string path2)
{
	int i,j;
	map<string, int>::iterator it;
	string pattern, dc;
	ofstream outfile;
	int dist_min = 150;
	int dist_max = 300;
	int dt = 3;

	int nmas1 = 6;
	string mas1[6];
	int mas2[6];
	mas1[0] = "TTCGTATTGCTGGACACCATCT";
	mas1[1] = "TTCGTGATGTAGCAACAGTGATTTC";
	mas1[2] = "ACGCTGTGACATCAAGGACCTGCCTAA";
	mas1[3] = "GATGGCTTTATGGGTAGAATTCGA";
	mas1[4] = "CTTCATGAGAGTTGAAAGGCACAT";
	mas1[5] = "CAGTTGCGTCACCAAA";

	prim_pair prp;
	std::cout << "slect pair path1:-   " << path << std::endl;
	std::cout << "slect pair path2:-   " << path2 << std::endl;
	std::cout << "slect pair path1 + path2:-   " << ".\\"+path+path2 << std::endl;
	//delete repeated fragments
	for (i = 0; i <= dna.size() - prsz1; i++)
	{
		pattern = dna.substr(i, prsz1);
		it_glob = mp1_glob.find(pattern);
		if (it_glob != mp1_glob.end())
		{
			if (it_glob->second == -1)
				it_glob->second = 1;
			else
				mp1_glob.erase(it_glob);
		}
	}

	for (i = 0; i < map_list1.size(); i++)
	{
		map_list1[i].pos = -1;

		if (map_list1[i].num != -1) continue;
		pattern = map_list1[i].seq;
		it = mp1_glob.find(pattern);
		if (it != mp1_glob.end())
			it->second = i;
	}

	outfile.open(path + "\\mp4_c");
	i = 1;
	for (auto& x : mp1_glob) {
		outfile << i << "," << x.first << ", " << x.second << endl;
		i++;
	}
	outfile.close();

	dc = dna;
	for (i = 0; i < dc.size(); i++)
		dc[i] = 'X';

	for (i = 0; i <= dna.size() - prsz1; i++)
	{
		pattern = dna.substr(i, prsz1);
		it = mp1_glob.find(pattern);
		if (it == mp1_glob.end()) continue;
		if (it->second == -1) continue;
		map_list1[it->second].pos = i;
		if (map_list1[it->second].r) dc[i] = 'r';
		if (map_list1[it->second].l) dc[i] = 'l';
		if (map_list1[it->second].r && map_list1[it->second].l) dc[i] = 'o';
	}


	prp.cons3 = -1;
	prp.pos3 = -1;
	for (i = 0; i <= dc.size() - (dist_min + prsz1); i++)
	{
		if (dc[i] != 'r' && dc[i] != 'o') continue;
		prp.seq1 = dna.substr(i, prsz1);
		prp.pos1 = i;
		prp.Tm1 = prime_Tm(prp.seq1);
		for (j = i + dist_min; j <= i + dist_max; j++)
		{
			if (j == dc.size() - prsz1 + 1) break;
			if (dc[j] != 'l' && dc[j] != 'o') continue;
			prp.seq2 = dna.substr(j, prsz1);
			prp.Tm2 = prime_Tm(prp.seq2);
			if (abs(prp.Tm2 - prp.Tm1) > dt) continue;
			prp.pos2 = j;
			prim_pair_list1.push_back(prp);
		}
	}


//mapping regions
	vector <int> pos_decr;
	int b = 0;
	for (i = 0; i < dna.size(); i++)
		pos_decr.push_back(b);

	for (i = 0; i < prim_pair_list1.size(); i++)
	{
//		for (j = 0; j < prim_pair_list1[i].seq1.length(); j++)
//			pos_decr[prim_pair_list1[i].pos1 + j] = 1;

//		for (j = 0; j < prim_pair_list1[i].seq2.length(); j++)
//			pos_decr[prim_pair_list1[i].pos2 + j] = 1;
		pos_decr[prim_pair_list1[i].pos1] = 1;
		pos_decr[prim_pair_list1[i].pos2] = 1;
	}

	int k = 0;

	outfile.open(path+"\\regions");
	for (i = 1; i < dna.size() - 1; i++)
	{
		if (pos_decr[i] == 0)
		{
			if (pos_decr[i - 1] == 1)
				outfile << ";\n";
			if (pos_decr[i + 1] == 0) continue;
			if (pos_decr[i + 1] == 1)
			{
				k++;
				outfile << k << " " << i+1 << " ";
			}
		}
		
		if (pos_decr[i] == 1)
			outfile << dna[i];
	}

	outfile.close();

	int kmin, j1, i1, t;
	pattern = "";
	//string path1 = "C:\\Users\\qotib\\OneDrive\\Desktop\\DNA_PROJECT\\Pr1\\SEQs\\";
	//string fname2 = path1 + "seqdump.txt";
	string fname2 = ".\\"+path+path2;
	std::cout << "slect pair fname2   " << fname2 << std::endl;

	vector<ListFasta> dna_mas = GetFileListFasta(fname2,path);
	
	
/*
	for (i1 = 0; i1 < nmas1; i1++)
	{
		pattern = mas1[i1];
		mas2[i1] = 0;

		//for every genome
		for (i = 41; i < dna_mas.size(); i++)
//		for (i = 41; i < 50; i++)
		{
			kmin = pattern.length();
			//for every position
			for (j = 0; j <= dna_mas[i].dna.size() - pattern.length(); j++)
			{
				k = 0;
				for (j1 = 0; j1 < pattern.length(); j1++)
				{
					if (dna_mas[i].dna[j + j1] != pattern[j1])
					{
						k++;
						if (k >= kmin) break;
					}
				}//j1
				if (k < kmin) kmin = k;
			}//j

			mas2[i1] += kmin;
		}
	}

	outfile.open("six_seq");
	//	outfile << dc << "\n";

	for (i = 0; i < nmas1; i++)
	{
		outfile << i + 1 << " " << mas1[i] << " " << mas1[i].length() << " " << mas2[i] 
			<< " " << mas2[i]/mas1[i].length() << "\n";
	}
	
	outfile.close();

*/

// 	return;

	//for every primer
	for (i1 = 0; i1 <= dna.size() - prsz1; i1++)
	{
		if (pos_decr[i1] == 0) continue;
		pattern = dna.substr(i1, prsz1);
		cerr << "\n" << i1 << " " << dna.size();

		//for every genome
		for (i = 41; i < dna_mas.size(); i++)
		{
			kmin = prsz1;
			//for every position
			for (j = 0; j <= dna_mas[i].dna.size() - prsz1; j++)
			{
				k = 0;
				for (j1 = 0; j1 < prsz1; j1++)
				{
					if (dna_mas[i].dna[j + j1] != pattern[j1])
					{
						k++;
						if (k >= kmin) break;
					}
				}//j1
				if (k < kmin) kmin = k;
			}//j

			pos_decr[i1] += kmin;
		}
	}

	outfile.open(path+"\\list_prim_1");
	//	outfile << dc << "\n";

	for (i = 0; i < prim_pair_list1.size(); i++)
	{
		prim_pair_list1[i].cons1 = pos_decr[prim_pair_list1[i].pos1];
		prim_pair_list1[i].cons2 = pos_decr[prim_pair_list1[i].pos2];
		outfile << i + 1 << ")\n" << prim_pair_list1[i].seq1 << " "
			<< prim_pair_list1[i].pos1 << " " << prim_pair_list1[i].Tm1 << " " << prim_pair_list1[i].cons1 << "\n";
		outfile << prim_pair_list1[i].seq2 << " " << prim_pair_list1[i].pos2 << " "
			<< prim_pair_list1[i].Tm2 << " " << prim_pair_list1[i].cons2 << "\n\n";
	}
	outfile.close();

	outfile.open(path+"\\list_prim_tripl");
	//	outfile << dc << "\n";
	k = 0;
	int pleng;
	int smin = -1;
	for (i = 0; i < prim_pair_list1.size(); i++)
	{
		for (j = prim_pair_list1[i].pos1 + prsz1; j < prim_pair_list1[i].pos2 - prsz1; j++)
		{
			if (pos_decr[j] == 0) continue;
			if (dna[j] == 'A' || dna[j] == 'T') continue;
			t = prime_Tm(dna.substr(j, prsz1));
			pleng = prsz1;
			while (t < 66)
			{
				pleng++;
				t = prime_Tm(dna.substr(j, pleng));
			}
			if (dna[j + pleng - 1] == 'C' || dna[j + pleng - 1] == 'G') continue;
			if (t < 58 || t > 68) continue;
			if (t - prim_pair_list1[i].Tm2 < 10) continue;
			if (t - prim_pair_list1[i].Tm1 < 10) continue;

			if (prim_pair_list1[i].cons3 == -1)
				kmin = pos_decr[j] + 1;

			if (pos_decr[j] < kmin)
			{
				kmin = pos_decr[j];
				prim_pair_list1[i].cons3 = kmin;
				prim_pair_list1[i].pos3 = j;
				prim_pair_list1[i].pleng3 = pleng;
			}
		}

		if (prim_pair_list1[i].cons3 == -1) continue;

		if (smin == -1)
			smin = prim_pair_list1[i].cons1 + prim_pair_list1[i].cons2 + prim_pair_list1[i].cons3 + 1;

		if (smin > prim_pair_list1[i].cons1 + prim_pair_list1[i].cons2 + prim_pair_list1[i].cons3)
		{
			smin = prim_pair_list1[i].cons1 + prim_pair_list1[i].cons2 + prim_pair_list1[i].cons3;
			j1 = i;
		}

		k++;
		prim_pair_list1[i].seq3 = dna.substr(prim_pair_list1[i].pos3, prim_pair_list1[i].pleng3);
		prim_pair_list1[i].Tm3 = prime_Tm(prim_pair_list1[i].seq3);

		outfile << k << ")\n" << prim_pair_list1[i].seq1 << " "
			<< prim_pair_list1[i].pos1 << " " << prim_pair_list1[i].Tm1 << " " << prim_pair_list1[i].cons1 << "\n";
		outfile << prim_pair_list1[i].seq2 << " " << prim_pair_list1[i].pos2 << " "
			<< prim_pair_list1[i].Tm2 << " " << prim_pair_list1[i].cons2 << "\n";
		outfile << prim_pair_list1[i].seq3 << " " << prim_pair_list1[i].pos3 << " "
			<<  prim_pair_list1[i].cons3 << "\n\n";
	}

	i = j1;
	outfile << "\n\n\nTHE BEST " << smin;
	outfile << "\n" << prim_pair_list1[i].seq1 << " "
		<< prim_pair_list1[i].pos1 << " " << prim_pair_list1[i].Tm1 << " " << prim_pair_list1[i].cons1 << "\n";
	outfile << prim_pair_list1[i].seq2 << " " << prim_pair_list1[i].pos2 << " "
		<< prim_pair_list1[i].Tm2 << " " << prim_pair_list1[i].cons2 << "\n";
	outfile << prim_pair_list1[i].seq3 << " " << prim_pair_list1[i].pos3 << " " << prim_pair_list1[i].Tm3 << " "
		<< prim_pair_list1[i].cons3 << "\n\n";


	outfile.close();




	   
}

#endif  // _INC_Pr1_4