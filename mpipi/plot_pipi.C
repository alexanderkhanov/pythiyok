void plot_pipi()
{
  TFile* ff = new TFile("output.root");
  TTree* tree = (TTree*)ff->Get("mytree");

  vector<float>* par_px;
  vector<float>* par_py;
  vector<float>* par_pz;
  vector<float>* par_e;
  vector<int>* par_pid;
  vector<int>* par_status;

  tree->SetBranchAddress("par_px", &par_px);
  tree->SetBranchAddress("par_py", &par_py);
  tree->SetBranchAddress("par_pz", &par_pz);
  tree->SetBranchAddress("par_e", &par_e);
  tree->SetBranchAddress("par_pid", &par_pid);
  tree->SetBranchAddress("par_status", &par_status);

  // book histograms
  TH1* h_mpipi = new TH1D("mpipi","",100,0.,2.);

  int nev = tree->GetEntries();
  for (int iev = 0; iev<nev; ++iev) {
    if (iev&&iev%1000==0) { cout << iev << '\r'; cout.flush(); }

    par_px = 0;
    par_py = 0;
    par_pz = 0;
    par_e = 0;
    par_pid = 0;
    par_status = 0;

    tree->GetEntry(iev);
    if (par_px==0 || par_px->empty()) continue;

    vector<TLorentzVector> pip, pim;
    int np = par_px->size();
    for (int ip = 0; ip<np; ++ip) {
      if ((*par_status)[ip]!=91) continue; // keep stable final particles
      TLorentzVector v((*par_px)[ip], (*par_py)[ip], (*par_pz)[ip], (*par_e)[ip]);
      int pid = (*par_pid)[ip];
      if (pid==211) pip.push_back(v); // pi+
      else if (pid==-211) pim.push_back(v); // pi-
    }

    for (int i1 = 0; i1<pip.size(); ++i1) {
      for (int i2 = 0; i2<pim.size(); ++i2) {
	h_mpipi->Fill((pip[i1]+pim[i2]).M());
      }
    }
  }

  h_mpipi->GetXaxis()->SetTitle("M(#pi^{+}#pi^{#minus}) [GeV]");
  h_mpipi->GetYaxis()->SetTitle("Arbitrary units");
  h_mpipi->Draw();
}
