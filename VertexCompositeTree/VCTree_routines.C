#include "./common/VertexCompositeTree.h"

void update_progress(int ientry, int total_entries, int percentage_increment)
{
	if (ientry % (total_entries / percentage_increment) == 0)
	{
		std::cout << "Processing " << ientry << "th entry... (" << (int)((double)ientry / total_entries * 100) << "%)" << std::endl;
	}
}

void open_VCTree(VertexCompositeTree &tree, std::string inFiles, std::string treeDir)
{
	// Extract the tree
	if (!tree.GetTree(inFiles, treeDir))
	{
		cout << "Invalid Correct-Sign tree!" << endl;
		return;
	}

	cout << "======> " << inFiles << " is opened. It has " << tree.GetEntries() << " entries." << endl;
}

void draw_latex(double latex_x, double latex_y, TString latex_text, double latex_size = 0.04)
{
	TLatex *latex = new TLatex();
	latex->SetNDC();
	latex->SetTextFont(42);
	latex->SetTextSize(latex_size);
	latex->SetTextAlign(11);
	latex->DrawLatex(latex_x, latex_y, Form("%s", latex_text.Data()));
}

void draw_header_Run3()
{
	TLatex *latex = new TLatex();
	latex->SetNDC();
	latex->SetTextFont(42);
	latex->SetTextSize(0.04);
	latex->DrawLatex(0.18, 0.93, "#bf{CMS} #it{Simulation}");
	latex->DrawLatex(0.68, 0.93, "PbPb Run3 (5.36 TeV)");
}