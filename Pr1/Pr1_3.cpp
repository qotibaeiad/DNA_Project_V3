#ifndef _INC_Pr1_3
#define _INC_Pr1_3

#include "Pr1.h"
bool Prim1::prim_filter1(string seq)
{
	int i,j,n;
	double d;
	n = 0;
	for (i = 0; i < seq.size(); i++)
	{
		if (seq[i] == 'C' || seq[i] == 'G')
			n++;

		if (seq[i] != 'A' && seq[i] != 'T' && seq[i] != 'C' && seq[i] != 'G')
			return 0;
	}

	d = (double)100 * n / double(seq.size());
	if (d < 40 || d > 60) return 0;

	//check one-letter repeats 
	for (i = 0; i <= seq.size() - 4; i++)
	{
		for (j = 1; j < 4; j++)
			if (seq[i] != seq[i + j]) break;

		if (j == 4) return 0;
	}

	//check two-letter repeats
	for (i = 0; i <= seq.size() - 5; i++)
	{
		for (j = 2; j <= 4; j = j+2)
			if (seq[i] != seq[i + j] || seq[i+1] != seq[i+1+j]) break;

		if (j == 6) return 0;
	}

	if (!prim_5_check(seq) && !prim_3_check(seq))
		return 0;

	return 1;
}

bool Prim1::prim_3_check(string seq)
{
	int i,n;
	
	i = seq.size() - 1;
	if (seq[i] != 'C' && seq[i] != 'G')
		return 0;

	n = 0;
	for (i = i-1; i >= seq.size()-5; i--)
	{
		if (seq[i] == 'C' || seq[i] == 'G')
			n++;
	}

	if (n < 2) return 0;
	return 1;
}

bool Prim1::prim_5_check(string seq)
{
	int i, n;

	i = 0;
	if (seq[i] != 'C' && seq[i] != 'G')
		return 0;

	n = 0;
	for (i = 1; i <= 4; i++)
	{
		if (seq[i] == 'C' || seq[i] == 'G')
			n++;
	}

	if (n < 2) return 0;
	return 1;
}

int Prim1::prime_Tm(string seq)
{
	int i, n;
	n = 0;
	for (i = 0; i < seq.size(); i++)
	{
		if (seq[i] == 'C' || seq[i] == 'G')
			n = n+4;

		if (seq[i] == 'A' || seq[i] == 'T')
			n = n + 2;
	}

	return n;
}


#endif  // _INC_Pr1_3