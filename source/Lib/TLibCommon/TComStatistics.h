
#ifndef TCOMSTATISTICS_H
#define	TCOMSTATISTICS_H


#define EN_STATISTICS 1
#define PRINT_INTRA 0

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
	static stats_item TZSearchStats;
        
	static ofstream statsFile;
	static ofstream bestChoicesFile;
	static ofstream TZStatisticsFile;
        static int currentPOC;
        static int nCU;
        static string inputPath;
        static bool startEncoding;
        static unsigned TZSearchRounds;

        TComStatistics();
	
        static void addOccurrence(string, double);
	static void addBestChoice(string, double);
	static void setEncCU(TComDataCU*);
	
        static void setEncPU(TComDataCU*, unsigned int);
        static void setCompPU(TComDataCU*, string, unsigned int,unsigned int);

        
	static void setEncTU(TComDataCU*, int w, int h, bool isLuma);
        static void setCompTU(TComDataCU*, string, unsigned int w,unsigned int h, bool isLuma);
	
        static void setTZStep(int w, int h, int step, bool taken);
        static void addTZStatistics(string name, double n);
        
        static void reportStatistics();
        static void writeOnCsv(stats_item stats, ofstream &file);
        static void clearStats();

        static void adjustDimensions(PartSize pSize, int& w, int& h, int pIdx);

};

#endif	/* TCOMSTATISTICS_H */

