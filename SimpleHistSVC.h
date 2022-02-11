#ifndef SimpleHistSVC_h
#define SimpleHistSVC_h
#include <map>
#include <string>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TH3F.h>

class SimpleHistSVC {
    protected:
        std::map<std::string,TH1F *> histsDB_1d;
        std::map<std::string,TH2F *> histsDB_2d;
        std::map<std::string,TH3F *> histsDB_3d;
        std::string processName;
        int caloIndex;
        int xtalIndex;
        int hitStatus;
        int timeTag;

        std::string GetFullName(std::string name);


        TDirectory *output_file;    
    private:
        int default_int;
    
    public:
        SimpleHistSVC();
        ~SimpleHistSVC();
        void BookFillHist(std::string name, int nbins, float start, float end, float value);
        void BookFillHist(std::string name, int nbinsX, float startX, float endX, int nbinsY, float startY, float endY,float x, float y);
        void BookFillHist(std::string name, int nbinsX, float startX, float endX, int nbinsY, float startY, float endY,int nbinsZ, float startZ, float endZ, float x, float y, float z);
        void SetProcessTag(std::string name);
        void SetCaloTag(int caloIndex);
        void ResetCaloTag();
        void SetXtalTag(int xtalIndex);
        void SetStatusTag(int hitStatus);
        void SetTimeTag(int timeTag);
        void ResetTimeTag();
        void InitNameTags();
        void Init();
        void BookFile(TDirectory *file);
        void Write();

        

};
#endif
