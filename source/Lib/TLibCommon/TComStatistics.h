
#ifndef TCOMSTATISTICS_H
#define	TCOMSTATISTICS_H


#define EN_STATISTICS 1

#include <vector>
#include <fstream>
#include <sstream>

#include "TComDataCU.h"

using namespace std;


class TComStatistics {

public:

	static vector<pair<string, double> > occurrences;
	static vector<pair<string, double> > bestChoices;
	static ofstream statsFile;
	static ofstream bestChoicesFile;
        static int currentPOC;
        static int nCU;
        static bool startEncoding;

        TComStatistics();
	static void addOccurrence(string, double);
	static void addBestChoice(string, double);
	static void setEncPU(TComDataCU*, unsigned int);
	static void setCompPU(TComDataCU*, int);
	static void setEncTU(TComDataCU*, int w, int h, bool isLuma);
	static void setCompTU(TComDataCU*, int w, int h, bool isLuma);
	static void reportStatistics();
        
        static void adjustDimensions(PartSize pSize, int& w, int& h, int pIdx);

};

#endif	/* TCOMSTATISTICS_H */

