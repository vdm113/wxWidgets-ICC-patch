/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////


// Name:        src/richtext/richtextborderspage.cpp
// Purpose:     
// Author:      Julian Smart
// Modified by: 
// Created:     21/10/2010 11:34:24
// RCS-ID:      
// Copyright:   (c) Julian Smart
// Licence:     
/////////////////////////////////////////////////////////////////////////////

////@begin includes
#include "wx/imaglist.h"
////@end includes

#include "wx/richtext/richtextborderspage.h"

////@begin XPM images
////@end XPM images


/*!
 * wxRichTextBordersPage type definition
 */

IMPLEMENT_DYNAMIC_CLASS( wxRichTextBordersPage, wxRichTextDialogPage )


/*!
 * wxRichTextBordersPage event table definition
 */

BEGIN_EVENT_TABLE( wxRichTextBordersPage, wxRichTextDialogPage )

    EVT_CHECKBOX(wxID_ANY, wxRichTextBordersPage::OnCommand)
    EVT_TEXT(wxID_ANY, wxRichTextBordersPage::OnCommand)
    EVT_TEXT_ENTER(wxID_ANY, wxRichTextBordersPage::OnCommand)
    EVT_COMBOBOX(wxID_ANY, wxRichTextBordersPage::OnCommand)
    EVT_BUTTON(wxID_ANY, wxRichTextBordersPage::OnCommand)

////@begin wxRichTextBordersPage event table entries
    EVT_CHECKBOX( ID_RICHTEXT_BORDER_LEFT_CHECKBOX, wxRichTextBordersPage::OnRichtextBorderCheckboxClick )

    EVT_UPDATE_UI( ID_RICHTEXT_BORDER_LEFT, wxRichTextBordersPage::OnRichtextBorderLeftUpdate )

    EVT_UPDATE_UI( ID_RICHTEXT_BORDER_LEFT_UNITS, wxRichTextBordersPage::OnRichtextBorderLeftUpdate )

    EVT_UPDATE_UI( ID_RICHTEXT_BORDER_LEFT_STYLE, wxRichTextBordersPage::OnRichtextBorderLeftUpdate )

    EVT_UPDATE_UI( ID_RICHTEXT_BORDER_LEFT_COLOUR, wxRichTextBordersPage::OnRichtextBorderLeftUpdate )

    EVT_CHECKBOX( ID_RICHTEXT_BORDER_RIGHT_CHECKBOX, wxRichTextBordersPage::OnRichtextBorderCheckboxClick )

    EVT_UPDATE_UI( ID_RICHTEXT_BORDER_RIGHT, wxRichTextBordersPage::OnRichtextBorderRightUpdate )

    EVT_UPDATE_UI( ID_RICHTEXT_BORDER_RIGHT_UNITS, wxRichTextBordersPage::OnRichtextBorderRightUpdate )

    EVT_UPDATE_UI( ID_RICHTEXT_BORDER_RIGHT_STYLE, wxRichTextBordersPage::OnRichtextBorderRightUpdate )

    EVT_UPDATE_UI( ID_RICHTEXT_BORDER_RIGHT_COLOUR, wxRichTextBordersPage::OnRichtextBorderRightUpdate )

    EVT_CHECKBOX( ID_RICHTEXT_BORDER_TOP_CHECKBOX, wxRichTextBordersPage::OnRichtextBorderCheckboxClick )

    EVT_UPDATE_UI( ID_RICHTEXT_BORDER_TOP, wxRichTextBordersPage::OnRichtextBorderTopUpdate )

    EVT_UPDATE_UI( ID_RICHTEXT_BORDER_TOP_UNITS, wxRichTextBordersPage::OnRichtextBorderTopUpdate )

    EVT_UPDATE_UI( ID_RICHTEXT_BORDER_TOP_STYLE, wxRichTextBordersPage::OnRichtextBorderTopUpdate )

    EVT_UPDATE_UI( ID_RICHTEXT_BORDER_TOP_COLOUR, wxRichTextBordersPage::OnRichtextBorderTopUpdate )

    EVT_CHECKBOX( ID_RICHTEXT_BORDER_BOTTOM_CHECKBOX, wxRichTextBordersPage::OnRichtextBorderCheckboxClick )

    EVT_UPDATE_UI( ID_RICHTEXT_BORDER_BOTTOM, wxRichTextBordersPage::OnRichtextBorderBottomUpdate )

    EVT_UPDATE_UI( ID_RICHTEXT_BORDER_BOTTOM_UNITS, wxRichTextBordersPage::OnRichtextBorderBottomUpdate )

    EVT_UPDATE_UI( ID_RICHTEXT_BORDER_BOTTOM_STYLE, wxRichTextBordersPage::OnRichtextBorderBottomUpdate )

    EVT_UPDATE_UI( ID_RICHTEXT_BORDER_BOTTOM_COLOUR, wxRichTextBordersPage::OnRichtextBorderBottomUpdate )

    EVT_CHECKBOX( ID_RICHTEXT_OUTLINE_LEFT_CHECKBOX, wxRichTextBordersPage::OnRichtextBorderCheckboxClick )

    EVT_UPDATE_UI( ID_RICHTEXT_OUTLINE_LEFT, wxRichTextBordersPage::OnRichtextOutlineLeftUpdate )

    EVT_UPDATE_UI( ID_RICHTEXT_OUTLINE_LEFT_UNITS, wxRichTextBordersPage::OnRichtextOutlineLeftUpdate )

    EVT_UPDATE_UI( ID_RICHTEXT_OUTLINE_LEFT_STYLE, wxRichTextBordersPage::OnRichtextOutlineLeftUpdate )

    EVT_UPDATE_UI( ID_RICHTEXT_OUTLINE_LEFT_COLOUR, wxRichTextBordersPage::OnRichtextOutlineLeftUpdate )

    EVT_CHECKBOX( ID_RICHTEXT_OUTLINE_RIGHT_CHECKBOX, wxRichTextBordersPage::OnRichtextBorderCheckboxClick )

    EVT_UPDATE_UI( ID_RICHTEXT_OUTLINE_RIGHT, wxRichTextBordersPage::OnRichtextOutlineRightUpdate )

    EVT_UPDATE_UI( ID_RICHTEXT_OUTLINE_RIGHT_UNITS, wxRichTextBordersPage::OnRichtextOutlineRightUpdate )

    EVT_UPDATE_UI( ID_RICHTEXT_OUTLINE_RIGHT_STYLE, wxRichTextBordersPage::OnRichtextOutlineRightUpdate )

    EVT_UPDATE_UI( ID_RICHTEXT_OUTLINE_RIGHT_COLOUR, wxRichTextBordersPage::OnRichtextOutlineRightUpdate )

    EVT_CHECKBOX( ID_RICHTEXT_OUTLINE_TOP_CHECKBOX, wxRichTextBordersPage::OnRichtextBorderCheckboxClick )

    EVT_UPDATE_UI( ID_RICHTEXT_OUTLINE_TOP, wxRichTextBordersPage::OnRichtextOutlineTopUpdate )

    EVT_UPDATE_UI( ID_RICHTEXT_OUTLINE_TOP_UNITS, wxRichTextBordersPage::OnRichtextOutlineTopUpdate )

    EVT_UPDATE_UI( ID_RICHTEXT_OUTLINE_TOP_STYLE, wxRichTextBordersPage::OnRichtextOutlineTopUpdate )

    EVT_UPDATE_UI( ID_RICHTEXT_OUTLINE_TOP_COLOUR, wxRichTextBordersPage::OnRichtextOutlineTopUpdate )

    EVT_CHECKBOX( ID_RICHTEXT_OUTLINE_BOTTOM_CHECKBOX, wxRichTextBordersPage::OnRichtextBorderCheckboxClick )

    EVT_UPDATE_UI( ID_RICHTEXT_OUTLINE_BOTTOM, wxRichTextBordersPage::OnRichtextOutlineBottomUpdate )

    EVT_UPDATE_UI( ID_RICHTEXT_OUTLINE_BOTTOM_UNITS, wxRichTextBordersPage::OnRichtextOutlineBottomUpdate )

    EVT_UPDATE_UI( ID_RICHTEXT_OUTLINE_BOTTOM_STYLE, wxRichTextBordersPage::OnRichtextOutlineBottomUpdate )

    EVT_UPDATE_UI( ID_RICHTEXT_OUTLINE_BOTTOM_COLOUR, wxRichTextBordersPage::OnRichtextOutlineBottomUpdate )

////@end wxRichTextBordersPage event table entries

END_EVENT_TABLE()

IMPLEMENT_HELP_PROVISION(wxRichTextBordersPage)

/*!
 * wxRichTextBordersPage constructors
 */

wxRichTextBordersPage::wxRichTextBordersPage()
{
    Init();
}

wxRichTextBordersPage::wxRichTextBordersPage( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, pos, size, style);
}


/*!
 * wxRichTextBordersPage creator
 */

bool wxRichTextBordersPage::Create( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
////@begin wxRichTextBordersPage creation
    SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    wxRichTextDialogPage::Create( parent, id, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end wxRichTextBordersPage creation
    return true;
}


/*!
 * wxRichTextBordersPage destructor
 */

wxRichTextBordersPage::~wxRichTextBordersPage()
{
////@begin wxRichTextBordersPage destruction
////@end wxRichTextBordersPage destruction
}


/*!
 * Member initialisation
 */

void wxRichTextBordersPage::Init()
{
    m_ignoreUpdates = false;

////@begin wxRichTextBordersPage member initialisation
    m_leftBorderCheckbox = NULL;
    m_leftBorderWidth = NULL;
    m_leftBorderWidthUnits = NULL;
    m_leftBorderStyle = NULL;
    m_leftBorderColour = NULL;
    m_rightBorderCheckbox = NULL;
    m_rightBorderWidth = NULL;
    m_rightBorderWidthUnits = NULL;
    m_rightBorderStyle = NULL;
    m_rightBorderColour = NULL;
    m_topBorderCheckbox = NULL;
    m_topBorderWidth = NULL;
    m_topBorderWidthUnits = NULL;
    m_topBorderStyle = NULL;
    m_topBorderColour = NULL;
    m_bottomBorderCheckbox = NULL;
    m_bottomBorderWidth = NULL;
    m_bottomBorderWidthUnits = NULL;
    m_bottomBorderStyle = NULL;
    m_bottomBorderColour = NULL;
    m_leftOutlineCheckbox = NULL;
    m_leftOutlineWidth = NULL;
    m_leftOutlineWidthUnits = NULL;
    m_leftOutlineStyle = NULL;
    m_leftOutlineColour = NULL;
    m_rightOutlineCheckbox = NULL;
    m_rightOutlineWidth = NULL;
    m_rightOutlineWidthUnits = NULL;
    m_rightOutlineStyle = NULL;
    m_rightOutlineColour = NULL;
    m_topOutlineCheckbox = NULL;
    m_topOutlineWidth = NULL;
    m_topOutlineWidthUnits = NULL;
    m_topOutlineStyle = NULL;
    m_topOutlineColour = NULL;
    m_bottomOutlineCheckbox = NULL;
    m_bottomOutlineWidth = NULL;
    m_bottomOutlineWidthUnits = NULL;
    m_bottomOutlineStyle = NULL;
    m_bottomOutlineColour = NULL;
    m_borderPreviewCtrl = NULL;
////@end wxRichTextBordersPage member initialisation
}


/*!
 * Control creation for wxRichTextBordersPage
 */

void wxRichTextBordersPage::CreateControls()
{    
////@begin wxRichTextBordersPage content construction
    wxRichTextBordersPage* itemRichTextDialogPage1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemRichTextDialogPage1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer2->Add(itemBoxSizer3, 1, wxGROW|wxALL, 5);

    wxNotebook* itemNotebook4 = new wxNotebook( itemRichTextDialogPage1, ID_RICHTEXTBORDERSPAGE_NOTEBOOK, wxDefaultPosition, wxDefaultSize, wxBK_DEFAULT );

    wxPanel* itemPanel5 = new wxPanel( itemNotebook4, ID_RICHTEXTBORDERSPAGE_BORDERS, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    itemPanel5->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxVERTICAL);
    itemPanel5->SetSizer(itemBoxSizer6);

    wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer6->Add(itemBoxSizer7, 0, wxGROW|wxALL, 5);
    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer7->Add(itemBoxSizer8, 0, wxGROW, 5);
    wxStaticText* itemStaticText9 = new wxStaticText( itemPanel5, wxID_STATIC, _("Border"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText9->SetFont(wxFont(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT).GetPointSize(), wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT).GetFamily(), wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT).GetStyle(), wxBOLD, false, wxT("")));
    itemBoxSizer8->Add(itemStaticText9, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticLine* itemStaticLine10 = new wxStaticLine( itemPanel5, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    itemBoxSizer8->Add(itemStaticLine10, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer11 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer7->Add(itemBoxSizer11, 0, wxGROW, 5);
    itemBoxSizer11->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer13 = new wxFlexGridSizer(0, 2, 0, 0);
    itemBoxSizer11->Add(itemFlexGridSizer13, 0, wxGROW, 5);
    m_leftBorderCheckbox = new wxCheckBox( itemPanel5, ID_RICHTEXT_BORDER_LEFT_CHECKBOX, _("&Left:"), wxDefaultPosition, wxDefaultSize, wxCHK_3STATE|wxCHK_ALLOW_3RD_STATE_FOR_USER );
    m_leftBorderCheckbox->SetValue(false);
    itemFlexGridSizer13->Add(m_leftBorderCheckbox, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer15 = new wxBoxSizer(wxHORIZONTAL);
    itemFlexGridSizer13->Add(itemBoxSizer15, 0, wxGROW|wxALIGN_CENTER_VERTICAL, 5);
    m_leftBorderWidth = new wxTextCtrl( itemPanel5, ID_RICHTEXT_BORDER_LEFT, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 0 );
    itemBoxSizer15->Add(m_leftBorderWidth, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

    wxArrayString m_leftBorderWidthUnitsStrings;
    m_leftBorderWidthUnitsStrings.Add(_("px"));
    m_leftBorderWidthUnitsStrings.Add(_("cm"));
    m_leftBorderWidthUnits = new wxComboBox( itemPanel5, ID_RICHTEXT_BORDER_LEFT_UNITS, _("px"), wxDefaultPosition, wxSize(60, -1), m_leftBorderWidthUnitsStrings, wxCB_READONLY );
    m_leftBorderWidthUnits->SetStringSelection(_("px"));
    m_leftBorderWidthUnits->SetHelpText(_("Units for the left border width."));
    if (wxRichTextBordersPage::ShowToolTips())
        m_leftBorderWidthUnits->SetToolTip(_("Units for the left border width."));
    itemBoxSizer15->Add(m_leftBorderWidthUnits, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer15->Add(2, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxArrayString m_leftBorderStyleStrings;
    m_leftBorderStyle = new wxComboBox( itemPanel5, ID_RICHTEXT_BORDER_LEFT_STYLE, wxEmptyString, wxDefaultPosition, wxDefaultSize, m_leftBorderStyleStrings, wxCB_READONLY );
    itemBoxSizer15->Add(m_leftBorderStyle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer15->Add(2, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    m_leftBorderColour = new wxRichTextColourSwatchCtrl( itemPanel5, ID_RICHTEXT_BORDER_LEFT_COLOUR, wxDefaultPosition, wxSize(40, 20), wxBORDER_THEME );
    itemBoxSizer15->Add(m_leftBorderColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_rightBorderCheckbox = new wxCheckBox( itemPanel5, ID_RICHTEXT_BORDER_RIGHT_CHECKBOX, _("&Right:"), wxDefaultPosition, wxDefaultSize, wxCHK_3STATE|wxCHK_ALLOW_3RD_STATE_FOR_USER );
    m_rightBorderCheckbox->SetValue(false);
    itemFlexGridSizer13->Add(m_rightBorderCheckbox, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer23 = new wxBoxSizer(wxHORIZONTAL);
    itemFlexGridSizer13->Add(itemBoxSizer23, 0, wxGROW|wxALIGN_CENTER_VERTICAL, 5);
    m_rightBorderWidth = new wxTextCtrl( itemPanel5, ID_RICHTEXT_BORDER_RIGHT, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 0 );
    itemBoxSizer23->Add(m_rightBorderWidth, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

    wxArrayString m_rightBorderWidthUnitsStrings;
    m_rightBorderWidthUnitsStrings.Add(_("px"));
    m_rightBorderWidthUnitsStrings.Add(_("cm"));
    m_rightBorderWidthUnits = new wxComboBox( itemPanel5, ID_RICHTEXT_BORDER_RIGHT_UNITS, _("px"), wxDefaultPosition, wxSize(60, -1), m_rightBorderWidthUnitsStrings, wxCB_READONLY );
    m_rightBorderWidthUnits->SetStringSelection(_("px"));
    m_rightBorderWidthUnits->SetHelpText(_("Units for the right border width."));
    if (wxRichTextBordersPage::ShowToolTips())
        m_rightBorderWidthUnits->SetToolTip(_("Units for the right border width."));
    itemBoxSizer23->Add(m_rightBorderWidthUnits, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer23->Add(2, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxArrayString m_rightBorderStyleStrings;
    m_rightBorderStyle = new wxComboBox( itemPanel5, ID_RICHTEXT_BORDER_RIGHT_STYLE, wxEmptyString, wxDefaultPosition, wxDefaultSize, m_rightBorderStyleStrings, wxCB_READONLY );
    itemBoxSizer23->Add(m_rightBorderStyle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer23->Add(2, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    m_rightBorderColour = new wxRichTextColourSwatchCtrl( itemPanel5, ID_RICHTEXT_BORDER_RIGHT_COLOUR, wxDefaultPosition, wxSize(40, 20), wxBORDER_THEME );
    itemBoxSizer23->Add(m_rightBorderColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_topBorderCheckbox = new wxCheckBox( itemPanel5, ID_RICHTEXT_BORDER_TOP_CHECKBOX, _("&Top:"), wxDefaultPosition, wxDefaultSize, wxCHK_3STATE|wxCHK_ALLOW_3RD_STATE_FOR_USER );
    m_topBorderCheckbox->SetValue(false);
    itemFlexGridSizer13->Add(m_topBorderCheckbox, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer31 = new wxBoxSizer(wxHORIZONTAL);
    itemFlexGridSizer13->Add(itemBoxSizer31, 0, wxGROW|wxALIGN_CENTER_VERTICAL, 5);
    m_topBorderWidth = new wxTextCtrl( itemPanel5, ID_RICHTEXT_BORDER_TOP, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 0 );
    itemBoxSizer31->Add(m_topBorderWidth, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

    wxArrayString m_topBorderWidthUnitsStrings;
    m_topBorderWidthUnitsStrings.Add(_("px"));
    m_topBorderWidthUnitsStrings.Add(_("cm"));
    m_topBorderWidthUnits = new wxComboBox( itemPanel5, ID_RICHTEXT_BORDER_TOP_UNITS, _("px"), wxDefaultPosition, wxSize(60, -1), m_topBorderWidthUnitsStrings, wxCB_READONLY );
    m_topBorderWidthUnits->SetStringSelection(_("px"));
    m_topBorderWidthUnits->SetHelpText(_("Units for the top border width."));
    if (wxRichTextBordersPage::ShowToolTips())
        m_topBorderWidthUnits->SetToolTip(_("Units for the top border width."));
    itemBoxSizer31->Add(m_topBorderWidthUnits, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer31->Add(2, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxArrayString m_topBorderStyleStrings;
    m_topBorderStyle = new wxComboBox( itemPanel5, ID_RICHTEXT_BORDER_TOP_STYLE, wxEmptyString, wxDefaultPosition, wxDefaultSize, m_topBorderStyleStrings, wxCB_READONLY );
    itemBoxSizer31->Add(m_topBorderStyle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer31->Add(2, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    m_topBorderColour = new wxRichTextColourSwatchCtrl( itemPanel5, ID_RICHTEXT_BORDER_TOP_COLOUR, wxDefaultPosition, wxSize(40, 20), wxBORDER_THEME );
    itemBoxSizer31->Add(m_topBorderColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_bottomBorderCheckbox = new wxCheckBox( itemPanel5, ID_RICHTEXT_BORDER_BOTTOM_CHECKBOX, _("&Bottom:"), wxDefaultPosition, wxDefaultSize, wxCHK_3STATE|wxCHK_ALLOW_3RD_STATE_FOR_USER );
    m_bottomBorderCheckbox->SetValue(false);
    itemFlexGridSizer13->Add(m_bottomBorderCheckbox, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer39 = new wxBoxSizer(wxHORIZONTAL);
    itemFlexGridSizer13->Add(itemBoxSizer39, 0, wxGROW|wxALIGN_CENTER_VERTICAL, 5);
    m_bottomBorderWidth = new wxTextCtrl( itemPanel5, ID_RICHTEXT_BORDER_BOTTOM, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 0 );
    itemBoxSizer39->Add(m_bottomBorderWidth, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

    wxArrayString m_bottomBorderWidthUnitsStrings;
    m_bottomBorderWidthUnitsStrings.Add(_("px"));
    m_bottomBorderWidthUnitsStrings.Add(_("cm"));
    m_bottomBorderWidthUnits = new wxComboBox( itemPanel5, ID_RICHTEXT_BORDER_BOTTOM_UNITS, _("px"), wxDefaultPosition, wxSize(60, -1), m_bottomBorderWidthUnitsStrings, wxCB_READONLY );
    m_bottomBorderWidthUnits->SetStringSelection(_("px"));
    m_bottomBorderWidthUnits->SetHelpText(_("Units for the bottom border width."));
    if (wxRichTextBordersPage::ShowToolTips())
        m_bottomBorderWidthUnits->SetToolTip(_("Units for the bottom border width."));
    itemBoxSizer39->Add(m_bottomBorderWidthUnits, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer39->Add(2, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxArrayString m_bottomBorderStyleStrings;
    m_bottomBorderStyle = new wxComboBox( itemPanel5, ID_RICHTEXT_BORDER_BOTTOM_STYLE, wxEmptyString, wxDefaultPosition, wxDefaultSize, m_bottomBorderStyleStrings, wxCB_READONLY );
    itemBoxSizer39->Add(m_bottomBorderStyle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer39->Add(2, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    m_bottomBorderColour = new wxRichTextColourSwatchCtrl( itemPanel5, ID_RICHTEXT_BORDER_BOTTOM_COLOUR, wxDefaultPosition, wxSize(40, 20), wxBORDER_THEME );
    itemBoxSizer39->Add(m_bottomBorderColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemNotebook4->AddPage(itemPanel5, _("Border"));

    wxPanel* itemPanel46 = new wxPanel( itemNotebook4, ID_RICHTEXTBORDERSPAGE_OUTLINE, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
    itemPanel46->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    wxBoxSizer* itemBoxSizer47 = new wxBoxSizer(wxVERTICAL);
    itemPanel46->SetSizer(itemBoxSizer47);

    wxBoxSizer* itemBoxSizer48 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer47->Add(itemBoxSizer48, 0, wxGROW|wxALL, 5);
    wxBoxSizer* itemBoxSizer49 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer48->Add(itemBoxSizer49, 0, wxGROW, 5);
    wxStaticText* itemStaticText50 = new wxStaticText( itemPanel46, wxID_STATIC, _("Outline"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText50->SetFont(wxFont(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT).GetPointSize(), wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT).GetFamily(), wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT).GetStyle(), wxBOLD, false, wxT("")));
    itemBoxSizer49->Add(itemStaticText50, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticLine* itemStaticLine51 = new wxStaticLine( itemPanel46, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    itemBoxSizer49->Add(itemStaticLine51, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer52 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer48->Add(itemBoxSizer52, 0, wxGROW, 5);
    itemBoxSizer52->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer54 = new wxFlexGridSizer(0, 2, 0, 0);
    itemBoxSizer52->Add(itemFlexGridSizer54, 0, wxGROW, 5);
    m_leftOutlineCheckbox = new wxCheckBox( itemPanel46, ID_RICHTEXT_OUTLINE_LEFT_CHECKBOX, _("&Left:"), wxDefaultPosition, wxDefaultSize, wxCHK_3STATE|wxCHK_ALLOW_3RD_STATE_FOR_USER );
    m_leftOutlineCheckbox->SetValue(false);
    itemFlexGridSizer54->Add(m_leftOutlineCheckbox, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer56 = new wxBoxSizer(wxHORIZONTAL);
    itemFlexGridSizer54->Add(itemBoxSizer56, 0, wxGROW|wxALIGN_CENTER_VERTICAL, 5);
    m_leftOutlineWidth = new wxTextCtrl( itemPanel46, ID_RICHTEXT_OUTLINE_LEFT, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 0 );
    itemBoxSizer56->Add(m_leftOutlineWidth, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

    wxArrayString m_leftOutlineWidthUnitsStrings;
    m_leftOutlineWidthUnitsStrings.Add(_("px"));
    m_leftOutlineWidthUnitsStrings.Add(_("cm"));
    m_leftOutlineWidthUnits = new wxComboBox( itemPanel46, ID_RICHTEXT_OUTLINE_LEFT_UNITS, _("px"), wxDefaultPosition, wxSize(60, -1), m_leftOutlineWidthUnitsStrings, wxCB_READONLY );
    m_leftOutlineWidthUnits->SetStringSelection(_("px"));
    m_leftOutlineWidthUnits->SetHelpText(_("Units for the left outline width."));
    if (wxRichTextBordersPage::ShowToolTips())
        m_leftOutlineWidthUnits->SetToolTip(_("Units for the left outline width."));
    itemBoxSizer56->Add(m_leftOutlineWidthUnits, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer56->Add(2, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxArrayString m_leftOutlineStyleStrings;
    m_leftOutlineStyle = new wxComboBox( itemPanel46, ID_RICHTEXT_OUTLINE_LEFT_STYLE, wxEmptyString, wxDefaultPosition, wxDefaultSize, m_leftOutlineStyleStrings, wxCB_READONLY );
    itemBoxSizer56->Add(m_leftOutlineStyle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer56->Add(2, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    m_leftOutlineColour = new wxRichTextColourSwatchCtrl( itemPanel46, ID_RICHTEXT_OUTLINE_LEFT_COLOUR, wxDefaultPosition, wxSize(40, 20), wxBORDER_THEME );
    itemBoxSizer56->Add(m_leftOutlineColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_rightOutlineCheckbox = new wxCheckBox( itemPanel46, ID_RICHTEXT_OUTLINE_RIGHT_CHECKBOX, _("&Right:"), wxDefaultPosition, wxDefaultSize, wxCHK_3STATE|wxCHK_ALLOW_3RD_STATE_FOR_USER );
    m_rightOutlineCheckbox->SetValue(false);
    itemFlexGridSizer54->Add(m_rightOutlineCheckbox, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer64 = new wxBoxSizer(wxHORIZONTAL);
    itemFlexGridSizer54->Add(itemBoxSizer64, 0, wxGROW|wxALIGN_CENTER_VERTICAL, 5);
    m_rightOutlineWidth = new wxTextCtrl( itemPanel46, ID_RICHTEXT_OUTLINE_RIGHT, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 0 );
    itemBoxSizer64->Add(m_rightOutlineWidth, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

    wxArrayString m_rightOutlineWidthUnitsStrings;
    m_rightOutlineWidthUnitsStrings.Add(_("px"));
    m_rightOutlineWidthUnitsStrings.Add(_("cm"));
    m_rightOutlineWidthUnits = new wxComboBox( itemPanel46, ID_RICHTEXT_OUTLINE_RIGHT_UNITS, _("px"), wxDefaultPosition, wxSize(60, -1), m_rightOutlineWidthUnitsStrings, wxCB_READONLY );
    m_rightOutlineWidthUnits->SetStringSelection(_("px"));
    m_rightOutlineWidthUnits->SetHelpText(_("Units for the right outline width."));
    if (wxRichTextBordersPage::ShowToolTips())
        m_rightOutlineWidthUnits->SetToolTip(_("Units for the right outline width."));
    itemBoxSizer64->Add(m_rightOutlineWidthUnits, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer64->Add(2, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxArrayString m_rightOutlineStyleStrings;
    m_rightOutlineStyle = new wxComboBox( itemPanel46, ID_RICHTEXT_OUTLINE_RIGHT_STYLE, wxEmptyString, wxDefaultPosition, wxDefaultSize, m_rightOutlineStyleStrings, wxCB_READONLY );
    itemBoxSizer64->Add(m_rightOutlineStyle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer64->Add(2, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    m_rightOutlineColour = new wxRichTextColourSwatchCtrl( itemPanel46, ID_RICHTEXT_OUTLINE_RIGHT_COLOUR, wxDefaultPosition, wxSize(40, 20), wxBORDER_THEME );
    itemBoxSizer64->Add(m_rightOutlineColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_topOutlineCheckbox = new wxCheckBox( itemPanel46, ID_RICHTEXT_OUTLINE_TOP_CHECKBOX, _("&Top:"), wxDefaultPosition, wxDefaultSize, wxCHK_3STATE|wxCHK_ALLOW_3RD_STATE_FOR_USER );
    m_topOutlineCheckbox->SetValue(false);
    itemFlexGridSizer54->Add(m_topOutlineCheckbox, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer72 = new wxBoxSizer(wxHORIZONTAL);
    itemFlexGridSizer54->Add(itemBoxSizer72, 0, wxGROW|wxALIGN_CENTER_VERTICAL, 5);
    m_topOutlineWidth = new wxTextCtrl( itemPanel46, ID_RICHTEXT_OUTLINE_TOP, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 0 );
    itemBoxSizer72->Add(m_topOutlineWidth, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

    wxArrayString m_topOutlineWidthUnitsStrings;
    m_topOutlineWidthUnitsStrings.Add(_("px"));
    m_topOutlineWidthUnitsStrings.Add(_("cm"));
    m_topOutlineWidthUnits = new wxComboBox( itemPanel46, ID_RICHTEXT_OUTLINE_TOP_UNITS, _("px"), wxDefaultPosition, wxSize(60, -1), m_topOutlineWidthUnitsStrings, wxCB_READONLY );
    m_topOutlineWidthUnits->SetStringSelection(_("px"));
    m_topOutlineWidthUnits->SetHelpText(_("Units for the top outline width."));
    if (wxRichTextBordersPage::ShowToolTips())
        m_topOutlineWidthUnits->SetToolTip(_("Units for the top outline width."));
    itemBoxSizer72->Add(m_topOutlineWidthUnits, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer72->Add(2, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxArrayString m_topOutlineStyleStrings;
    m_topOutlineStyle = new wxComboBox( itemPanel46, ID_RICHTEXT_OUTLINE_TOP_STYLE, wxEmptyString, wxDefaultPosition, wxDefaultSize, m_topOutlineStyleStrings, wxCB_READONLY );
    itemBoxSizer72->Add(m_topOutlineStyle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer72->Add(2, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    m_topOutlineColour = new wxRichTextColourSwatchCtrl( itemPanel46, ID_RICHTEXT_OUTLINE_TOP_COLOUR, wxDefaultPosition, wxSize(40, 20), wxBORDER_THEME );
    itemBoxSizer72->Add(m_topOutlineColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_bottomOutlineCheckbox = new wxCheckBox( itemPanel46, ID_RICHTEXT_OUTLINE_BOTTOM_CHECKBOX, _("&Bottom:"), wxDefaultPosition, wxDefaultSize, wxCHK_3STATE|wxCHK_ALLOW_3RD_STATE_FOR_USER );
    m_bottomOutlineCheckbox->SetValue(false);
    itemFlexGridSizer54->Add(m_bottomOutlineCheckbox, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer80 = new wxBoxSizer(wxHORIZONTAL);
    itemFlexGridSizer54->Add(itemBoxSizer80, 0, wxGROW|wxALIGN_CENTER_VERTICAL, 5);
    m_bottomOutlineWidth = new wxTextCtrl( itemPanel46, ID_RICHTEXT_OUTLINE_BOTTOM, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 0 );
    itemBoxSizer80->Add(m_bottomOutlineWidth, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

    wxArrayString m_bottomOutlineWidthUnitsStrings;
    m_bottomOutlineWidthUnitsStrings.Add(_("px"));
    m_bottomOutlineWidthUnitsStrings.Add(_("cm"));
    m_bottomOutlineWidthUnits = new wxComboBox( itemPanel46, ID_RICHTEXT_OUTLINE_BOTTOM_UNITS, _("px"), wxDefaultPosition, wxSize(60, -1), m_bottomOutlineWidthUnitsStrings, wxCB_READONLY );
    m_bottomOutlineWidthUnits->SetStringSelection(_("px"));
    m_bottomOutlineWidthUnits->SetHelpText(_("Units for the bottom outline width."));
    if (wxRichTextBordersPage::ShowToolTips())
        m_bottomOutlineWidthUnits->SetToolTip(_("Units for the bottom outline width."));
    itemBoxSizer80->Add(m_bottomOutlineWidthUnits, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer80->Add(2, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    wxArrayString m_bottomOutlineStyleStrings;
    m_bottomOutlineStyle = new wxComboBox( itemPanel46, ID_RICHTEXT_OUTLINE_BOTTOM_STYLE, wxEmptyString, wxDefaultPosition, wxDefaultSize, m_bottomOutlineStyleStrings, wxCB_READONLY );
    itemBoxSizer80->Add(m_bottomOutlineStyle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer80->Add(2, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

    m_bottomOutlineColour = new wxRichTextColourSwatchCtrl( itemPanel46, ID_RICHTEXT_OUTLINE_BOTTOM_COLOUR, wxDefaultPosition, wxSize(40, 20), wxBORDER_THEME );
    itemBoxSizer80->Add(m_bottomOutlineColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemNotebook4->AddPage(itemPanel46, _("Outline"));

    itemBoxSizer3->Add(itemNotebook4, 0, wxGROW|wxALL, 5);

    m_borderPreviewCtrl = new wxRichTextBorderPreviewCtrl( itemRichTextDialogPage1, ID_RICHTEXT_BORDER_PREVIEW, wxDefaultPosition, wxSize(60, 60), wxBORDER_THEME );
    itemBoxSizer3->Add(m_borderPreviewCtrl, 1, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);

////@end wxRichTextBordersPage content construction

    m_borderStyles.Add(wxTEXT_BOX_ATTR_BORDER_NONE); m_borderStyleNames.Add(_("None"));
    m_borderStyles.Add(wxTEXT_BOX_ATTR_BORDER_SOLID); m_borderStyleNames.Add(_("Solid"));
    m_borderStyles.Add(wxTEXT_BOX_ATTR_BORDER_DOTTED); m_borderStyleNames.Add(_("Dotted"));
    m_borderStyles.Add(wxTEXT_BOX_ATTR_BORDER_DASHED); m_borderStyleNames.Add(_("Dashed"));
    m_borderStyles.Add(wxTEXT_BOX_ATTR_BORDER_DOUBLE); m_borderStyleNames.Add(_("Double"));
    m_borderStyles.Add(wxTEXT_BOX_ATTR_BORDER_GROOVE); m_borderStyleNames.Add(_("Groove"));
    m_borderStyles.Add(wxTEXT_BOX_ATTR_BORDER_RIDGE); m_borderStyleNames.Add(_("Ridge"));
    m_borderStyles.Add(wxTEXT_BOX_ATTR_BORDER_INSET); m_borderStyleNames.Add(_("Inset"));
    m_borderStyles.Add(wxTEXT_BOX_ATTR_BORDER_OUTSET); m_borderStyleNames.Add(_("Outset"));

    m_ignoreUpdates = true;

    FillStyleComboBox(m_leftBorderStyle);
    FillStyleComboBox(m_rightBorderStyle);
    FillStyleComboBox(m_topBorderStyle);
    FillStyleComboBox(m_bottomBorderStyle);
    
    FillStyleComboBox(m_leftOutlineStyle);
    FillStyleComboBox(m_rightOutlineStyle);
    FillStyleComboBox(m_topOutlineStyle);
    FillStyleComboBox(m_bottomOutlineStyle);
    
    m_borderPreviewCtrl->SetAttributes(GetAttributes());

    m_ignoreUpdates = false;
}


/*!
 * Should we show tooltips?
 */

bool wxRichTextBordersPage::ShowToolTips()
{
    return true;
}

// Updates the preview
void wxRichTextBordersPage::OnCommand(wxCommandEvent& event)
{
    event.Skip();
    if (m_ignoreUpdates)
        return;

    if (m_borderPreviewCtrl)
    {
        TransferDataFromWindow();
        m_borderPreviewCtrl->Refresh();
    }
}

wxRichTextAttr* wxRichTextBordersPage::GetAttributes()
{
    return wxRichTextFormattingDialog::GetDialogAttributes(this);
}

// Fill style combo
void wxRichTextBordersPage::FillStyleComboBox(wxComboBox* styleComboBox)
{
    styleComboBox->Freeze();
    styleComboBox->Append(m_borderStyleNames);
    styleComboBox->Thaw();    
}

bool wxRichTextBordersPage::TransferDataToWindow()
{
    m_ignoreUpdates = true;

    // Border
    SetBorderValue(GetAttributes()->GetTextBoxAttr().GetBorder().GetLeft(), m_leftBorderWidth, m_leftBorderWidthUnits, m_leftBorderCheckbox, m_leftBorderStyle, m_leftBorderColour, m_borderStyles);
    SetBorderValue(GetAttributes()->GetTextBoxAttr().GetBorder().GetRight(), m_rightBorderWidth, m_rightBorderWidthUnits, m_rightBorderCheckbox, m_rightBorderStyle, m_rightBorderColour, m_borderStyles);
    SetBorderValue(GetAttributes()->GetTextBoxAttr().GetBorder().GetTop(), m_topBorderWidth, m_topBorderWidthUnits, m_topBorderCheckbox, m_topBorderStyle, m_topBorderColour, m_borderStyles);
    SetBorderValue(GetAttributes()->GetTextBoxAttr().GetBorder().GetBottom(), m_bottomBorderWidth, m_bottomBorderWidthUnits, m_bottomBorderCheckbox, m_bottomBorderStyle, m_bottomBorderColour, m_borderStyles);

    // Outline
    SetBorderValue(GetAttributes()->GetTextBoxAttr().GetOutline().GetLeft(), m_leftOutlineWidth, m_leftOutlineWidthUnits, m_leftOutlineCheckbox, m_leftOutlineStyle, m_leftOutlineColour, m_borderStyles);
    SetBorderValue(GetAttributes()->GetTextBoxAttr().GetOutline().GetRight(), m_rightOutlineWidth, m_rightOutlineWidthUnits, m_rightOutlineCheckbox, m_rightOutlineStyle, m_rightOutlineColour, m_borderStyles);
    SetBorderValue(GetAttributes()->GetTextBoxAttr().GetOutline().GetTop(), m_topOutlineWidth, m_topOutlineWidthUnits, m_topOutlineCheckbox, m_topOutlineStyle, m_topOutlineColour, m_borderStyles);
    SetBorderValue(GetAttributes()->GetTextBoxAttr().GetOutline().GetBottom(), m_bottomOutlineWidth, m_bottomOutlineWidthUnits, m_bottomOutlineCheckbox, m_bottomOutlineStyle, m_bottomOutlineColour, m_borderStyles);

    m_ignoreUpdates = false;

    return true;
}

bool wxRichTextBordersPage::TransferDataFromWindow()
{
    // Border
    GetBorderValue(GetAttributes()->GetTextBoxAttr().GetBorder().GetLeft(), m_leftBorderWidth, m_leftBorderWidthUnits, m_leftBorderCheckbox, m_leftBorderStyle, m_leftBorderColour, m_borderStyles);
    GetBorderValue(GetAttributes()->GetTextBoxAttr().GetBorder().GetRight(), m_rightBorderWidth, m_rightBorderWidthUnits, m_rightBorderCheckbox, m_rightBorderStyle, m_rightBorderColour, m_borderStyles);
    GetBorderValue(GetAttributes()->GetTextBoxAttr().GetBorder().GetTop(), m_topBorderWidth, m_topBorderWidthUnits, m_topBorderCheckbox, m_topBorderStyle, m_topBorderColour, m_borderStyles);
    GetBorderValue(GetAttributes()->GetTextBoxAttr().GetBorder().GetBottom(), m_bottomBorderWidth, m_bottomBorderWidthUnits, m_bottomBorderCheckbox, m_bottomBorderStyle, m_bottomBorderColour, m_borderStyles);

    // Outline
    GetBorderValue(GetAttributes()->GetTextBoxAttr().GetOutline().GetLeft(), m_leftOutlineWidth, m_leftOutlineWidthUnits, m_leftOutlineCheckbox, m_leftOutlineStyle, m_leftOutlineColour, m_borderStyles);
    GetBorderValue(GetAttributes()->GetTextBoxAttr().GetOutline().GetRight(), m_rightOutlineWidth, m_rightOutlineWidthUnits, m_rightOutlineCheckbox, m_rightOutlineStyle, m_rightOutlineColour, m_borderStyles);
    GetBorderValue(GetAttributes()->GetTextBoxAttr().GetOutline().GetTop(), m_topOutlineWidth, m_topOutlineWidthUnits, m_topOutlineCheckbox, m_topOutlineStyle, m_topOutlineColour, m_borderStyles);
    GetBorderValue(GetAttributes()->GetTextBoxAttr().GetOutline().GetBottom(), m_bottomOutlineWidth, m_bottomOutlineWidthUnits, m_bottomOutlineCheckbox, m_bottomOutlineStyle, m_bottomOutlineColour, m_borderStyles);

    return true;
}

// Set the border controls
void wxRichTextBordersPage::SetBorderValue(wxTextAttrBorder& border, /* wxTextAttrBorder& borderToReset, */ wxTextCtrl* widthValueCtrl, wxComboBox* widthUnitsCtrl, wxCheckBox* checkBox,
        wxComboBox* styleCtrl, wxRichTextColourSwatchCtrl* colourCtrl, const wxArrayInt& borderStyles)
{
    if (!border.IsValid())
    {
        checkBox->Set3StateValue(wxCHK_UNDETERMINED);
        widthValueCtrl->SetValue(wxT("1"));
        widthUnitsCtrl->SetSelection(0);
        colourCtrl->SetColour(*wxBLACK);
        styleCtrl->SetSelection(0);
    }
    else
    {
        wxRichTextFormattingDialog::SetDimensionValue(border.GetWidth(), widthValueCtrl, widthUnitsCtrl, checkBox);

        int sel = borderStyles.Index(border.GetStyle());
        if (sel == -1)
            sel = 1;
        styleCtrl->SetSelection(sel);        
        colourCtrl->SetColour(border.GetColour());
        
        if (sel == 0)
            checkBox->Set3StateValue(wxCHK_UNCHECKED);
        else
            checkBox->Set3StateValue(wxCHK_CHECKED);
    }
}

// Get data from the border controls
void wxRichTextBordersPage::GetBorderValue(wxTextAttrBorder& border, /* wxTextAttrBorder& borderToReset, */ wxTextCtrl* widthValueCtrl, wxComboBox* widthUnitsCtrl, wxCheckBox* checkBox,
        wxComboBox* styleCtrl, wxRichTextColourSwatchCtrl* colourCtrl, const wxArrayInt& borderStyles)
{
    wxRichTextFormattingDialog::GetDimensionValue(border.GetWidth(), widthValueCtrl, widthUnitsCtrl, checkBox);

    int sel = styleCtrl->GetSelection();
    border.SetColour(colourCtrl->GetColour());

    if (checkBox->Get3StateValue() == wxCHK_UNDETERMINED)
    {
        // When we apply the attributes, we won't apply this one, to leave the original unchanged.
        border.Reset();
        // borderToReset.Reset();
    }
    else if (checkBox->Get3StateValue() == wxCHK_UNCHECKED)
    {
        // We make a note to reset this attribute.
        // borderToReset.GetWidth().MakeValid();
        border.SetStyle(wxTEXT_BOX_ATTR_BORDER_NONE);
    }
    else
    {
        // borderToReset.Reset(); // Don't reset this, in case we were going to previously.
        if (sel != -1)
            border.SetStyle(borderStyles[sel]);
    }
}

/*!
 * Get bitmap resources
 */

wxBitmap wxRichTextBordersPage::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin wxRichTextBordersPage bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end wxRichTextBordersPage bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon wxRichTextBordersPage::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin wxRichTextBordersPage icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end wxRichTextBordersPage icon retrieval
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_RICHTEXT_BORDER_LEFT
 */

void wxRichTextBordersPage::OnRichtextBorderLeftUpdate( wxUpdateUIEvent& event )
{
    event.Enable(m_leftBorderCheckbox->Get3StateValue() == wxCHK_CHECKED);
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_RICHTEXT_BORDER_RIGHT
 */

void wxRichTextBordersPage::OnRichtextBorderRightUpdate( wxUpdateUIEvent& event )
{
    event.Enable(m_rightBorderCheckbox->Get3StateValue() == wxCHK_CHECKED);
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_RICHTEXT_BORDER_TOP
 */

void wxRichTextBordersPage::OnRichtextBorderTopUpdate( wxUpdateUIEvent& event )
{
    event.Enable(m_topBorderCheckbox->Get3StateValue() == wxCHK_CHECKED);
}

/*!
 * wxEVT_UPDATE_UI event handler for ID_RICHTEXT_BORDER_BOTTOM
 */

void wxRichTextBordersPage::OnRichtextBorderBottomUpdate( wxUpdateUIEvent& event )
{
    event.Enable(m_bottomBorderCheckbox->Get3StateValue() == wxCHK_CHECKED);
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_RICHTEXT_OUTLINE_LEFT
 */

void wxRichTextBordersPage::OnRichtextOutlineLeftUpdate( wxUpdateUIEvent& event )
{
    event.Enable(m_leftOutlineCheckbox->Get3StateValue() == wxCHK_CHECKED);
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_RICHTEXT_OUTLINE_RIGHT
 */

void wxRichTextBordersPage::OnRichtextOutlineRightUpdate( wxUpdateUIEvent& event )
{
    event.Enable(m_rightOutlineCheckbox->Get3StateValue() == wxCHK_CHECKED);
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_RICHTEXT_OUTLINE_TOP
 */

void wxRichTextBordersPage::OnRichtextOutlineTopUpdate( wxUpdateUIEvent& event )
{
    event.Enable(m_topOutlineCheckbox->Get3StateValue() == wxCHK_CHECKED);
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_RICHTEXT_OUTLINE_BOTTOM
 */

void wxRichTextBordersPage::OnRichtextOutlineBottomUpdate( wxUpdateUIEvent& event )
{
    event.Enable(m_bottomOutlineCheckbox->Get3StateValue() == wxCHK_CHECKED);
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_RICHTEXT_BORDER_LEFT_CHECKBOX
 */

void wxRichTextBordersPage::OnRichtextBorderCheckboxClick( wxCommandEvent& event )
{
    if (m_ignoreUpdates)
        return;

    m_ignoreUpdates = true;
    wxCheckBox* checkBox = NULL;
    wxComboBox* comboBox = NULL;
    if (event.GetId() == ID_RICHTEXT_OUTLINE_LEFT_CHECKBOX)
    {
        checkBox = m_leftOutlineCheckbox;
        comboBox = m_leftOutlineStyle;
    }
    else if (event.GetId() == ID_RICHTEXT_OUTLINE_TOP_CHECKBOX)
    {
        checkBox = m_topOutlineCheckbox;
        comboBox = m_topOutlineStyle;
    }
    else if (event.GetId() == ID_RICHTEXT_OUTLINE_RIGHT_CHECKBOX)
    {
        checkBox = m_rightOutlineCheckbox;
        comboBox = m_rightOutlineStyle;
    }
    else if (event.GetId() == ID_RICHTEXT_OUTLINE_BOTTOM_CHECKBOX)
    {
        checkBox = m_bottomOutlineCheckbox;
        comboBox = m_bottomOutlineStyle;
    }
    else if (event.GetId() == ID_RICHTEXT_BORDER_LEFT_CHECKBOX)
    {
        checkBox = m_leftBorderCheckbox;
        comboBox = m_leftBorderStyle;
    }
    else if (event.GetId() == ID_RICHTEXT_BORDER_TOP_CHECKBOX)
    {
        checkBox = m_topBorderCheckbox;
        comboBox = m_topBorderStyle;
    }
    else if (event.GetId() == ID_RICHTEXT_BORDER_RIGHT_CHECKBOX)
    {
        checkBox = m_rightBorderCheckbox;
        comboBox = m_rightBorderStyle;
    }
    else if (event.GetId() == ID_RICHTEXT_BORDER_BOTTOM_CHECKBOX)
    {
        checkBox = m_bottomBorderCheckbox;
        comboBox = m_bottomBorderStyle;
    }
    
    if (checkBox && comboBox)
    {
        if (checkBox->Get3StateValue() == wxCHK_UNCHECKED || checkBox->Get3StateValue() == wxCHK_UNDETERMINED)
            comboBox->SetSelection(0);
        else
            comboBox->SetSelection(1);
        
        TransferDataFromWindow();
        m_borderPreviewCtrl->Refresh();
    }

    m_ignoreUpdates = false;
}

BEGIN_EVENT_TABLE(wxRichTextBorderPreviewCtrl, wxWindow)
    EVT_PAINT(wxRichTextBorderPreviewCtrl::OnPaint)
END_EVENT_TABLE()

wxRichTextBorderPreviewCtrl::wxRichTextBorderPreviewCtrl(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& sz, long style)
{
    if ((style & wxBORDER_MASK) == wxBORDER_DEFAULT)
        style |= wxBORDER_THEME;

    m_attributes = NULL;

    wxWindow::Create(parent, id, pos, sz, style);
    SetBackgroundColour(*wxWHITE);
}

void wxRichTextBorderPreviewCtrl::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    wxPaintDC dc(this);

    if (m_attributes)
    {
        wxRect rect = GetClientRect();
        
        int margin = 10;
        rect.x += margin;
        rect.y += margin;
        rect.width -= 2*margin;
        rect.height -= 2*margin;
        
        wxRichTextObject::DrawBorder(dc, NULL, m_attributes->GetTextBoxAttr().GetOutline(), rect);

        rect.x += margin;
        rect.y += margin;
        rect.width -= 2*margin;
        rect.height -= 2*margin;
        
        wxRichTextObject::DrawBorder(dc, NULL, m_attributes->GetTextBoxAttr().GetBorder(), rect);
    }
}
