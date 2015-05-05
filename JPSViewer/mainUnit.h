//---------------------------------------------------------------------------

#ifndef mainUnitH
#define mainUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "FImage.h"
#include <ActnList.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
// extern "C"
//{
        #include "JPSFile.h"
//}
//---------------------------------------------------------------------------
class TmainForm : public TForm
{
__published:	// IDE-managed Components
    TFImage *FImage1;
    TFImageToolkit *FImageToolkit1;
    TActionList *ActionList1;
    TAction *OpneImageAction;
    TPopupMenu *PopupMenu1;
    TMenuItem *N1;
    TMenuItem *N2;
    TAction *CloseAction;
    TMenuItem *N3;
    TOpenDialog *OpenDialog1;
    TAction *ModePaneAction;
    TAction *ModeSelRectAction;
    TMenuItem *N4;
    TMenuItem *N5;
    TMenuItem *N6;
    TMenuItem *N7;
    TAction *ActualSizeAction;
    TAction *BestFitAction;
    TMenuItem *N8;
    TMenuItem *N9;
    TAction *AboutAction;
    TMenuItem *N10;
    TAction *ViewTAnglfAction;
    TAction *ViewOAnglfAction;
    TAction *ViewGAnglfAction;
    TAction *ViewCAnglfAction;
    TAction *ViewSourceAction;
    TAction *ViewLeftAction;
    TAction *ViewRightAction;
    TMenuItem *N11;
    TMenuItem *N12;
    TMenuItem *N13;
    TMenuItem *N14;
    TMenuItem *N15;
    TMenuItem *N16;
    TMenuItem *N17;
    TMenuItem *N18;
    TMenuItem *N19;
    void __fastcall CloseActionExecute(TObject *Sender);
    void __fastcall OpneImageActionExecute(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall ModePaneActionExecute(TObject *Sender);
    void __fastcall ModePaneActionUpdate(TObject *Sender);
    void __fastcall ModeSelRectActionExecute(TObject *Sender);
    void __fastcall ModeSelRectActionUpdate(TObject *Sender);
    void __fastcall ActualSizeActionUpdate(TObject *Sender);
    void __fastcall ActualSizeActionExecute(TObject *Sender);
    void __fastcall BestFitActionExecute(TObject *Sender);
    void __fastcall BestFitActionUpdate(TObject *Sender);
    void __fastcall AboutActionExecute(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall ViewTAnglfActionExecute(TObject *Sender);
    void __fastcall ViewTAnglfActionUpdate(TObject *Sender);
    void __fastcall ViewSourceActionExecute(TObject *Sender);
    void __fastcall ViewSourceActionUpdate(TObject *Sender);
    void __fastcall ViewLeftActionUpdate(TObject *Sender);
    void __fastcall ViewLeftActionExecute(TObject *Sender);
    void __fastcall ViewRightActionExecute(TObject *Sender);
    void __fastcall ViewRightActionUpdate(TObject *Sender);
    void __fastcall ViewOAnglfActionExecute(TObject *Sender);
    void __fastcall ViewOAnglfActionUpdate(TObject *Sender);
    void __fastcall ViewGAnglfActionExecute(TObject *Sender);
    void __fastcall ViewGAnglfActionUpdate(TObject *Sender);
    void __fastcall ViewCAnglfActionExecute(TObject *Sender);
    void __fastcall ViewCAnglfActionUpdate(TObject *Sender);
private:	// User declarations

        IJPSFile* m_jpsFile;
        awpImage* GetAwpImage(IImage* image);

public:		// User declarations
    __fastcall TmainForm(TComponent* Owner);
    void __fastcall UpdateImage();
};
//---------------------------------------------------------------------------
extern PACKAGE TmainForm *mainForm;
//---------------------------------------------------------------------------
#endif
