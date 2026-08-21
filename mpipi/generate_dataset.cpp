#include <iostream>
#include <vector>

#include "TFile.h"
#include "TTree.h"

#include "Pythia8/Pythia.h"

// Main pythia loop
int main()
{
    // Define tree with particles

    TFile *output = new TFile("output.root", "recreate");
    TTree *tree = new TTree("mytree", "mytree");
    std::vector<float> par_px, par_py, par_pz, par_e;
    std::vector<int> par_pid, par_status;
    tree->Branch("par_px", &par_px);
    tree->Branch("par_py", &par_py);
    tree->Branch("par_pz", &par_pz);
    tree->Branch("par_e", &par_e);
    tree->Branch("par_pid", &par_pid);
    tree->Branch("par_status", &par_status);

    // Configure pythia

    Pythia8::Pythia pythia;

    pythia.readString("Beams:frameType = 2");
    pythia.readString("Beams:idA = -211");
    pythia.readString("Beams:idB = 2212");
    pythia.readString("Beams:eA = 10"); // GeV
    pythia.readString("Beams:eB = 0"); // fixed target

    pythia.readString("SoftQCD:inelastic = on");

    pythia.readString("HadronLevel:Hadronize = on");
    pythia.readString("HadronLevel:Decay = on");

    // If Pythia fails to initialize, exit with error.
    if (!pythia.init()) return 1;

    int nAbort = 10;
    int iAbort = 0;

    int nevent = 100000;
    int ievent = 0;

    // Begin Event Loop; generate until none left in input file
    while (ievent<nevent && iAbort < nAbort) {

        // Generate events, and check whether generation failed.
        if (!pythia.next()) {
          ++iAbort;
          continue;
        }
	++ievent;

	par_px.clear();
	par_py.clear();
	par_pz.clear();
	par_e.clear();
	par_pid.clear();
	par_status.clear();

        int entries = pythia.event.size();

        // Add in hard scatter particles!
        for(int j=0;j<pythia.event.size();j++){
            auto &p = pythia.event[j];
            if (not p.isFinal()) continue;
            par_px.push_back(p.px());
            par_py.push_back(p.py());
            par_pz.push_back(p.pz());
            par_e.push_back(p.e());
            par_pid.push_back(p.id());
            par_status.push_back(p.status());
        }

        tree->Fill();
    }

    output->Write();
    output->Close();

    return 0;
}
