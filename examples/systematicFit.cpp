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
#include <FileManager.h>
// #include <CompositeBinnedNLLH.h>
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

int function(){

    Rand::SetSeed(0);

    Gaussian gaus1(2, 0.5);
    Gaussian gaus2(4, 0.25);
    Gaussian gaus3(6, 0.25);
    Gaussian gaus4(8, 0.5);

    AxisCollection axes;
    axes.AddAxis(BinAxis("axis1", 0, 10 , 200));

    BinnedED pdf3("a_data", DistTools::ToHist(gaus1, axes));
    BinnedED pdf4("b_data", DistTools::ToHist(gaus4, axes));
    BinnedED pdf1("a_mc", DistTools::ToHist(gaus2, axes));
    BinnedED pdf2("b_mc", DistTools::ToHist(gaus3, axes));

    pdf1.SetObservables(0);
    pdf2.SetObservables(0);    
    pdf3.SetObservables(0);
    pdf4.SetObservables(0);

    pdf1.Scale(400);
    pdf2.Scale(400);    
    // pdf1.Normalise();
    // pdf2.Normalise();    

    std::vector<BinnedED> mcPdfs;
    mcPdfs.push_back(pdf1);
    mcPdfs.push_back(pdf2);

    padPDFs(mcPdfs);

    std::vector<BinnedED> dataPdfs;
    dataPdfs.push_back(pdf3);
    dataPdfs.push_back(pdf4);
    padPDFs(dataPdfs);

    BinnedEDGenerator dataGen;
    dataGen.SetPdfs(dataPdfs);
    std::vector<double> rates(2,50000);
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
 
    // // Only interested in energy RECO bit of the ntuple (energy).
    // ObsSet obsSet_reco(4);
    // BinnedED first_RECO("Bi210_reco",axes); first_RECO.SetObservables(obsSet_reco);
    // BinnedED second_RECO("Po210_reco",axes); second_RECO.SetObservables(obsSet_reco);
    // std::vector<BinnedED> DataPdfs;
    // DataPdfs.push_back(first_RECO);
    // DataPdfs.push_back(second_RECO);
    //
    // fileMan_RECO.AddCuts(cutCol_reco);
    // fileMan_RECO.FillEDs(DataPdfs);
    // padPDFs(DataPdfs);
    //
    // std::vector<TH1D> hists;
    // for (int i = 0; i < pdfs.size(); ++i) {
    //     hists.push_back(DistTools::ToTH1D(pdfs.at(i)));
    // }
    // for (int i = 0; i < DataPdfs.size(); ++i) {
    //     hists.push_back(DistTools::ToTH1D(DataPdfs.at(i)));
    // }
    //
    // for(int i =0;i<pdfs.size();i++){
    //     pdfs.at(i).Normalise();
    // }
    // for(int i =0;i<DataPdfs.size();i++){
    //     DataPdfs.at(i).Normalise();
    // }
    //
    // //Now make systematics
    //
    // bool Qsys = 1;
    // bool Qsec = 1;
    // bool Qmc = 0;
    // Convolution* conv = new Convolution("Bi210_Smear");
    // Gaussian gaus(0,1,"Bi210_Gaus"); 
    // conv->SetFunction(&gaus);
    //
    // // RunEnergy func(0,1,0);
    // // conv->SetFunction(&func);
    //
    // //axes is the axisCollection of the full fit.
    // conv->SetAxes(axes);
    //
    // ObsSet  obsToSmear(2);
    //
    // conv->SetTransformationObs(obsToSmear);
    // //obsSet is the full data Rep. Which contains indices [0,2] which corrsponds to the bit 0 and 2 of the ntuples i'm using 
    // conv->SetDistributionObs(obsSet);
    //
    // conv->Construct();
    //
    // Scale* scale= new Scale("Bi210_Scale");
    // scale->SetAxes(axes);
    //
    // scale->SetTransformationObs(obsToSmear);
    // //obsSet is the full data Rep. Which contains indices [0,2] which corrsponds to the bit 0 and 2 of the ntuples i'm using 
    // scale->SetDistributionObs(obsSet);
    //
    // scale->Construct();
    //
    // Convolution* PoConv = new Convolution("Po210_Smear");
    // Gaussian PoGaus(0,1,"Po210_Gaus"); 
    // PoConv->SetFunction(&PoGaus);
    //
    // PoConv->SetAxes(axes);
    //
    //
    // PoConv->SetTransformationObs(obsToSmear);
    // //obsSet is the full data Rep. Which contains indices [0,2] which corrsponds to the bit 0 and 2 of the ntuples i'm using 
    // PoConv->SetDistributionObs(obsSet);
    //
    // // PoConv->MakeFittable();
    // PoConv->Construct();
    //
    // Scale* PoScale= new Scale("Po210_Scale");
    // PoScale->SetAxes(axes);
    //
    // PoScale->SetTransformationObs(obsToSmear);
    // //obsSet is the full data Rep. Which contains indices [0,2] which corrsponds to the bit 0 and 2 of the ntuples i'm using 
    // PoScale->SetDistributionObs(obsSet);
    //
    // PoScale->Construct();
    //
    // // Convolution * PoConv = conv;
    // // Scale * PoScale = scale;
    //
    // SystematicManager SysManager;
    // SysManager.AddSystematic(conv,"ele");
    // SysManager.AddSystematic(scale,"ele");
    // // BiSysManager.AddPdfToGroup(std::string("ele"),pdfs.at(0));
    // SysManager.AddPdfToGroup("ele",pdfs.at(0));
    //
    // SysManager.AddSystematic(PoConv,"alpha");
    // SysManager.AddSystematic(PoScale,"alpha");
    // SysManager.AddPdfToGroup(std::string("alpha"),pdfs.at(1) );
    //
    // // // Setting optimisation limits
    // // std::vector<double> minima;
    // // minima.push_back(10);               // normalisation of Bi210 in data set A
    // // minima.push_back(10);               // normalisation of Po210 in data set A
    // // if(Qsys) minima.push_back(0.0);             // Gaussian mean and sigma
    // // if(Qsys) minima.push_back(0);
    // // if(Qsys) minima.push_back(0.99999);             // Scale in data set A
    // // if(Qsys) if(Qsec)  minima.push_back(0.0);             // Gaussian mean and sigma
    // // if(Qsys) if(Qsec)  minima.push_back(0.);
    // // if(Qsys) if(Qsec)  minima.push_back(0.90);            // Scale in data set A
    // //
    // // std::vector<double> maxima;
    // // maxima.push_back(100000);               // normalisation of Bi210 in data set A
    // // maxima.push_back(100000);               // normalisation of Po210 in data set A
    // // if(Qsys) maxima.push_back(0.101001);             // Gaussian mean and sigma
    // // if(Qsys) maxima.push_back(1.42);
    // // if(Qsys) maxima.push_back(1.001);            // Scale in data set A
    // // if(Qsys) if(Qsec)  maxima.push_back(0.10000);             // Gaussian mean and sigma
    // // if(Qsys) if(Qsec)  maxima.push_back(0.40);
    // // if(Qsys) if(Qsec)  maxima.push_back(1.01);            // Scale in data set A
    // //
    // // Rand rand;
    // //
    // // std::vector<double> initialerr;
    // // initialerr.push_back(1000);               // normalisation of Bi210 in data set A
    // // initialerr.push_back(1000);               // normalisation of Po210 in data set A
    // // if(Qsys) initialerr.push_back(0.1);             // Gaussian mean and sigma
    // // if(Qsys) initialerr.push_back(0.1);
    // // if(Qsys) initialerr.push_back(0.00001);            // Scale in data set A
    // // if(Qsys) if(Qsec)  initialerr.push_back(0.1);             // Gaussian mean and sigma
    // // if(Qsys) if(Qsec)  initialerr.push_back(0.1);
    // // if(Qsys) if(Qsec)  initialerr.push_back(0.00001);            // Scale in data set A
    // //
    // // // std::vector<double> initialerr;
    // // // for (int i = 0; i < maxima.size(); ++i) {
    // // //     if(i<
    // // //     initialerr.push_back(0.1*maxima.at(i));
    // // // }
    // //
    // // std::vector<double> sigmas;
    // // sigmas.push_back(1);               // normalisation of Bi210 in data set A
    // // sigmas.push_back(1);               // normalisation of Po210 in data set A
    // // if(Qsys) sigmas.push_back(1);             // Gaussian mean and sigma
    // // if(Qsys) sigmas.push_back(1);
    // // if(Qsys) sigmas.push_back(0.0001);            // Scale in data set A
    // // // sigmas.push_back(1);               // normalisation of Bi210 in data set B
    // // // sigmas.push_back(1);               // normalisation of Po210 in data set B
    // // if(Qsys) if(Qsec)  sigmas.push_back(1);             // Gaussian mean and sigma
    // // if(Qsys) if(Qsec)  sigmas.push_back(1);
    // // if(Qsys) if(Qsec)  sigmas.push_back(0.0001);            // Scale in data set A
    //
    // std::map<std::string,double> minima;
    //                         minima["Bi210_norm"]=10;               // normalisation of Bi210 in data set A
    //                         minima["Po210_norm"]=10;               // normalisation of Po210 in data set A
    // if(Qsys)                minima["Bi210_Gaus_means"]= 0.0;             // Gaussian mean and sigma
    // if(Qsys)                minima["Bi210_Gaus_stddevs"]=0;
    // if(Qsys)                minima["Bi210_Scale"]=0.99999;             // Scale in data set A
    // if(Qsys) if(Qsec)       minima["Po210_Gaus_means"]= 0.0;            // Gaussian mean and sigma
    // if(Qsys) if(Qsec)       minima["Po210_Gaus_stddevs"]=0;
    // if(Qsys) if(Qsec)       minima["Po210_Scale"]=0.99999;            // Scale in data set A
    //
    // std::map<std::string,double> maxima;
    //                         maxima["Bi210_norm"]=100000;                  // normalisation of Bi210 in data set A
    //                         maxima["Po210_norm"]=100000;                  // normalisation of Po210 in data set A
    // if(Qsys)                maxima["Bi210_Gaus_means"]= 0.1;            // Gaussian mean and sigma
    // if(Qsys)                maxima["Bi210_Gaus_stddevs"]=1.42;
    // if(Qsys)                maxima["Bi210_Scale"]=1.001;        // Scale in data set A
    // if(Qsys) if(Qsec)       maxima["Po210_Gaus_means"]= 0.1;           // Gaussian mean and sigma
    // if(Qsys) if(Qsec)       maxima["Po210_Gaus_stddevs"]=0.4;
    // if(Qsys) if(Qsec)       maxima["Po210_Scale"]=1.1;       // Scale in data set A
    //
    // Rand rand;
    //
    // std::map<std::string,double> initialerr;
    //                         initialerr["Bi210_norm"]=100;             // normalisation of Bi210 in data set A
    //                         initialerr["Po210_norm"]=100;             // normalisation of Po210 in data set A
    // if(Qsys)                initialerr["Bi210_Gaus_means"]= 0.1;        // Gaussian mean and sigma
    // if(Qsys)                initialerr["Bi210_Gaus_stddevs"]=0.1;
    // if(Qsys)                initialerr["Bi210_Scale"]=0.001;             // Scale in data set A
    // if(Qsys) if(Qsec)       initialerr["Po210_Gaus_means"]= 0.1;        // Gaussian mean and sigma
    // if(Qsys) if(Qsec)       initialerr["Po210_Gaus_stddevs"]=0.1;
    // if(Qsys) if(Qsec)       initialerr["Po210_Scale"]=0.0001;               // Scale in data set A
    //
    // // std::vector<double> initialerr;
    // // for (int i = 0; i < maxima.size(); ++i) {
    // //     if(i<
    // //     initialerr.push_back(0.1*maxima.at(i));
    // // }
    //
    // // std::vector<double> sigmas;
    // // sigmas.push_back(1);               // normalisation of Bi210 in data set A
    // // sigmas.push_back(1);               // normalisation of Po210 in data set A
    // // if(Qsys) sigmas.push_back(1);             // Gaussian mean and sigma
    // // if(Qsys) sigmas.push_back(1);
    // // if(Qsys) sigmas.push_back(0.0001);            // Scale in data set A
    // // // sigmas.push_back(1);               // normalisation of Bi210 in data set B
    // // // sigmas.push_back(1);               // normalisation of Po210 in data set B
    // // if(Qsys) if(Qsec)  sigmas.push_back(1);             // Gaussian mean and sigma
    // // if(Qsys) if(Qsec)  sigmas.push_back(1);
    // // if(Qsys) if(Qsec)  sigmas.push_back(0.0001);            // Scale in data set A
    //
    // // int nTest=100;
    // int nTest=1;
    //
    // std::vector<std::map<std::string,double> > fits;
    //
    // for (int iTest = 0; iTest < nTest; ++iTest) {
    //
    //     BinnedEDGenerator dataGen;
    //
    //     if (Qmc){
    //         std::cout << "Filling with MC pdfs" << std::endl;
    //         dataGen.SetPdfs(pdfs);
    //     }else{
    //         dataGen.SetPdfs(DataPdfs);
    //     }
    //
    //     std::vector<double> rates(DataPdfs.size(),50000);
    //     dataGen.SetRates(rates);
    //
    //     // BinnedED fakeData= dataGen.ExpectedRatesED();
    //     BinnedED fakeData= dataGen.PoissonFluctuatedED();
    //
    //
    //     {
    //         TH1D hist = DistTools::ToTH1D(fakeData);
    //         TCanvas c1;
    //         c1.cd();
    //         hist.Draw();
    //         if (Qmc){
    //             c1.Print("fakeDataTest.png");
    //         }else{
    //             c1.Print("fakeDataTest.png");
    //         }
    //     }
    //
    //
    //
    //     //Setting  up the combined likelihood.
    //     int BiBuffLow  =10;
    //     int BiBuffHigh =10;
    //     int PoBuffLow  =10;
    //     int PoBuffHigh =10;
    //     int totalNbins= axes.GetAxis(0).GetNBins();
    //     double BiBuffScale = (double)1/(1-(BiBuffLow+BiBuffHigh)/(double)totalNbins);
    //
    //     BinnedNLLH onlyLh; 
    //     onlyLh.SetBufferAsOverflow(false);
    //     onlyLh.SetBuffer(2,BiBuffLow,BiBuffHigh);
    //     onlyLh.SetBuffer(4,BiBuffLow,BiBuffHigh);
    //     onlyLh.SetDataDist(fakeData); // initialise with the data set
    //     onlyLh.AddPdfs(pdfs);
    //     std::cout << "here" << std::endl;
    //     if(Qsys) onlyLh.SetSystematicManager(SysManager);
    //     onlyLh.RegisterFitComponents();
    //     std::cout << "here" << std::endl;
    //
    //     // BinnedNLLH BiLh; 
    //     // BiLh.SetBufferAsOverflow(false);
    //     // BiLh.SetBuffer(2,BiBuffLow,BiBuffHigh);
    //     // BiLh.SetBuffer(4,BiBuffLow,BiBuffHigh);
    //     // BiLh.SetDataDist(fakeData); // initialise with the data set
    //
    //     //constrain mean to be about zero.
    //     // BiLh.SetConstraint("Convolution : Gaussian Means 0", 0., 0.01);
    //     //constrain RMS to be small.
    //     // BiLh.SetConstraint("Convolution : Gaussian st.devs 0", 0., 0.0000001);
    //     //constrain scale to be small.
    //     // BiLh.SetConstraint("Scale Factor", 0., 0.05);
    //     // BiLh.AddPdf(pdfs.at(0));
    //     // BiLh.AddPdfs(pdfs);
    //     // if(Qsys) BiLh.SetSystematicManager(BiSysManager);
    //     // BiLh.RegisterFitComponents();
    //
    //     // BinnedNLLH PoLh; 
    //     // PoLh.SetBufferAsOverflow(false);
    //     // PoLh.SetBuffer(2,PoBuffLow,PoBuffHigh);
    //     // PoLh.SetBuffer(4,PoBuffLow,PoBuffHigh);
    //     // PoLh.SetDataDist(fakeData); // initialise with the data set
    //     // //constrain mean to be about zero.
    //     // // PoLh.SetConstraint("Convolution : Gaussian Means 0", 0., 0.01);
    //     // //constrain RMS to be small.
    //     // // PoLh.SetConstraint("Convolution : Gaussian st.devs 0", 0., 0.0000001);
    //     // // //constrain scale to be small.
    //     // // PoLh.SetConstraint("Scale Factor", 0., 0.05);
    //     // // PoLh.AddPdf(pdfs.at(1));
    //     // PoLh.AddPdfs(pdfs);
    //     // if(Qsys) if(Qsec) PoLh.SetSystematicManager(PoSysManager);
    //     // PoLh.RegisterFitComponents();
    //
    //     // CompositeBinnedNLLH lh;
    //     // lh.AddBinnedNLLH(BiLh);
    //     // lh.AddBinnedNLLH(PoLh);
    //     //
    //     // //Need to share norms.
    //     // lh.SetSharedParameter("Bi210_norm");
    //     // lh.SetSharedParameter("Po210_norm");
    //     //
    //     //
    //     // lh.Initialise();
    //     // onlyLh.Initialise();
    //
    //     std::map<std::string,double> initialval;
    //                             initialval["Bi210_norm"]=(rand.UniformRange(minima["Bi210_norm"],maxima["Bi210_norm"]));               // normalisation of Bi210 in data set A
    //                             initialval["Po210_norm"]=(rand.UniformRange(minima["Po210_norm"],maxima["Po210_norm"]));               // normalisation of Po210 in data set A
    //     if(Qsys)                initialval["Bi210_Gaus_means"] =rand.UniformRange(minima["Bi210_Gaus_means"],maxima["Bi210_Gaus_means"]);             // Gaussian mean and sigma
    //     if(Qsys)                initialval["Bi210_Gaus_stddevs"]=(rand.UniformRange(minima["Bi210_Gaus_stddevs"],maxima["Bi210_Gaus_stddevs"]));
    //     if(Qsys)                initialval["Bi210_Scale"]=(rand.UniformRange(minima["Bi210_Scale"],maxima["Bi210_Scale"]));             // Scale in data set A
    //     if(Qsys) if(Qsec)       initialval["Po210_Gaus_means"]=(rand.UniformRange(minima["Po210_Gaus_means"],maxima["Po210_Gaus_means"]));             // Gaussian mean and sigma
    //     if(Qsys) if(Qsec)       initialval["Po210_Gaus_stddevs"] = (rand.UniformRange(minima["Po210_Gaus_stddevs"],maxima["Po210_Gaus_stddevs"]));
    //     if(Qsys) if(Qsec)       initialval["Po210_Scale"]=(rand.UniformRange(minima["Po210_Scale"],maxima["Po210_Scale"]));            // Scale in data set A
    //
    //     // Set up the optimisation
    //
    //     Minuit min;
    //     // min.SetMethod("Simplex");
    //     // min.SetMethod("Minimize");
    //     min.SetMaxCalls(1000000000);
    //     min.SetMinima(minima);
    //     min.SetMaxima(maxima);
    //     min.SetInitialValues(initialval);
    //     min.SetInitialErrors(initialerr);
    //
    //     // metHast.SetMaxIter(100000); 
    //     // metHast.SetMaxima(maxima); 
    //     // metHast.SetMinima(minima);         
    //     // metHast.SetFlipSign(true); 
    //     // metHast.SetTestStatLogged(true); 
    //     // metHast.SetSigmas(sigmas); 
    //
    //     MetropolisHastings metHast;
    //     // min.SetMethod("Simplex");
    //     // metHast.SetMethod("Minimize");
    //     // metHast.SetHistogramAxes(axes); 
    //     metHast.SetFlipSign(true); 
    //     metHast.SetTestStatLogged(true); 
    //     metHast.SetMaxIter(1000000);
    //     metHast.SetMinima(minima);
    //     metHast.SetMaxima(maxima);
    //     // metHast.SetSigmas(sigmas);
    //     metHast.SetInitialTrial(initialval);
    //
    //     std::cout << "__________printing Components___________" << std::endl;
    //     // lh.PrintComponents();
    //
    //     ////////////
    //     // 4. Fit //
    //     ////////////
    //     // std::cout << "Starting Fit for test " << itest << std::endl;
    //
    //     std::cout << "About to Fit" << std::endl;
    //     // FitResult result = min.Optimise(&lh);
    //     FitResult result = min.Optimise(&onlyLh);
    //     // FitResult result = metHast.Optimise(&lh);
    //
    //     std::map<std::string,double> tmpVec = result.GetBestFit(); 
    //     //This BiBuffScale comes from the buffer ajustment.
    //     tmpVec["BiNorm"] =  BiBuffScale*tmpVec["BiNorm"];
    //     std::map<std::string,double> bufScaledResult = tmpVec;
    //
    //     fits.push_back(bufScaledResult);
    //
    //     // fits[itest] = result.GetBestFit();
    //     result.SetPrintPrecision(3);
    //     result.Print();
    //     
    //     if (Qmc){
    //         result.SaveAs(Form("fitResultsMC_2_MC/result_%i.txt",iTest));
    //     }else{
    //         result.SaveAs(Form("fitResultsMC_2_RECO/result_%i.txt",iTest));
    //     }
    //
    //     // std::cout << "BiBuffScale = "<< BiBuffScale << std::endl;
    //     // for (int i = 0; i < bufScaledResult.size(); ++i) {
    //     //     std::cout <<bufScaledResult.at(i) << " " ;
    //     // }
    //     std::cout << std::endl;
    //     
        return 0;
    //     //Make double plot
    //     {
    //         BinnedED BiHolder = pdfs.at(0);
    //         BinnedED PoHolder = pdfs.at(1);
    //         BinnedED BiResult;
    //         BinnedED PoResult;
    //
    //         if(Qsys){
    //             Scale BiScale;
    //             BiScale.SetScaleFactor(fits.back().at(4));
    //             BiScale.SetAxes(axes);
    //             BiScale.SetTransformationObs(obsToSmear);
    //             BiScale.SetDistributionObs(obsSet);
    //             // BiScale.MakeFittable();
    //             BiScale.Construct();
    //             Convolution BiSmearer;
    //             BiSmearer.SetFunction(new Gaussian(fits.back().at(2),fits.back().at(3)));
    //             BiSmearer.SetAxes(axes);
    //             BiSmearer.SetTransformationObs(obsToSmear);
    //             BiSmearer.SetDistributionObs(obsSet);
    //             // BiSmearer.MakeFittable();
    //             BiSmearer.Construct();
    //
    //         if(Qsec){ 
    //             Scale PoScale;
    //             PoScale.SetScaleFactor(fits.back().at(7));
    //             PoScale.SetAxes(axes);
    //             PoScale.SetTransformationObs(obsToSmear);
    //             PoScale.SetDistributionObs(obsSet);
    //             // PoScale.MakeFittable();
    //             PoScale.Construct();
    //
    //
    //             Convolution PoSmearer;
    //             PoSmearer.SetFunction(new Gaussian(fits.back().at(5),fits.back().at(6)));
    //             PoSmearer.SetAxes(axes);
    //             PoSmearer.SetTransformationObs(obsToSmear);
    //             PoSmearer.SetDistributionObs(obsSet);
    //             // PoSmearer.MakeFittable();
    //             PoSmearer.Construct();
    //
    //             PoResult = PoScale( PoSmearer( PoHolder ) );
    //         }else{
    //
    //                 PoResult = PoHolder ;
    //         }
    //
    //             BiResult = BiScale( BiSmearer( BiHolder ) );
    //
    //             BiResult.Scale(fits.back().at(0) );
    //             //because the parameters are shared only appear in first set.
    //             PoResult.Scale(fits.back().at(1));
    //
    //         }else{
    //             BiResult = BiHolder ;
    //             PoResult = PoHolder ;
    //
    //             BiResult.Scale(fits.back().at(0));
    //             PoResult.Scale(fits.back().at(1));
    //         }
    //
    //         TH1D fakeDataHist;
    //         TH1D BiFit;
    //         TH1D PoFit;
    //         BiFit.Sumw2();
    //         PoFit.Sumw2();
    //         // fakeDataHist.Sumw2();
    //         fakeDataHist = DistTools::ToTH1D(fakeData);
    //         BiFit= DistTools::ToTH1D(BiResult);
    //         PoFit= DistTools::ToTH1D(PoResult);
    //         TH1D FullFit("FullFit","",BiFit.GetNbinsX(),BiFit.GetXaxis()->GetXmin(),BiFit.GetXaxis()->GetXmax());
    //         FullFit.Sumw2();
    //
    //         std::cout << "HowMany bins = "<< BiFit.GetNbinsX()<< ", "<< PoFit.GetNbinsX() << std::endl;
    //         FullFit.Add(&BiFit,&PoFit);
    //
    //         TLegend* leg =new TLegend(0.6,0.6,0.9,0.9); 
    //         leg->AddEntry(&fakeDataHist,"FakeData","lf"); 
    //         leg->AddEntry(&FullFit,"Summed fit","lf"); 
    //
    //         TCanvas* diff = new TCanvas("diff","",800,800);
    //         diff->cd(); 
    //         // -------------- Top panel 
    //         gStyle->SetOptStat(kFALSE);  
    //         TPad *pad1 = new TPad("pad1","pad1",0,0.3,1,1); 
    //         pad1->Draw(); 
    //         pad1->SetLogy(1); 
    //         pad1->cd(); 
    //         pad1->SetGrid(kTRUE); 
    //         pad1->SetBottomMargin(0.00); 
    //         gPad->RedrawAxis();  
    //
    //         fakeDataHist.SetTitle("Two background fit");
    //         fakeDataHist.GetYaxis()->SetTitle(Form("counts"));
    //         fakeDataHist.GetYaxis()->SetTitleOffset(1.5); 
    //         fakeDataHist.SetFillColorAlpha(kGreen,0.5); 
    //
    //
    //         fakeDataHist.Draw(); 
    //         FullFit.SetFillColorAlpha(kRed,0.5); 
    //         FullFit.Draw("same e"); 
    //         BiFit.Draw("same"); 
    //         PoFit.Draw("same"); 
    //         
    //         leg->Draw(); 
    //
    //         TPaveText pt(0.1,0.1,0.3,0.3,"NDC");
    //         pt.AddText(Form("BiNorm = %.2f",bufScaledResult.at(0)));
    //         pt.AddText(Form("PoNorm = %.2f",bufScaledResult.at(1)));
    //         pt.AddText(Form("Bi conv mean = %.2f",bufScaledResult.at(2)));
    //         pt.AddText(Form("Bi conv RMS= %.2f",bufScaledResult.at(3)));
    //         pt.AddText(Form("Bi scale= %.2f",bufScaledResult.at(4)));
    //         pt.AddText(Form("Po conv mean = %.2f",bufScaledResult.at(5)));
    //         pt.AddText(Form("Po conv RMS= %.2f",bufScaledResult.at(6)));
    //         pt.AddText(Form("Po scale= %.2f",bufScaledResult.at(7)));
    //         pt.Draw();
    //         // pt.SetNDC(kTRUE);
    //         diff->cd(); 
    //
    //         // -------------- Bottom panel 
    //         TPad *pad2 = new TPad("pad2","pad2",0,0.0,1,0.3); 
    //         pad2->SetTopMargin(0.00); 
    //         pad2->Draw(); 
    //         // pad2->SetLogy(); 
    //         pad2->cd(); 
    //         pad2->SetBottomMargin(0.3); 
    //         pad2->SetGrid(kTRUE); 
    //         gStyle->SetOptStat(kFALSE);  
    //
    //
    //         // TH1D * fracDiff= util->diffHist(&fakeDataHist,&FullFit); 
    //         // fracDiff->Sumw2(); 
    //         // fracDiff->SetLineColor(kRed); 
    //         // fracDiff->SetMarkerStyle(3); 
    //         // fracDiff->SetLineWidth(2); 
    //         // fracDiff->SetFillColorAlpha(kRed,0.1); 
    //         // // diff_minuit->GetXaxis()->SetRangeUser(0.3, 0.7);
    //         //
    //         //
    //         //
    //         // fracDiff->GetXaxis()->SetTitle("Energy (MeV)"); 
    //         // fracDiff->GetYaxis()->SetTitle("Fractional bin error"); 
    //         // fracDiff->GetYaxis()->SetTitleOffset(0.5); 
    //         // fracDiff->GetXaxis()->SetLabelSize(0.1); 
    //         // fracDiff->GetXaxis()->SetTitleSize(0.1); 
    //         // fracDiff->GetYaxis()->SetLabelSize(0.1); 
    //         // fracDiff->GetYaxis()->SetTitleSize(0.1); 
    //         //
    //         // fracDiff->SetMaximum(2); 
    //         // fracDiff->SetMinimum(-2); 
    //         // // diff_minuit->SetMinimum(0); 
    //         // fracDiff->Draw(); 
    //
    //         // TH1D * diff_minuit_smear= util->diffHist(&temp_data_energy,&complete_minuit_Energy_smeared); 
    //         // diff_minuit_smear->SetLineColor(kYellow); 
    //         // diff_minuit_smear->SetMarkerStyle(3); 
    //         // diff_minuit_smear->SetLineWidth(2); 
    //         // diff_minuit_smear->SetFillColorAlpha(kYellow,0.1); 
    //         //
    //         // diff_minuit_smear->Draw("same"); 
    //         //
    //         // TH1D * diff_minuit_smear_scale= util->diffHist(&temp_data_energy,&complete_minuit_Energy_smeared_scaled); 
    //         // diff_minuit_smear_scale->SetLineColor(kBlue); 
    //         // diff_minuit_smear_scale->SetMarkerStyle(3); 
    //         // diff_minuit_smear_scale->SetLineWidth(2); 
    //         // diff_minuit_smear_scale->SetFillColorAlpha(kBlue,0.1); 
    //         //
    //         // diff_minuit_smear->Draw("same"); 
    //         // diff_minuit_smear_scale->Draw("same"); 
    //         //
    //         //
    //         // if(QmetHast){
    //         //     TH1D * diff_metHast= util->diffHist(&temp_data_energy,complete_metHast_Energy); 
    //         //     diff_metHast->SetLineColor(kBlack); 
    //         //     diff_metHast->SetLineWidth(2); 
    //         //     diff_metHast->SetFillColorAlpha(kBlack,0.1); 
    //         //     diff_metHast->Draw("same");
    //         // }
    //         // // gStyle->SetOptStat(0); 
    //
    //         if(Qmc){
    //             diff->Print(Form("doublePlanel_Energy_%i.png",iTest)); 
    //             // diff->Print(Form("doublePlanel_Energy_%i.pdf",iTest)); 
    //             // diff->Print(Form("doublePlanel_Energy_%i.tex",iTest)); 
    //         }else{
    //             diff->Print(Form("doublePlanel_Energy_%i.png",iTest)); 
    //             // diff->Print(Form("doublePlanel_Energy_%i.pdf",iTest)); 
    //             // diff->Print(Form("doublePlanel_Energy_%i.tex",iTest)); 
    //         }
    //     }
    //
    // }//end of the itest loop.

    // //Now look at fits 
    // int nPlots = fits.at(0).size();
    // std::vector<TH1D> histlist;
    //     histlist.push_back(TH1D("Bi Norm","Result for component Norm Bi",          100,48000,52000)); //normBi
    //     histlist.push_back(TH1D("Po Norm","Result for component Norm Po",          100,48000,52000)); //normPo 
    //     histlist.push_back(TH1D("Bi conv mean","Result for component Bi conv mean",     100,-0.5,0.5));        //Bi mean
    //     histlist.push_back(TH1D("Bi conv sig","Result for component Bi conv RMS",      100,0,1));        //Bi sig
    //     histlist.push_back(TH1D("Bi scale","Result for component Bi scale",         100,0.85,1.15));        //Bi Scale
    //     histlist.push_back(TH1D("Po conv mean ","Result for component Po conv mean",     100,-0.5,0.5));        //Po mean
    //     histlist.push_back(TH1D("Po conv sig  ","Result for component Po conv RMS",      100,0,1));        //Po sig  
    //     histlist.push_back(TH1D("Po scale","Result for component Po scale",         100,0.85,1.15));        //Po Scale  
    //     // histlist.push_back(TH1D("Bi conv mean","Result for component Bi conv mean",     100,-maxima.at(2),maxima.at(2)));        //Bi mean
    //     // histlist.push_back(TH1D("Bi conv sig","Result for component Bi conv RMS",      100,-maxima.at(3),maxima.at(3)));        //Bi sig
    //     // histlist.push_back(TH1D("Bi scale","Result for component Bi scale",         100,-maxima.at(4),maxima.at(4)));        //Bi Scale
    //     // histlist.push_back(TH1D("Po conv mean ","Result for component Po conv mean",     100,-maxima.at(5),maxima.at(5)));        //Po mean
    //     // histlist.push_back(TH1D("Po conv sig  ","Result for component Po conv RMS",      100,-maxima.at(6),maxima.at(6)));        //Po sig  
    //     // histlist.push_back(TH1D("Po scale","Result for component Po scale",         100,-maxima.at(7),maxima.at(7)));        //Po Scale  
    // // for (int i = 0; i < nPlots; ++i) {                                                                           
    // //     histlist.push_back(TH1D(Form("%i",i),Form("Result for component %i",i),100,minima.at(i),maxima.at(i)));
    // // }
    // for (int i = 0; i < fits.size(); ++i) {
    //     for (int j = 0; j < nPlots; ++j) {
    //         histlist.at(j).Fill(fits.at(i).at(j));
    //     }
    // }
    //
    // {
    //     TFile output("fitResultsMC_2_RECO/indPlots.root","recreate");
    //     // if(Qmc){
    //     //     output("fitResultsMC_2_MC/indPlots.root","recreate");
    //     // }else{
    //     //     output("fitResultsMC_2_RECO/indPlots.root","recreate");
    //     // }
    //     TCanvas c1("c1","c1",1000,1000);
    //     int nSec= ceil(sqrt(nPlots));
    //     c1.Divide(nSec,nSec);
    //     for (int i = 0; i < nPlots; ++i) {
    //         c1.cd(i+1);
    //         histlist.at(i).Draw();
    //         output.cd();
    //         histlist.at(i).Write();
    //     }
    //     if(Qmc){
    //         c1.Print("plot.png");
    //         c1.Print("plot.pdf");
    //         c1.Print("plot.tex");
    //     }else{
    //         c1.Print("plot.png");
    //         c1.Print("plot.pdf");
    //         c1.Print("plot.tex");
    //     }
    //
    //     output.Close();
    // }

    //Print results

    // if(nTest>=100){
    //     ofstream outputText;
    //     if(Qmc){
    //         outputText.open ("fitResultsMC_2_MC/biasOutput.txt");
    //     }else{
    //         outputText.open ("fitResultsMC_2_RECO/biasOutput.txt");
    //     }
    //     outputText << "Summary of results from bias test.\n";
    //     outputText << "____________________________________________\n";
    //     outputText << Form("n tests = %i\n",nTest);
    //     for (int i = 0; i < histlist.size(); ++i) {
    //         outputText << Form("%s = %f +/- %f\n",histlist.at(i).GetName(),histlist.at(i).GetMean(),histlist.at(i).GetRMS());
    //     }
    //     outputText << "\n";
    //     for (int i = 0; i < histlist.size(); ++i) {
    //         outputText << Form("%s & %.2f $\\pm$ %.2f \\\\\n",histlist.at(i).GetName(),histlist.at(i).GetMean(),histlist.at(i).GetRMS());
    //     }
    //     outputText.close();
    // }

    


    }

    int main(int argc, char *argv[]){

        function();
        return 0;

    }

