
#include "TComStatistics.h"
#include "TComPic.h"

vector<pair<string, double> > TComStatistics::occurrences;
vector<pair<string, double> > TComStatistics::bestChoices;
ofstream TComStatistics::statsFile;
ofstream TComStatistics::bestChoicesFile;
int TComStatistics::currentPOC;
int TComStatistics::nCU;

bool TComStatistics::startEncoding;

using namespace std;

TComStatistics::TComStatistics() {

}


void TComStatistics::setCompPU(TComDataCU* pu, int partIdx){
    int w,h,x,y;
    
    startEncoding = false;
    nCU=0;
    
    pu->getPartPosition(partIdx,x,y,w,h);
    currentPOC = pu->getPic()->getPOC();
    
    stringstream sstr;
    
    if(pu->isIntra( partIdx ))
        sstr << "Intra " << w << "x" << h;
    else        
        sstr << "Inter " << w << "x" << h;
    addOccurrence(sstr.str(),(double) w*h/(64.0*64));

}

void TComStatistics::setEncPU(TComDataCU* pu, unsigned int partIdx){

    if(not(startEncoding)) return;    
    
    int w,h;
   
    currentPOC = pu->getPic()->getPOC();
    
    PartSize ePartSize = pu->getPartitionSize( partIdx );
    UInt uiNumPU = ( ePartSize == SIZE_2Nx2N ? 1 : ( ePartSize == SIZE_NxN ? 4 : 2 ) );
    UInt uiDepth = pu->getDepth( partIdx );
    UInt uiPUOffset = ( g_auiPUOffset[UInt( ePartSize )] << ( ( pu->getSlice()->getSPS()->getMaxCUDepth() - uiDepth ) << 1 ) ) >> 4;

  for ( UInt uiPartIdx = 0, uiSubPartIdx = partIdx; uiPartIdx < uiNumPU; uiPartIdx++, uiSubPartIdx += uiPUOffset ){
    stringstream sstr;
   
    w = pu->getWidth(uiSubPartIdx);
    h = pu->getHeight(uiSubPartIdx);
    adjustDimensions (ePartSize, w,h,uiPartIdx);


        if(pu->getIPCMFlag(partIdx))
            sstr << "PCM " << w << "x" << h;
        else if(pu->isIntra( partIdx ))
            sstr << "Intra " << w << "x" << h;
        else if (pu->isSkipped( partIdx))
            sstr << "SKIP " << w << "x" << h;
        else if ( pu->getMergeFlag( uiSubPartIdx ) )
            sstr << "Merge " << w << "x" << h;
        else
            sstr << "Inter " << w << "x" << h;

    
        addBestChoice(sstr.str(),(double) w*h/(64*64));
    
  }
}

void TComStatistics::addOccurrence(string item, double n){
    vector<pair<string, double> >::iterator it;

    for(it = occurrences.begin(); it != occurrences.end(); it++){
        if (it->first == item){
            it->second += n;
            return;
        }
    }
    if(it == occurrences.end()){
        occurrences.push_back(make_pair(item, n));	
    }
}


void TComStatistics::addBestChoice(string item, double n){
    vector<pair<string, double> >::iterator it;

    for(it = bestChoices.begin(); it != bestChoices.end(); it++){
        if (it->first == item){
            it->second += n;
            return;
        }
    }
    if(it == bestChoices.end()){
        bestChoices.push_back(make_pair(item, n));	
    }
}

void TComStatistics::reportStatistics(){
    vector<pair<string, double> >::iterator it;

    if(not(statsFile.is_open())){
        statsFile.open("statistics.csv",ofstream::out);        
    }

    statsFile << "Frame " << currentPOC << endl;
    for(it = occurrences.begin(); it != occurrences.end(); it++){
        statsFile << it->first << "\t" << it->second << endl;
    }
    statsFile << endl;
    
    occurrences.clear();
    
        
    if(not(bestChoicesFile.is_open())){
        bestChoicesFile.open("bestChoices.csv",ofstream::out);        
    }

    bestChoicesFile << "Frame " << currentPOC << endl;
    bestChoicesFile << "nCU " << nCU << endl;
    for(it = bestChoices.begin(); it != bestChoices.end(); it++){
        bestChoicesFile << it->first << "\t" << it->second << endl;
    }
    bestChoicesFile << endl;
    
    bestChoices.clear();

}


void TComStatistics::adjustDimensions(PartSize pSize, int& w, int& h, int pIdx){
    switch(pSize){
        case SIZE_2NxN:
            h >>= 1;
            break;
        case SIZE_Nx2N:
            w >>= 1;
            break;

        case SIZE_NxN:
            w >>= 1;
            h >>= 1;
            break;
        case SIZE_nLx2N:
            if(pIdx == 0)
                w >>= 2;
            else
                w = 3*w/4;
            break;
        case SIZE_nRx2N:
            if(pIdx == 0)
                w = 3*w/4;    
            else
                w >>= 2;
            break;
        case SIZE_2NxnU:
            if(pIdx == 0)
                h >>= 2;
            else
                h = 3*h/4;
            break;
        case SIZE_2NxnD:
            if(pIdx == 0)
                h = 3*h/4;
            else
                h >>= 2;
            break;
        default:
            break;
    }
}