#include <wx/wx.h>

#include "libcplot.hpp"

class CPlotWindow : public wxFrame {
public:
    CPlotWindow(const wxString& title) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600), wxDEFAULT_FRAME_STYLE) {
        wxPanel* panel = new wxPanel(this, wxID_ANY);

		menuBar = new wxMenuBar;

		fileMenu = new wxMenu;
		fileMenu->Append(wxID_EXIT, "&Exit\tAlt-X", "Exit the application");
        fileMenu->Append(wxID_SAVE, "&Save\tAlt-S", "Save the current image");
        fileMenu->Append(wxID_SAVEAS, "&Save as...\tAlt-Shift-S", "Save and name the current image");
        menuBar->Append(fileMenu, "&File");

		helpMenu = new wxMenu;
		helpMenu->Append(wxID_ABOUT, "&About\tAlt-A", "Show about dialog");
        menuBar->Append(helpMenu, "&Help");

		SetMenuBar(menuBar);

        staticBitmap = new wxStaticBitmap(panel, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE | wxFULL_REPAINT_ON_RESIZE);
		staticBitmap->SetMinSize({400, 400});
		wxStaticText* text = new wxStaticText(panel, wxID_ANY, "f(z) = ");

        // Add a text box below the bitmap
        textBox = new wxTextCtrl(panel, wxID_ANY, "f(z) = ...",
                                 wxDefaultPosition, wxDefaultSize,
                                 wxTE_DONTWRAP);

        // Set up the main sizer to arrange the controls vertically
        mainSizer = new wxBoxSizer(wxVERTICAL);
        mainSizer->Add(staticBitmap, 1, wxEXPAND); // Add the bitmap to the sizer and let it expand to fill the frame

		wxBoxSizer* text_sizer = new wxBoxSizer(wxHORIZONTAL);
		text_sizer->Add(text, 0, wxALIGN_CENTER_VERTICAL | wxALL, 10);
        text_sizer->Add(textBox, 1, wxEXPAND | wxALL, 10);

		mainSizer->Add(text_sizer, 0, wxEXPAND | wxALL, 10);

        panel->SetSizer(mainSizer); // Assign the sizer to the panel

        // Bind an event handler to handle frame resizing
        Bind(wxEVT_SIZE, &CPlotWindow::OnResize, this);
    }

private:
	wxMenuBar* menuBar;
	wxMenu* fileMenu;
	wxMenu* helpMenu;
    wxStaticBitmap* staticBitmap;
    wxTextCtrl* textBox;
    wxBoxSizer* mainSizer;

    void OnResize(wxSizeEvent& event) {
        // Adjust the width of the text box to match the frame's width while keeping its height fixed
        wxSize size = GetClientSize();
        textBox->SetSize(size.GetWidth(), 100); // 100 is the fixed height of the text box
        event.Skip(); // Allow default handling of the resize event
    }
};

class CPlotApp : public wxApp {
public:
    virtual bool OnInit() {
        CPlotWindow* window = new CPlotWindow("CPlot");
        window->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(CPlotApp);
