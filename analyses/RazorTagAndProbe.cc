
#define RazorAnalyzer_cxx
#include "RazorAnalyzer.h"
#include "JetCorrectorParameters.h"
#include "TagAndProbePair.h"

//C++ includes

//ROOT includes
#include "TH1F.h"

using namespace std;

struct greater_than_pt{
    inline bool operator() (const TLorentzVector& p1, const TLorentzVector& p2){
        return p1.Pt() > p2.Pt();
    }
};
 
void RazorAnalyzer::RazorTagAndProbe( string outputfilename, int option, bool isData)
{
    //initialization: create one TTree for each analysis box 
    cout << "Initializing..." << endl;    
    cout << "IsData = " << isData << "\n";
    cout << "Option = " << option << "\n";

    //***************************************************
    //ten thousands digit refers to object type. 
    //***************************************************
    //1: electrons
    //2: muons
    //3: photons
    //4: taus
    //***************************************************
    //thousands and hundreds digit refers to denominator
    //***************************************************
    //0: Basic Object (Track or Supercluster)
    //1: reco object
    //2: pass ID+Iso veto
    //3: pass ID+Iso loose
    //4: pass ID+Iso medium
    //5: pass ID+Iso tight
    //6: pass ID veto
    //7: pass ID loose
    //8: pass ID medium
    //9: pass ID tight
    //10: pass Iso veto
    //11: pass Iso loose
    //12: pass Iso medium
    //13: pass Iso tight

    //***************************************************
    //ones and tens digit refers to numerator cuts
    //***************************************************
    //1: reco object
    //2: pass ID+Iso veto
    //3: pass ID+Iso loose
    //4: pass ID+Iso medium
    //5: pass ID+Iso tight
    //6: pass ID veto
    //7: pass ID loose
    //8: pass ID medium
    //9: pass ID tight
    //10: pass Iso veto
    //11: pass Iso loose
    //12: pass Iso medium
    //13: pass Iso tight
    //50 - 99: pass HLT Filter ( see specific mapping in the code below )

    int objectTypeOption = floor(float(option) / 10000);
    int denominatorType = floor( float(option - objectTypeOption*10000) / 100);
    int numeratorType = option - objectTypeOption*10000 - denominatorType*100;
    if (objectTypeOption == 1) cout << "Object Type : Electrons\n";
    else if (objectTypeOption == 2) cout << "Object Type : Muons\n";
    else if (objectTypeOption == 3) cout << "Object Type : Photons\n";
    else if (objectTypeOption == 4) cout << "Object Type : Taus\n";
    
    if (denominatorType == 0) cout << "Denominator Type : Basic Object (Track or Supercluster)\n";
    else if (denominatorType == 1) cout << "Denominator Type : Reco Object\n";
    else if (denominatorType == 2) cout << "Denominator Type : ID+Iso Veto\n";
    else if (denominatorType == 3) cout << "Denominator Type : ID+Iso Loose\n";
    else if (denominatorType == 4) cout << "Denominator Type : ID+Iso Medium\n";
    else if (denominatorType == 5) cout << "Denominator Type : ID+Iso Tight\n";
    else if (denominatorType == 6) cout << "Denominator Type : ID Veto\n";
    else if (denominatorType == 7) cout << "Denominator Type : ID Loose\n";
    else if (denominatorType == 8) cout << "Denominator Type : ID Medium\n";
    else if (denominatorType == 9) cout << "Denominator Type : ID Tight\n";
    else if (denominatorType == 10) cout << "Denominator Type : Iso Veto\n";
    else if (denominatorType == 11) cout << "Denominator Type : Iso Loose\n";
    else if (denominatorType == 12) cout << "Denominator Type : Iso Medium\n";
    else if (denominatorType == 13) cout << "Denominator Type : Iso Tight\n";
    else if (denominatorType == 23) cout << "Denominator Type : EGamma ID+Iso Loose\n";
    else if (denominatorType == 24) cout << "Denominator Type : EGamma ID+Iso Medium\n";
    else if (denominatorType == 25) cout << "Denominator Type : EGamma ID+Iso Tight\n";
    else if (denominatorType >= 50) cout << "Denominator Type : pass HLT Filters\n";

    if (numeratorType == 1) cout << "Numerator Type : Reco Object\n";
    else if (numeratorType == 2) cout << "Numerator Type : ID+Iso Veto\n";
    else if (numeratorType == 3) cout << "Numerator Type : ID+Iso Loose\n";
    else if (numeratorType == 4) cout << "Numerator Type : ID+Iso Medium\n";
    else if (numeratorType == 5) cout << "Numerator Type : ID+Iso Tight\n";
    else if (numeratorType == 6) cout << "Numerator Type : ID Veto\n";
    else if (numeratorType == 7) cout << "Numerator Type : ID Loose\n";
    else if (numeratorType == 8) cout << "Numerator Type : ID Medium\n";
    else if (numeratorType == 9) cout << "Numerator Type : ID Tight\n";
    else if (numeratorType == 10) cout << "Numerator Type : Iso Veto\n";
    else if (numeratorType == 11) cout << "Numerator Type : Iso Loose\n";
    else if (numeratorType == 12) cout << "Numerator Type : Iso Medium\n";
    else if (numeratorType == 13) cout << "Numerator Type : Iso Tight\n";
    else if (numeratorType >= 50) cout << "Numerator Type : pass HLT Filters\n";


    TRandom3 *random = new TRandom3(33333); //Artur wants this number 33333

    bool printSyncDebug = false;
    std::vector<JetCorrectorParameters> correctionParameters;

    if (isData) {
      correctionParameters.push_back(JetCorrectorParameters("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_2_0/src/RazorAnalyzer/data/PHYS14_V2_MC_L1FastJet_AK4PFchs.txt"));
      correctionParameters.push_back(JetCorrectorParameters("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_2_0/src/RazorAnalyzer/data/PHYS14_V2_MC_L2Relative_AK4PFchs.txt"));
      correctionParameters.push_back(JetCorrectorParameters("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_2_0/src/RazorAnalyzer/data/PHYS14_V2_MC_L3Absolute_AK4PFchs.txt"));    
    } else {
      correctionParameters.push_back(JetCorrectorParameters("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_2_0/src/RazorAnalyzer/data/PHYS14_V2_MC_L1FastJet_AK4PFchs.txt"));
      correctionParameters.push_back(JetCorrectorParameters("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_2_0/src/RazorAnalyzer/data/PHYS14_V2_MC_L2Relative_AK4PFchs.txt"));
      correctionParameters.push_back(JetCorrectorParameters("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_7_2_0/src/RazorAnalyzer/data/PHYS14_V2_MC_L3Absolute_AK4PFchs.txt"));  
    }
    
    FactorizedJetCorrector *JetCorrector = new FactorizedJetCorrector(correctionParameters);
    JetCorrectorParameters *JetResolutionParameters = new JetCorrectorParameters("/afs/cern.ch/work/s/sixie/public/releases/run2/CMSSW_5_3_26/src/RazorAnalyzer/data/JetResolutionInputAK5PF.txt");
    SimpleJetResolution *JetResolutionCalculator = new SimpleJetResolution(*JetResolutionParameters);


    //*************************************************************************
    //Set up Output File
    //*************************************************************************
    string outfilename = outputfilename;
    if (outfilename == "") outfilename = "RazorControlRegions.root";
    TFile *outFile = new TFile(outfilename.c_str(), "RECREATE");
    TagAndProbePair *TPPair = new TagAndProbePair;
    TPPair->CreateTree();  
    TPPair->tree_->SetAutoFlush(0);

    //histogram containing total number of processed events (for normalization)
    TH1F *NEvents = new TH1F("NEvents", "NEvents", 1, 1, 2);
  
    //*************************************************************************
    //Look over Input File Events
    //*************************************************************************
    if (fChain == 0) return;
    cout << "Total Events: " << fChain->GetEntries() << "\n";
    Long64_t nbytes = 0, nb = 0;

    for (Long64_t jentry=0; jentry<fChain->GetEntries();jentry++) {

      //begin event
      if(jentry % 1000 == 0) cout << "Processing entry " << jentry << endl;

      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;


      printSyncDebug = false;
      if (printSyncDebug) {
	cout << "\n****************************************************************\n";
	cout << "Debug Event : " << runNum << " " << lumiNum << " " << eventNum << "\n";
      }

      //fill normalization histogram
      NEvents->Fill(1.0);
     
      //event info
      TPPair->weight = 1.0;
      TPPair->run = runNum;
      TPPair->lumi = lumiNum;
      TPPair->event = eventNum;
	
      //get NPU
      for (int i=0; i < nBunchXing; ++i) {
	if (BunchXing[i] == 0) {
	  TPPair->NPU_0 = nPUmean[i];
	}
	if (BunchXing[i] == -1) {
	  TPPair->NPU_Minus1 = nPUmean[i];
	}
	if (BunchXing[i] == 1) {
	  TPPair->NPU_Plus1 = nPUmean[i];
	}	  
      }
      TPPair->NPV = nPV;
      TPPair->Rho = fixedGridRhoFastjetAll;
      
 

      //******************************************
      //Find Generated leptons
      //******************************************
      vector<int> genLeptonIndex;      
      //find gen electrons
      if (objectTypeOption == 1) {
	for(int j = 0; j < nGenParticle; j++){

	  //look for electrons
	  if (abs(gParticleId[j]) == 11 && gParticleStatus[j] == 1 	      
	      && abs(gParticleEta[j]) < 3.0 && gParticlePt[j] > 3
	      ) {
	    if ( abs(gParticleMotherId[j]) == 23 )  {
	      genLeptonIndex.push_back(j);	      
	    }
	  }
	} //loop over gen particles
      }
      
      //look for muons
      if (objectTypeOption == 1) {
	for(int j = 0; j < nGenParticle; j++){
	  if (abs(gParticleId[j]) == 13 && gParticleStatus[j] == 1  
	      && abs(gParticleEta[j]) < 3.0 && gParticlePt[j] > 3
	      ) {
	    if ( abs(gParticleMotherId[j]) == 23 ) {	     
	      genLeptonIndex.push_back(j);
	    }	    
	  }
	} //loop over gen particles		
      }
	

      //*********************************************************
      //Electrons
      //*********************************************************
      if (objectTypeOption == 1) {

	//*******************************************************
	//Loop over Tag electrons
	//*******************************************************
	for(int indexTag = 0; indexTag < nElectrons; indexTag++){

	  if(elePt[indexTag] < 30) continue;
	  if(fabs(eleEta[indexTag]) > 2.5) continue;
	  

	  //For MC, Match to Gen level electron
	  if (!isData) {
	    bool genmatch = false;
	    for (int q=0;q<int(genLeptonIndex.size()); q++) {
	      if ( deltaR(eleEta[indexTag],elePhi[indexTag],
			  gParticleEta[genLeptonIndex[q]], 
			  gParticlePhi[genLeptonIndex[q]]) < 0.1) {
		genmatch = true;
	      }
	    }
	    if (!genmatch) continue;
	  }


	  //tag must pass tight cuts
	  if ( !isEGammaPOGTightElectron(indexTag) ) continue;
	  
	  //Tag must match single electron HLT Filters OR tag leg filter of the dedicated T&P trigger
	  if ( !matchTagElectronHLTFilters(indexTag)) continue;
	  	 
	  TLorentzVector vtag;
	  vtag.SetPtEtaPhiM(elePt[indexTag], eleEta[indexTag], elePhi[indexTag], 0.000511);

	  //*******************************************************
	  //Loop over Probe electrons
	  //*******************************************************
	  for(int indexProbe = 0; indexProbe < nElectrons; indexProbe++){
	    
	    if(elePt[indexProbe] < 5) continue;
	    if(fabs(eleEta[indexProbe]) > 2.5) continue;
	  
	    //skip the tag
	    if (indexTag == indexProbe) continue;
	  
	    //For MC, Match to Gen level electron
	    if (!isData) {
	      bool genmatch = false;
	      for (int q=0;q<int(genLeptonIndex.size()); q++) {
		if ( deltaR(eleEta[indexTag],elePhi[indexTag],
			    gParticleEta[genLeptonIndex[q]], 
			    gParticlePhi[genLeptonIndex[q]]) < 0.1) {
		  genmatch = true;
		}
	      }
	      if (!genmatch) continue;
	    }
	    

	    // //Probe must match probe leg filter of the dedicated T&P trigger
	    // if ( !matchProbeElectronHLTFilters(indexTag)) continue;
	    // if ( !matchProbeSCHLTFilters(indexTag)) continue;

	    //cout << "Probe: " << elePt[indexProbe] << " " << eleEta[indexProbe] << " " <<  elePhi[indexProbe] << " : " << isTightElectron(indexProbe) << "\n";

	    //*******************************************************
	    //denominator selection
	    //*******************************************************
	    if (denominatorType == 1) {
	      // reco object doesn't require any additional cuts
	    }
	    if (denominatorType == 5) {
	      if ( !isTightElectron(indexProbe) ) continue;
	    }

	    TLorentzVector vprobe;
	    vprobe.SetPtEtaPhiM(elePt[indexProbe], eleEta[indexProbe], elePhi[indexProbe], 0.000511);

	    TPPair->mass = (vtag+vprobe).M();
	    TPPair->pt = elePt[indexProbe];
	    TPPair->eta = eleEta[indexProbe];
	    TPPair->phi = elePhi[indexProbe];
	    TPPair->charge = eleCharge[indexProbe];


	    //****************************************
	    //PASS OR FAIL
	    //****************************************
	    bool pass = false;
	    if (numeratorType == 2) {
	      pass = isVetoElectron(indexProbe);
	    }
	    if (numeratorType == 3) {
	      pass = isLooseElectron(indexProbe);
	    }
	    if (numeratorType == 5) {
	      pass = isTightElectron(indexProbe);
	    }
	    if (numeratorType == 23) {
	      pass = isEGammaPOGLooseElectron(indexProbe);
	    }
	    if (numeratorType == 25) {
	      pass = isEGammaPOGTightElectron(indexProbe);
	    }
	    if (numeratorType == 50) {
	      // cout << "pass: " << matchElectronHLTFilters(indexProbe, "SingleElectron") << " : "
	      // 	   << ele_passHLTFilter[indexProbe][1] << " " 
	      // 	   << ele_passHLTFilter[indexProbe][5] << " " 
	      // 	   << ele_passHLTFilter[indexProbe][6] << " " 
	      // 	   << ele_passHLTFilter[indexProbe][12] << " " 
	      // 	   << ele_passHLTFilter[indexProbe][13] << " " 
	      // 	   << ele_passHLTFilter[indexProbe][3] << " " 
	      // 	   << ele_passHLTFilter[indexProbe][8] << " " 
	      // 	   << ele_passHLTFilter[indexProbe][10] << " " 
	      // 	   << ele_passHLTFilter[indexProbe][15] << " " 
	      // 	   << " \n";
	      pass = matchElectronHLTFilters(indexProbe, "SingleElectron");
	    }
	    TPPair->pass = pass;
	    // cout << " TP Pass: " << TPPair->pass << "\n";

	    //****************************************
	    //Fill Output Tree
	    //****************************************
	    TPPair->tree_->Fill();
	    

	  } //loop over probe electrons

	} // loop over tag electrons


      } //if objects are electrons

      //*********************************************************
      //Muons
      //*********************************************************
      if (objectTypeOption == 2) {

      }
    
      //*********************************************************
      //Photons
      //*********************************************************
      if (objectTypeOption == 3) {

      }

                                                                                                 
    }//end of event loop


    cout << "Filled Total of " << NEvents->GetBinContent(1) << " Events\n";
    cout << "Writing output trees..." << endl;
    outFile->Write();
    outFile->Close();

}

