
#ifndef TCOMSTATISTICS_H
#define	TCOMSTATISTICS_H


#define EN_STATISTICS 1

#include <vector>
#include <fstream>
#include <sstream>

#include "TComDataCU.h"

using namespace std;

typedef vector<pair<string, double> > stats_item;

class TComStatistics {

public:

	static stats_item occurrences;
	static stats_item bestChoices;
	static ofstream statsFile;
	static ofstream bestChoicesFile;
        static int currentPOC;
        static int nCU;
        static string inputPath;
        static bool startEncoding;

        TComStatistics();
	static void addOccurrence(string, double);
	static void addBestChoice(string, double);
	static void setEncCU(TComDataCU*);
	static void setEncPU(TComDataCU*, unsigned int);
	static void setCompPU(TComDataCU*, int);
	static void setEncTU(TComDataCU*, int w, int h, bool isLuma);
	static void setCompTU(TComDataCU*, int w, int h, bool isLuma);
	static void reportStatistics();
        static void writeOnCsv(stats_item stats, ofstream &file);
        
        static void adjustDimensions(PartSize pSize, int& w, int& h, int pIdx);

};

#endif	/* TCOMSTATISTICS_H */

