#ifndef SimpleHistSVC_h
#define SimpleHistSVC_h
#include <map>
#include <string>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>

class SimpleHistSVC {
    protected:
        std::map<std::string,TH1F *> histsDB_1d;
        std::map<std::string,TH2F *> histsDB_2d;
        std::string processName;
        TFile *output_file;    
    
    public:
        SimpleHistSVC();
        ~SimpleHistSVC();
        void BookFillHist(std::string name, int nbins, float start, float end, float value);
        void BookFillHist(std::string name, int nbinsX, float startX, float endX, int nbinsY, float startY, float endY,float x, float y);
        void SetProcessTag(std::string name);
        void Init();
        void BookFile(TFile *file);
        void Write();
        

};
#endif
