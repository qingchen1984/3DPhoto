//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "mainUnit.h"
#include "aboutUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "FImage"

 //extern "C"
//{
       #pragma link "JPSFile.lib"
//}

#pragma resource "*.dfm"
TmainForm *mainForm;
//---------------------------------------------------------------------------
__fastcall TmainForm::TmainForm(TComponent* Owner)
    : TForm(Owner)
{
        m_jpsFile = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TmainForm::CloseActionExecute(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TmainForm::OpneImageActionExecute(TObject *Sender)
{
    if (OpenDialog1->Execute())
    {
        AnsiString strExt = ExtractFileExt(OpenDialog1->FileName);

        if (strExt == ".jps")
        {
            if (!this->m_jpsFile->LoadFromFile(OpenDialog1->FileName.c_str()))
            {
                ShowMessage("Ќе могу открыть файл.");
                return;
            }
        }
        else
        {
           if (!this->m_jpsFile->InitFromFile(OpenDialog1->FileName.c_str()))
            {
                ShowMessage("Ќе могу открыть файл.");
                return;
            }
        }

        IImage* img = this->m_jpsFile->GetJPS();
        awpImage* awp = GetAwpImage(img);
/*
        awpCreateImage(&awp, img->Width(), img->Height(), 3, AWP_BYTE);

        unsigned char* p = img->Pixels();
        AWPBYTE* b = (AWPBYTE*)awp->pPixels;
        int size = awp->sSizeX*awp->bChannels;
        for (int i = 0; i < awp->sSizeY; i++)
        {
            memcpy(b, p, size);
            b+= size;
            p+= img->WidthStep();
        }
*/
        FImage1->Bitmap->SetAWPImage(awp);
        awpReleaseImage(&awp);
        img->Rlease();

        UpdateImage();
    }
}

awpImage* TmainForm::GetAwpImage(IImage* img)
{
    awpImage* awp = NULL;

    awpCreateImage(&awp, img->Width(), img->Height(), 3, AWP_BYTE);

    unsigned char* p = img->Pixels();
    AWPBYTE* b = (AWPBYTE*)awp->pPixels;
    int size = awp->sSizeX*awp->bChannels;
    for (int i = 0; i < awp->sSizeY; i++)
    {
        memcpy(b, p, size);
        b+= size;
        p+= img->WidthStep();
    }
    return awp;
}

void __fastcall TmainForm::UpdateImage()
{
        // установка размеров изображени€.
        // если размер изображени€ больше размеров окна -
        // выполн€ем операцию BestFit
        // иначе выполн€ем операцию ActualSize

        int w = FImage1->Width;
        int h = FImage1->Height;
        int w1 = FImage1->Bitmap->Width;
        int h1 = FImage1->Bitmap->Height;

        if (w1 > w || h1 > h)
            FImage1->BestFit();
        else
            FImage1->ActualSize();

}
//---------------------------------------------------------------------------
void __fastcall TmainForm::FormResize(TObject *Sender)
{
    UpdateImage();
}
//---------------------------------------------------------------------------
void __fastcall TmainForm::ModePaneActionExecute(TObject *Sender)
{
    FImageToolkit1->CurrentTool = ttPane;    
}
//---------------------------------------------------------------------------
void __fastcall TmainForm::ModePaneActionUpdate(TObject *Sender)
{
    ModePaneAction->Checked =  FImageToolkit1->CurrentTool == ttPane;
}
//---------------------------------------------------------------------------
void __fastcall TmainForm::ModeSelRectActionExecute(TObject *Sender)
{
    FImageToolkit1->CurrentTool = ttSelRect;
}
//---------------------------------------------------------------------------
void __fastcall TmainForm::ModeSelRectActionUpdate(TObject *Sender)
{
    ModeSelRectAction->Checked =  FImageToolkit1->CurrentTool == ttSelRect;
}
//---------------------------------------------------------------------------
void __fastcall TmainForm::ActualSizeActionUpdate(TObject *Sender)
{
    ActualSizeAction->Enabled = !FImage1->Bitmap->Empty;
}
//---------------------------------------------------------------------------

void __fastcall TmainForm::ActualSizeActionExecute(TObject *Sender)
{
    FImage1->ActualSize();
}
//---------------------------------------------------------------------------

void __fastcall TmainForm::BestFitActionExecute(TObject *Sender)
{
    FImage1->BestFit();    
}
//---------------------------------------------------------------------------

void __fastcall TmainForm::BestFitActionUpdate(TObject *Sender)
{
     BestFitAction->Enabled = !FImage1->Bitmap->Empty;
}
//---------------------------------------------------------------------------


void __fastcall TmainForm::AboutActionExecute(TObject *Sender)
{
    AboutBox->ShowModal();    
}
//---------------------------------------------------------------------------

void __fastcall TmainForm::FormCreate(TObject *Sender)
{
    if(ParamCount() > 0)
    {
        FImage1->Init(ParamStr(1), NULL);
        FImage1->BestFit();
    }

    if (createJPSFile(&this->m_jpsFile) != S_OK)
    {
        ShowMessage("Cannot create engine.");
    }

}
//---------------------------------------------------------------------------

void __fastcall TmainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
        if (m_jpsFile != NULL)
                m_jpsFile->Rlease();

}
//---------------------------------------------------------------------------

void __fastcall TmainForm::ViewTAnglfActionExecute(TObject *Sender)
{
    IImage* img = this->m_jpsFile->GetTrueAnaglyph();
    awpImage* awp = GetAwpImage(img);
    FImage1->Bitmap->SetAWPImage(awp);
    awpReleaseImage(&awp);
    img->Rlease();
    UpdateImage();
}
//---------------------------------------------------------------------------

void __fastcall TmainForm::ViewTAnglfActionUpdate(TObject *Sender)
{
    ViewTAnglfAction->Enabled = !this->m_jpsFile->IsEmpty();
}
//---------------------------------------------------------------------------

void __fastcall TmainForm::ViewSourceActionExecute(TObject *Sender)
{
    IImage* img = this->m_jpsFile->GetJPS();
    awpImage* awp = GetAwpImage(img);
    FImage1->Bitmap->SetAWPImage(awp);
    awpReleaseImage(&awp);
    img->Rlease();
    UpdateImage();
}
//---------------------------------------------------------------------------

void __fastcall TmainForm::ViewSourceActionUpdate(TObject *Sender)
{
    ViewSourceAction->Enabled = !this->m_jpsFile->IsEmpty();
}
//---------------------------------------------------------------------------


void __fastcall TmainForm::ViewLeftActionUpdate(TObject *Sender)
{
    ViewLeftAction->Enabled = !this->m_jpsFile->IsEmpty();
}
//---------------------------------------------------------------------------

void __fastcall TmainForm::ViewLeftActionExecute(TObject *Sender)
{
    IImage* img = this->m_jpsFile->GetLep();
    awpImage* awp = GetAwpImage(img);
    FImage1->Bitmap->SetAWPImage(awp);
    awpReleaseImage(&awp);
    img->Rlease();
    UpdateImage();
}
//---------------------------------------------------------------------------

void __fastcall TmainForm::ViewRightActionExecute(TObject *Sender)
{
    IImage* img = this->m_jpsFile->GetRep();
    awpImage* awp = GetAwpImage(img);
    FImage1->Bitmap->SetAWPImage(awp);
    awpReleaseImage(&awp);
    img->Rlease();
    UpdateImage();
}
//---------------------------------------------------------------------------

void __fastcall TmainForm::ViewRightActionUpdate(TObject *Sender)
{
    ViewRightAction->Enabled = !this->m_jpsFile->IsEmpty();
}
//---------------------------------------------------------------------------


void __fastcall TmainForm::ViewOAnglfActionExecute(TObject *Sender)
{
    IImage* img = this->m_jpsFile->GetOptimizedAnglyph() ;
    awpImage* awp = GetAwpImage(img);
    FImage1->Bitmap->SetAWPImage(awp);
    awpReleaseImage(&awp);
    img->Rlease();
    UpdateImage();
}
//---------------------------------------------------------------------------

void __fastcall TmainForm::ViewOAnglfActionUpdate(TObject *Sender)
{
    ViewOAnglfAction->Enabled = !this->m_jpsFile->IsEmpty();
}
//---------------------------------------------------------------------------


void __fastcall TmainForm::ViewGAnglfActionExecute(TObject *Sender)
{
    IImage* img = this->m_jpsFile->GetGrayAnaglyph();
    awpImage* awp = GetAwpImage(img);
    FImage1->Bitmap->SetAWPImage(awp);
    awpReleaseImage(&awp);
    img->Rlease();
    UpdateImage();
}
//---------------------------------------------------------------------------

void __fastcall TmainForm::ViewGAnglfActionUpdate(TObject *Sender)
{
    ViewGAnglfAction->Enabled = !this->m_jpsFile->IsEmpty();
}
//---------------------------------------------------------------------------

void __fastcall TmainForm::ViewCAnglfActionExecute(TObject *Sender)
{
    IImage* img = this->m_jpsFile->GetColorAnaglyph();
    awpImage* awp = GetAwpImage(img);
    FImage1->Bitmap->SetAWPImage(awp);
    awpReleaseImage(&awp);
    img->Rlease();
    UpdateImage();
}
//---------------------------------------------------------------------------

void __fastcall TmainForm::ViewCAnglfActionUpdate(TObject *Sender)
{
    ViewCAnglfAction->Enabled = !this->m_jpsFile->IsEmpty();
}
//---------------------------------------------------------------------------

