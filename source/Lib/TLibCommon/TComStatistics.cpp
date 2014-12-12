
#include "TComStatistics.h"
#include "TComPic.h"

stats_item TComStatistics::occurrences;
stats_item TComStatistics::bestChoices;
stats_item TComStatistics::TZSearchStats;
ofstream TComStatistics::statsFile;
ofstream TComStatistics::bestChoicesFile;
ofstream TComStatistics::TZStatisticsFile;
int TComStatistics::currentPOC;
int TComStatistics::nCU;
Pel* TComStatistics::origLumaPointer;

string TComStatistics::inputPath;
unsigned TComStatistics::TZSearchRounds;
bool TComStatistics::startEncoding;

using namespace std;

TComStatistics::TComStatistics() {

}


void TComStatistics::setCompPU(TComDataCU* pu, string mode, unsigned int w, unsigned int h){

    startEncoding = false;
    nCU=0;
    
    
    currentPOC = pu->getPic()->getPOC();

    stringstream sstr;
            
    sstr << mode << " " << w << "x" << h;
        
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
       
    UInt tu_depth = pu->getTransformIdx(partIdx);
    UInt tu_w = pu->getWidth(partIdx) >> tu_depth;
    UInt tu_h = pu->getHeight(partIdx) >> tu_depth;
    UInt nTU = (unsigned) (pu->getWidth(partIdx)*pu->getHeight(partIdx))/(tu_w*tu_h);
    
    if (pu->getWidth(partIdx) == 64){
        nTU = nTU * 4;
        tu_w >>= 1;
        tu_h >>= 1;
    }
    
    stringstream sstr;
    sstr << "TU " << tu_w << "x" << tu_h;
    addBestChoice(sstr.str(),(double) nTU*tu_w*tu_h/(64*64));
    


  for ( UInt uiPartIdx = 0, uiSubPartIdx = partIdx; uiPartIdx < uiNumPU; uiPartIdx++, uiSubPartIdx += uiPUOffset ){
    sstr.str( std::string() );
    sstr.clear();
    
    w = pu->getWidth(uiSubPartIdx);
    h = pu->getHeight(uiSubPartIdx);
    adjustDimensions (ePartSize, w,h,uiPartIdx);
    sstr << "PU ";
        
    int max_d = max(w, h);
    int min_d = min(w, h);
    
        if(pu->getIPCMFlag(partIdx))
            sstr << "PCM " << w << "x" << h;
        else if(pu->isIntra( partIdx ))
            sstr << "Intra " << w << "x" << h;
        else if (pu->isSkipped( partIdx))
            sstr << "SKIP " << w << "x" << h;
        else{

            if ( pu->getMergeFlag( uiSubPartIdx ) )
                sstr << "Merge " ;
            else
                sstr << "Inter ";
                    
            if (min_d == max_d/4 or min_d == 3*max_d/4)
                sstr << "AMP " << w << "x" << h;
            else
                sstr << "SMP " << w << "x" << h;

        }



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
    stats_item::iterator it;

    size_t pos = inputPath.rfind("/") + 1;
    size_t pos2 = inputPath.rfind(".");
    string tok2 = inputPath.substr (pos, pos2-pos);

    string statsPath = tok2 + "_statistics.csv";
    string bestPath = tok2 + "_bestChoices.csv";

    if(not(statsFile.is_open())){
        statsFile.open(statsPath.c_str(),ofstream::out);        
    }
    
    if(not(bestChoicesFile.is_open())){
        bestChoicesFile.open(bestPath.c_str(),ofstream::out);        
    }

       
    
        
    statsFile << "Frame " << currentPOC << endl;
    for(it = occurrences.begin(); it != occurrences.end(); it++){
        statsFile << it->first << "\t" << it->second << endl;
    }
    statsFile << "END" << endl;

    
    bestChoicesFile << "Frame " << currentPOC << endl;
    for(it = bestChoices.begin(); it != bestChoices.end(); it++){
        bestChoicesFile << it->first << "\t" << it->second << endl;
    }
    bestChoicesFile << "END" << endl;
    
    clearStats();
}
void TComStatistics::clearStats(){
    bestChoices.clear();
    occurrences.clear();

}

void TComStatistics::adjustDimensions(PartSize pSize, int& w, int& h, int pIdx){
    switch(pSize){
        case SIZE_2NxN: h >>= 1; break;
        case SIZE_Nx2N: w >>= 1; break;
        case SIZE_NxN: w >>= 1; h >>= 1; break;
        case SIZE_nLx2N:
            if(pIdx == 0) w >>= 2;
            else w = 3*w/4;
            break;
        case SIZE_nRx2N:
            if(pIdx == 0) w = 3*w/4;    
            else w >>= 2;
            break;
        case SIZE_2NxnU:
            if(pIdx == 0) h >>= 2;
            else h = 3*h/4;
            break;
        case SIZE_2NxnD:
            if(pIdx == 0) h = 3*h/4;
            else h >>= 2;
            break;
        default:
            break;
    }
}

void TComStatistics::setTZStep(int w, int h, int step, bool taken){
            
    stringstream sstr;
    string mode;
    int n_w = max(w,h);
    int n_h = n_w;
    switch(step){
        case 0: // First Search
            mode = "First Search";
            break;
        case 1: // Raster Search
            mode = "Raster Search";
            break;
        case 2: // Refinement Search
            mode = "Refinement Search";
            break;
        default:
            mode = "Total Calls";
            break;
    }

 
            
    if (step == 0 or step == 2)
        sstr << "TZS " <<  mode << " " << n_w << "x" << n_h << " "  << TZSearchRounds ;
    else    
        sstr << "TZS " <<  mode << " " << n_w << "x" << n_h ;

  //  addTZStatistics(sstr.str(),(double) ((w*h*1.0)/(64.0*64)));   
    if (taken)
        addOccurrence(sstr.str(),(double) 1);   
    else
        addOccurrence(sstr.str(),(double) 0);   

    TZSearchRounds = 0;
}
      
/*void TComStatistics::calcPUSobel(TComDataCU* cu, TComPicYuv* origFrame){  
    Pel* lumaPointer = origFrame->getLumaAddr();
    Int stride = origFrame->getStride();
    Int cuWidth = cu->getWidth(0);
    Int cuHeight = cu->getHeight(0);
    Int cuX = cu->getCUPelX();
    Int cuY = cu->getCUPelY();
    lumaPointer += cuY*stride;
    double local_max_grad = -999;
    
    for(int i = 0; i < cuHeight and cuY+i < picHeight  ; i++){
        for(int j = 0; j < cuWidth and cuX+j < picWidth ; j++){
            if (j == 0 or j == cuWidth-1 or i == 0 or i == cuHeight-1 or  cuY+i == picHeight-1 or cuX+j == picWidth-1 ){
                 grad[cuY+i][cuX+j] = -1;
            }
            else{
                int s00 = *(&lumaPointer[j+cuX]-stride-1);
                int s01 = *(&lumaPointer[j+cuX]-stride);
                int s02 = *(&lumaPointer[j+cuX]-stride+1);
                int s10 = *(&lumaPointer[j+cuX]-1);
                int s11 = lumaPointer[j+cuX];
                int s12 = *(&lumaPointer[j+cuX]+1);
                int s20 = *(&lumaPointer[j+cuX]+stride-1);
                int s21 = *(&lumaPointer[j+cuX]+stride);
                int s22 = *(&lumaPointer[j+cuX]+stride+1);
                
                double gv = (-1.0*s00 - 2.0*s01 - 1.0*s02 + \
                            0*s10 +   0*s11 +   0*s12 + \
                          1.0*s20 + 2.0*s21 + 1.0*s22)/8.0;

                double gh = (-1.0*s00 -   0*s01 + 1.0*s02 + \
                          -2.0*s10 +  0*s11 + 2.0*s12 + \
                          -1.0*s20 +  0*s21 + 1.0*s22)/8.0;
                double gr = sqrt(gv*gv + gh*gh);
                
                max_grad = max(gr, max_grad);
                local_max_grad = max(gr, local_max_grad);
                grad[cuY+i][cuX+j] = gr; // > 255? 255 :  sqrt(gv*gv + gh*gh);
                magnitude[cuY+i][cuX+j] = atan2(gv,gh);


            }
        }
        lumaPointer += stride;
    }
        
    lumaPointer = origFrame->getLumaAddr();
    lumaPointer += cuY*stride;
    
    xcalcCTUDivisions(local_max_grad, cuX, cuY, cuWidth, cuHeight);

}  */
