#include <string>        
#include <vector>        
#include <math.h>	
#include <iomanip>
#include <unistd.h>
#include <limits.h>
#include <limits>
#include <Rand.h>
#include <fstream>

#include <TH1D.h>        
#include <TH2D.h>        
#include <TFrame.h> 
#include <TGraph.h> 
#include <TGraphErrors.h> 
#include <TPaveText.h> 
#include <TCanvas.h> 
#include <TSystemDirectory.h> 
#include <TList.h> 
#include <THStack.h> 
#include <TLegend.h> 
#include <TLine.h> 
#include <TFile.h> 
#include <TNtuple.h> 
#include <TStyle.h> 
#include <TString.h> 
#include <THStack.h> 
#include <TPad.h> 
#include <TPaveStats.h> 
#include <TAttFill.h>

#include <BinnedED.h>        
#include <BinnedEDGenerator.h>
#include <SystematicManager.h>
#include <BinnedNLLH.h>
#include <FitResult.h>
#include <Minuit.h>
#include <DistTools.h>
#include <MetropolisHastings.h>
#include <Minuit.h>
#include <Convolution.h>
#include <Scale.h>
#include <BoolCut.h>
#include <BoxCut.h>
#include <Gaussian.h>
#include <ParameterDict.h>
#include <ContainerTools.hpp>


void padPDFs(std::vector<BinnedED>& binnedEDList){
    std::cout<<"Padding Now"<<std::endl;

    // std::vector<TH1D> tempList;
    for(int i=0;i<binnedEDList.size();i++){

        std::vector<double> bincontent =binnedEDList.at(i).GetBinContents();
        std::vector<double> new_bincontent;
        std::cout<<"Size of the bin contents vector = "<<bincontent.size()<<std::endl;
        for(int j =0; j<bincontent.size();j++){
            if(bincontent[j]==0){
                new_bincontent.push_back(std::numeric_limits< double >::min());
                // new_bincontent.push_back(0.00000001);
                // new_bincontent.push_back(1);
            }else {
                new_bincontent.push_back(bincontent[j]);
            }
        }

        binnedEDList[i].SetBinContents(new_bincontent);

    }

}
TH1D* diffHist(TH1D * h1,TH1D * h2){
    /*This Finds the fractional difference between two histograms, (see code for details). 
    */
    double minBin=h1->GetXaxis()->GetXmin();
    double maxBin=h1->GetXaxis()->GetXmax();
    double sliceWidth=h1->GetXaxis()->GetBinWidth(1);
    double numOfBins=h1->GetNbinsX();

    TH1D* rhist = new TH1D("rhist","",numOfBins,minBin,maxBin);
    for(double i=0;i<numOfBins;i++){
        double h1cont=h1->GetBinContent(i);
        double h2cont=h2->GetBinContent(i);
        double weight;
        if (h1cont!=0 && h1cont-h2cont!=0) {
            // weight= h2cont/h1cont;
            weight= (h1cont-h2cont)/h1cont;
        } else {
            weight= 10000000;
        }
        rhist->SetBinContent(i,weight);
    }
    return rhist;
}

int function(){

    Rand::SetSeed(0);

    Gaussian gaus1(2, 0.55);
    Gaussian gaus2(2, 0.5);
    Gaussian gaus3(8, 0.5);
    Gaussian gaus4(8, 0.55);

    AxisCollection axes;
    axes.AddAxis(BinAxis("axis1", 0, 10 , 100));

    BinnedED pdf3("a_data", DistTools::ToHist(gaus1, axes));
    BinnedED pdf4("b_data", DistTools::ToHist(gaus4, axes));
    BinnedED pdf1("a_mc", DistTools::ToHist(gaus2, axes));
    BinnedED pdf2("b_mc", DistTools::ToHist(gaus3, axes));

    pdf1.SetObservables(0);
    pdf2.SetObservables(0);    
    pdf3.SetObservables(0);
    pdf4.SetObservables(0);

    pdf1.Scale(40000);
    pdf2.Scale(40000);    


    std::vector<BinnedED> dataPdfs;
    dataPdfs.push_back(pdf3);
    dataPdfs.push_back(pdf4);
    padPDFs(dataPdfs);


    BinnedEDGenerator dataGen;
    dataGen.SetPdfs(dataPdfs);
    // std::vector<double> rates(2,50000);
    std::vector<double> rates;
    rates.push_back(60000);
    rates.push_back(60000);
    dataGen.SetRates(rates);

    BinnedED fakeData= dataGen.ExpectedRatesED();
    // BinnedED fakeData= dataGen.PoissonFluctuatedED();
    {
        TCanvas c1;
        c1.SetLogy();
        // TH1D base("base","base",200,0,10);
        TH1D dataHist = DistTools::ToTH1D(fakeData);
        TH1D hist1 =  DistTools::ToTH1D(pdf1);
        TH1D hist2 =  DistTools::ToTH1D(pdf2);
        dataHist.Draw();
        hist1.Draw("same");
        hist2.Draw("same");
        c1.Print("setup.png");
    }

    std::vector<BinnedED> mcPdfs;
    mcPdfs.push_back(pdf1);
    mcPdfs.push_back(pdf2);

    padPDFs(mcPdfs);
    pdf1.Normalise();
    pdf2.Normalise();    

    ObsSet  obsSet(0);

    Convolution* conv_a = new Convolution("conv_a");
    // Gaussian gaus_a(0,1,"gaus_a"); 
    Gaussian* gaus_a = new Gaussian(0,1,"gaus_a"); 

    gaus_a->RenameParameter("means_0","gaus_a_1");
    gaus_a->RenameParameter("stddevs_0","gaus_a_2");

    // conv_a->SetFunction(&gaus_a);
    conv_a->SetFunction(gaus_a);

    conv_a->SetAxes(axes);
    conv_a->SetTransformationObs(obsSet);
    conv_a->SetDistributionObs(obsSet);
    conv_a->Construct();

    Convolution* conv_b = new Convolution("conv_b");
    // Gaussian gaus_b(0,1,"gaus_b"); 
    Gaussian * gaus_b = new Gaussian(0,1,"gaus_b"); 
    gaus_b->RenameParameter("means_0","gaus_b_1");
    gaus_b->RenameParameter("stddevs_0","gaus_b_2");
    conv_b->SetFunction(gaus_b);
    conv_b->SetAxes(axes);
    conv_b->SetTransformationObs(obsSet);
    conv_b->SetDistributionObs(obsSet);
    conv_b->Construct();

    Scale* scale_a= new Scale("Scale_a");
    scale_a->RenameParameter("scaleFactor","scale_a_1");
    scale_a->SetAxes(axes);
    scale_a->SetScaleFactor(1.1);
    scale_a->SetTransformationObs(obsSet);
    scale_a->SetDistributionObs(obsSet);

    scale_a->Construct();

    Scale* scale_b= new Scale("Scale_b");
    scale_b->RenameParameter("scaleFactor","scale_b_1");
    scale_b->SetAxes(axes);
    scale_b->SetScaleFactor(1.2);
    scale_b->SetTransformationObs(obsSet);
    scale_b->SetDistributionObs(obsSet);

    scale_b->Construct();

    // Setting optimisation limits
    ParameterDict minima;
    minima["a_mc_norm"] = 10;               // normalisation of Bi210 in data set A
    minima["b_mc_norm"] = 10;               // normalisation of Bi210 in data set A
    minima["gaus_a_1"] = -0.1;               // normalisation of Bi210 in data set A
    minima["gaus_a_2"] = 0.000;               // normalisation of Bi210 in data set A
    minima["gaus_b_1"] = -0.1;               // normalisation of Bi210 in data set A
    minima["gaus_b_2"] = 0.000;               // normalisation of Bi210 in data set A
    // minima["scale_a_1"] = 0.7;               // normalisation of Bi210 in data set A
    // minima["scale_b_1"] = 0.7;               // normalisation of Bi210 in data set A

    ParameterDict maxima;
    maxima["a_mc_norm"] = 100000;               // normalisation of Bi210 in data set A
    maxima["b_mc_norm"] = 100000;               // normalisation of Bi210 in data set A
    maxima["gaus_a_1"] = 0.9;               // normalisation of Bi210 in data set A
    maxima["gaus_a_2"] = 0.8;               // normalisation of Bi210 in data set A
    maxima["gaus_b_1"] = 0.9;               // normalisation of Bi210 in data set A
    maxima["gaus_b_2"] = 0.8;               // normalisation of Bi210 in data set A
    // maxima["scale_a_1"] = 1.3;               // normalisation of Bi210 in data set A
    // maxima["scale_b_1"] = 1.3;               // normalisation of Bi210 in data set A

    Rand rand;

    ParameterDict initialerr;
    initialerr["a_mc_norm"] = 0;               // normalisation of Bi210 in data set A
    initialerr["b_mc_norm"] = 0;               // normalisation of Bi210 in data set A
    initialerr["gaus_a_1"] = 100.01;               // normalisation of Bi210 in data set A
    initialerr["gaus_a_2"] = 100.01;               // normalisation of Bi210 in data set A
    initialerr["gaus_b_1"] = 100.01;               // normalisation of Bi210 in data set A
    initialerr["gaus_b_2"] = 100.01;               // normalisation of Bi210 in data set A
    // initialerr["scale_a_1"] = 0.1;               // normalisation of Bi210 in data set A
    // initialerr["scale_b_1"] = 0.1;               // normalisation of Bi210 in data set A

    ParameterDict initialval;
    initialval["a_mc_norm"] = 60000;               // normalisation of Bi210 in data set A
    initialval["b_mc_norm"] = 60000;               // normalisation of Bi210 in data set A
    initialval["gaus_a_1"] = 0.01;               // normalisation of Bi210 in data set A
    initialval["gaus_a_2"] = 0.1;               // normalisation of Bi210 in data set A
    initialval["gaus_b_1"] = 0.01;               // normalisation of Bi210 in data set A
    initialval["gaus_b_2"] = 0.001;               // normalisation of Bi210 in data set A
    // initialval["scale_a_1"] = 1;               // normalisation of Bi210 in data set A
    // initialval["scale_b_1"] = 1;               // normalisation of Bi210 in data set A

    //Setting  up the combined likelihood.
    int BuffLow  =1;
    int BuffHigh =1;

    BinnedNLLH lh; 
    lh.SetBufferAsOverflow(false);
    lh.SetBuffer(0,BuffLow,BuffHigh);
    lh.SetDataDist(fakeData); // initialise with the data set
    lh.AddPdfs(mcPdfs);
    // lh.AddSystematic(scale_a,"aGroup");
    lh.AddSystematic(conv_a,"aGroup");
    lh.AddSystematic(conv_b,"bGroup");
    // lh.AddSystematic(scale_b,"bGroup");
    lh.AddDist(mcPdfs.at(0),std::vector<std::string>(1,"aGroup"));
    lh.AddDist(mcPdfs.at(1),std::vector<std::string>(1,"bGroup"));

    std::cout << "here" << std::endl;
    // lh.RegisterFitComponents();
    std::cout << "here" << std::endl;

    std::cout << lh.GetParameterNames().size() << std::endl;
    // for (int i = 0; i <lh.GetParameterNames().size(); ++i) {
    //     // std::cout << ContainerTools::GetValues(lh.GetParameterNames()) << std::endl;
    //     std::cout <<lh.GetParameterNames().at(i) << std::endl;
    // }


    Minuit min;
    // min.SetMethod("Simplex");
    // min.SetMethod("Minimize");
    min.SetMaxCalls(1000000);
    min.SetMinima(minima);
    min.SetMaxima(maxima);
    min.SetInitialValues(initialval);
    min.SetInitialErrors(initialerr);

    ParameterDict checker = lh.GetParameters();

    for (std::map<std::string, double >::const_iterator i = checker.begin(); i !=  checker.end(); ++i) {
       std::cout << i->first << "\t" << i->second << std::endl; 
    }
    
    std::cout << "About to Fit" << std::endl;
    FitResult result = min.Optimise(&lh);
    result.SetPrintPrecision(4);
    result.Print();
    ParameterDict bestResult = result.GetBestFit();
    for (ParameterDict::const_iterator i =bestResult.begin(); i != bestResult.end(); ++i) {
        std::cout << i->first<< " "<< i->second << std::endl;
    }
    

    {
        BinnedED BiHolder = mcPdfs.at(0);
        BinnedED PoHolder = mcPdfs.at(1);
        BinnedED BiResult;
        BinnedED PoResult;

        Scale BiScale("aScale");
        BiScale.SetScaleFactor( bestResult.at("scale_a_1") );
        BiScale.SetAxes(axes);
        BiScale.SetTransformationObs(obsSet);
        BiScale.SetDistributionObs(obsSet);
        BiScale.Construct();

        Convolution BiSmearer("aConv");
        BiSmearer.SetFunction(new Gaussian(bestResult.at("gaus_a_1"),bestResult.at("gaus_a_2")));
        BiSmearer.SetAxes(axes);
        BiSmearer.SetTransformationObs(obsSet);
        BiSmearer.SetDistributionObs(obsSet);
        BiSmearer.Construct();

        Scale PoScale("bScale");
        PoScale.SetScaleFactor(bestResult.at("scale_b_1"));
        PoScale.SetAxes(axes);
        PoScale.SetTransformationObs(obsSet);
        PoScale.SetDistributionObs(obsSet);
        PoScale.Construct();


        Convolution PoSmearer("bConv");
        PoSmearer.SetFunction(new Gaussian(bestResult.at("gaus_b_1"),bestResult.at("gaus_b_2")));
        PoSmearer.SetAxes(axes);
        PoSmearer.SetTransformationObs(obsSet);
        PoSmearer.SetDistributionObs(obsSet);
        PoSmearer.Construct();

        PoResult = PoScale( PoSmearer( PoHolder ) );
        BiResult = BiScale( BiSmearer( BiHolder ) );

        BiResult.Scale(bestResult.at("a_mc_norm"));
        PoResult.Scale(bestResult.at("b_mc_norm"));
 
        TH1D fakeDataHist;
        TH1D BiFit;
        TH1D PoFit;
        BiFit.Sumw2();
        PoFit.Sumw2();
        // fakeDataHist.Sumw2();
        fakeDataHist = DistTools::ToTH1D(fakeData);
        BiFit= DistTools::ToTH1D(BiResult);
        PoFit= DistTools::ToTH1D(PoResult);
        TH1D FullFit("FullFit","",BiFit.GetNbinsX(),BiFit.GetXaxis()->GetXmin(),BiFit.GetXaxis()->GetXmax());
        FullFit.Sumw2();

        std::cout << "HowMany bins = "<< BiFit.GetNbinsX()<< ", "<< PoFit.GetNbinsX() << std::endl;
        FullFit.Add(&BiFit,&PoFit);

        TLegend* leg =new TLegend(0.6,0.6,0.9,0.9); 
        leg->AddEntry(&fakeDataHist,"FakeData","lf"); 
        leg->AddEntry(&FullFit,"Summed fit","lf"); 

        TCanvas* diff = new TCanvas("diff","",800,800);
        diff->cd(); 
        // -------------- Top panel 
        gStyle->SetOptStat(kFALSE);  
        TPad *pad1 = new TPad("pad1","pad1",0,0.3,1,1); 
        pad1->Draw(); 
        pad1->SetLogy(1); 
        pad1->cd(); 
        pad1->SetGrid(kTRUE); 
        pad1->SetBottomMargin(0.00); 
        gPad->RedrawAxis();  

        fakeDataHist.SetTitle("Two background fit");
        fakeDataHist.GetYaxis()->SetTitle(Form("counts"));
        fakeDataHist.GetYaxis()->SetTitleOffset(1.5); 
        fakeDataHist.SetFillColorAlpha(kGreen,0.5); 


        fakeDataHist.Draw(); 
        FullFit.SetFillColorAlpha(kRed,0.5); 
        // FullFit.Draw("same e"); 
        BiFit.Draw("same"); 
        PoFit.Draw("same"); 

        // leg->Draw(); 

        // TPaveText pt(0.1,0.1,0.3,0.3,"NDC");
        // pt.AddText(Form("BiNorm = %.2f",bufScaledResult.at(0)));
        // pt.AddText(Form("PoNorm = %.2f",bufScaledResult.at(1)));
        // pt.AddText(Form("Bi conv mean = %.2f",bufScaledResult.at(2)));
        // pt.AddText(Form("Bi conv RMS= %.2f",bufScaledResult.at(3)));
        // pt.AddText(Form("Bi scale= %.2f",bufScaledResult.at(4)));
        // pt.AddText(Form("Po conv mean = %.2f",bufScaledResult.at(5)));
        // pt.AddText(Form("Po conv RMS= %.2f",bufScaledResult.at(6)));
        // pt.AddText(Form("Po scale= %.2f",bufScaledResult.at(7)));
        // pt.Draw();
        // pt.SetNDC(kTRUE);
        diff->cd(); 

        // -------------- Bottom panel 
        TPad *pad2 = new TPad("pad2","pad2",0,0.0,1,0.3); 
        pad2->SetTopMargin(0.00); 
        pad2->Draw(); 
        // pad2->SetLogy(); 
        pad2->cd(); 
        pad2->SetBottomMargin(0.3); 
        pad2->SetGrid(kTRUE); 
        gStyle->SetOptStat(kFALSE);  

        TH1D * fracDiff= diffHist(&fakeDataHist,&FullFit); 
        fracDiff->Sumw2(); 
        fracDiff->SetLineColor(kRed); 
        fracDiff->SetMarkerStyle(3); 
        fracDiff->SetLineWidth(2); 
        fracDiff->SetFillColorAlpha(kRed,0.1); 
        // diff_minuit->GetXaxis()->SetRangeUser(0.3, 0.7);



        fracDiff->GetXaxis()->SetTitle("Energy (MeV)"); 
        fracDiff->GetYaxis()->SetTitle("Fractional bin error"); 
        fracDiff->GetYaxis()->SetTitleOffset(0.5); 
        fracDiff->GetXaxis()->SetLabelSize(0.1); 
        fracDiff->GetXaxis()->SetTitleSize(0.1); 
        fracDiff->GetYaxis()->SetLabelSize(0.1); 
        fracDiff->GetYaxis()->SetTitleSize(0.1); 

        fracDiff->SetMaximum(2); 
        fracDiff->SetMinimum(-2); 
        // diff_minuit->SetMinimum(0); 
        fracDiff->Draw(); 

        diff->Print("systematicFitExample.png"); 
    }
        return 0;

    }

    int main(int argc, char *argv[]){

        function();
        return 0;

    }

