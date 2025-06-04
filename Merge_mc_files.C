TString Input_path_NPS = "generated_events/";
TString Input_path_HMS = "worksim/";
TString Output_path = "worksim/";

double Read_HMS_Angle(TString Simulation_Name){
    TString Input_File_Name = "./infiles/" + Simulation_Name + ".inp";
    std::ifstream file(Input_File_Name);
    if (!file.is_open()) {
        std::cerr << "Error opening the input file!" << std::endl;
        return -1;
    }

    std::string line;
    while (getline(file, line)) {

        if (line.find("Spectrometer angle") != std::string::npos) {

            std::istringstream ss(line);
            double angle; 
            ss >> angle;
            file.close();
            return angle;
        }
    }

    file.close();
    std::cerr << "Spectrometer angle not found!" << std::endl;
    return -1;
}

void Merge_mc_files(TString Input_name_NPS = "test", TString Input_name_HMS = "hms_nps_example", TString Output_name = "default") {
    TString Input_file_NPS = Input_path_NPS + Input_name_NPS + ".root";
    TString Input_file_HMS = Input_path_HMS + Input_name_HMS + ".root";

    Double_t HMS_Angle = Read_HMS_Angle(Input_name_HMS);
    Double_t HMS_Angle_cos = cos(HMS_Angle * 3.1415926535 / 180.0);
    Double_t HMS_Angle_sin = sin(HMS_Angle * 3.1415926535 / 180.0);

    if(Output_name == "default") Output_name = Input_name_HMS + "_merged.root";
    else Output_name = Output_name + "_merged.root";
    TString Output_file = Output_path + Output_name;

    TFile *file1 = TFile::Open(Input_file_NPS);
    TFile *file2 = TFile::Open(Input_file_HMS);

    if (!file1 || !file2) {
        std::cerr << "Error opening root files!" << std::endl;
        return;
    }

    TTree *tree1 = (TTree*)file1->Get("t_dvcs");
    TTree *tree2 = (TTree*)file2->Get("h10");

    if (!tree1 || !tree2) {
        std::cerr << "Error accessing trees!" << std::endl;
        return;
    }

    if (!(tree1->GetEntries()==tree2->GetEntries())) {
        std::cerr << "Two root files don't have the same entries!" << std::endl;
        // return;
    }

    Int_t evtNb, clust_size;
    Double_t edep[1080];
    Double_t clust_ene, clust_x, clust_y, clust_W2, psf, RIE_px, RIE_py, RIE_pz;
    Double_t GIE_px, GIE_py, GIE_pz, GSE_px, GSE_py, GSE_pz;
    Double_t RP_px, RP_py, RP_pz, GP_px, GP_py, GP_pz, RV_z, GV_x, GV_y, GV_z, Rt, Gt, RxB, GxB;
    Double_t RQ2, GQ2, Rphi, Gphi, X_sum, X_diff, X_BH, Rr_val, Gr_val;

    tree1->SetBranchAddress("evtNb", &evtNb);
    tree1->SetBranchAddress("edep", edep);
    // tree1->SetBranchAddress("clust_ene", &clust_ene);
    // tree1->SetBranchAddress("clust_x", &clust_x);
    // tree1->SetBranchAddress("clust_y", &clust_y);
    // tree1->SetBranchAddress("clust_W2", &clust_W2);
    // tree1->SetBranchAddress("clust_size", &clust_size);
    tree1->SetBranchAddress("psf", &psf);
    tree1->SetBranchAddress("RIE_px", &RIE_px);
    tree1->SetBranchAddress("RIE_py", &RIE_py);
    tree1->SetBranchAddress("RIE_pz", &RIE_pz);
    tree1->SetBranchAddress("GIE_px", &GIE_px);
    tree1->SetBranchAddress("GIE_py", &GIE_py);
    tree1->SetBranchAddress("GIE_pz", &GIE_pz);
    tree1->SetBranchAddress("GSE_px", &GSE_px);
    tree1->SetBranchAddress("GSE_py", &GSE_py);
    tree1->SetBranchAddress("GSE_pz", &GSE_pz);
    // tree1->SetBranchAddress("RP_px", &RP_px);
    // tree1->SetBranchAddress("RP_py", &RP_py);
    // tree1->SetBranchAddress("RP_pz", &RP_pz);
    tree1->SetBranchAddress("GP_px", &GP_px);
    tree1->SetBranchAddress("GP_py", &GP_py);
    tree1->SetBranchAddress("GP_pz", &GP_pz);
    // tree1->SetBranchAddress("RV_z", &RV_z);
    tree1->SetBranchAddress("GV_x", &GV_x);
    tree1->SetBranchAddress("GV_y", &GV_y);
    tree1->SetBranchAddress("GV_z", &GV_z);
    // tree1->SetBranchAddress("Rt", &Rt);
    tree1->SetBranchAddress("Gt", &Gt);
    // tree1->SetBranchAddress("RxB", &RxB);
    tree1->SetBranchAddress("GxB", &GxB);
    // tree1->SetBranchAddress("RQ2", &RQ2);
    tree1->SetBranchAddress("GQ2", &GQ2);
    // tree1->SetBranchAddress("Rphi", &Rphi);
    tree1->SetBranchAddress("Gphi", &Gphi);
    tree1->SetBranchAddress("X_sum", &X_sum);
    tree1->SetBranchAddress("X_diff", &X_diff);
    tree1->SetBranchAddress("X_BH", &X_BH);
    // tree1->SetBranchAddress("Rr_val", &Rr_val);
    // tree1->SetBranchAddress("Gr_val", &Gr_val);

    Double_t eventnumber, hsxfp, hsyfp, hsxpfp, hsypfp, hsytar, hsxptar, hsyptar, hsdelta;
    Double_t MC_RSE_px, MC_RSE_py, MC_RSE_pz, hms_stop_id;

    tree2->SetBranchAddress("eventnumber", &eventnumber);
    tree2->SetBranchAddress("hsxfp", &hsxfp);
    tree2->SetBranchAddress("hsyfp", &hsyfp);
    tree2->SetBranchAddress("hsxpfp", &hsxpfp);
    tree2->SetBranchAddress("hsypfp", &hsypfp);
    tree2->SetBranchAddress("hsytar", &hsytar);
    tree2->SetBranchAddress("hsxptar", &hsxptar);
    tree2->SetBranchAddress("hsyptar", &hsyptar);
    tree2->SetBranchAddress("hsdelta", &hsdelta);
    tree2->SetBranchAddress("hms_stop_id", &hms_stop_id);
    tree2->SetBranchAddress("MC_RSE_px", &MC_RSE_px);
    tree2->SetBranchAddress("MC_RSE_py", &MC_RSE_py);
    tree2->SetBranchAddress("MC_RSE_pz", &MC_RSE_pz);

    TFile *outputFile = new TFile(Output_file, "RECREATE");

    TTree *mergedTree = new TTree("MC", "Merged Tree with matching events");

    TObjArray *branches1 = tree1->GetListOfBranches();
    TObjArray *branches2 = tree2->GetListOfBranches();

    for (int i = 0; i < branches1->GetEntries(); ++i) {
        TBranch *branch1 = (TBranch*)branches1->At(i);
        mergedTree->Branch(branch1->GetName(), branch1->GetAddress(), branch1->GetTitle());
    }

    for (int i = 0; i < branches2->GetEntries(); ++i) {
        TBranch *branch2 = (TBranch*)branches2->At(i);
        mergedTree->Branch(branch2->GetName(), branch2->GetAddress(), branch2->GetTitle());
    }

    Double_t vertex_z;
    mergedTree->Branch("vertex_z", &vertex_z, "vertex_z/D");


    // cout<<tree1->GetEntries()<<"\t"<<tree2->GetEntries()<<"\n"<<endl;
    // cout<<"Event number:"<<endl;
    // cout<<"Input_tree \t Output_tree"<<endl;

    Int_t BadSync = 0;

    Long64_t nEntries = tree2->GetEntries();
    for (Long64_t entry = 0; entry < nEntries; ++entry) {
    // for (Long64_t entry = 0; entry < 10; ++entry) {
    // for (Long64_t entry = nEntries-10; entry < nEntries; ++entry) {
        tree1->GetEntry(entry);
        tree2->GetEntry(entry);
        // cout<<evtNb<<"\t\t"<<int(eventnumber)<<endl;
        // cout<<GSE_pz<<"\t"<<MC_RSE_px<<endl;

        if(hsytar<-1e5) vertex_z = -1e6;
        else{
            vertex_z = hsytar + 0.1*(0.52-0.012*HMS_Angle+0.002*HMS_Angle*HMS_Angle) - GV_x*( HMS_Angle_cos + hsyptar*HMS_Angle_sin );
            // vertex_z = hsytar - GV_x*( HMS_Angle_cos + hsyptar*HMS_Angle_sin );
            vertex_z = vertex_z / ( HMS_Angle_sin - hsyptar*HMS_Angle_cos );
        }

        if (evtNb == eventnumber) {
            mergedTree->Fill();
        }
        else BadSync += 1;
    }

    outputFile->Write();
    outputFile->Close();

    if (BadSync>=1) {
        std::cerr << "The synchronization is bad!" << std::endl;
        std::cerr << "BadSync = " << BadSync << std::endl;
        return;
    }

    std::cout << "Merging completed! Output saved to "<< Output_file << " ." << std::endl;
}