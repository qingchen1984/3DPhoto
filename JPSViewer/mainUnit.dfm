object mainForm: TmainForm
  Left = 195
  Top = 128
  Width = 482
  Height = 476
  Caption = 'JPS Viewer'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object FImage1: TFImage
    Left = 0
    Top = 0
    Width = 466
    Height = 437
    Cursor = 1
    BorderStyle = bsFSingle
    Align = alClient
    ParentColor = False
    PopupMenu = PopupMenu1
  end
  object FImageToolkit1: TFImageToolkit
    CurrentTool = ttPane
    Image = FImage1
    Left = 16
    Top = 24
  end
  object ActionList1: TActionList
    Left = 56
    Top = 24
    object OpneImageAction: TAction
      Caption = #1054#1090#1082#1088#1099#1090#1100' '#1080#1079#1086#1073#1088#1072#1078#1077#1085#1080#1077'...'
      OnExecute = OpneImageActionExecute
    end
    object CloseAction: TAction
      Caption = #1042#1099#1093#1086#1076
      OnExecute = CloseActionExecute
    end
    object ModePaneAction: TAction
      Caption = #1056#1077#1078#1080#1084' '#1087#1088#1086#1089#1084#1086#1090#1088#1072
      OnExecute = ModePaneActionExecute
      OnUpdate = ModePaneActionUpdate
    end
    object ModeSelRectAction: TAction
      Caption = #1042#1099#1073#1088#1072#1090#1100' '#1087#1088#1103#1084#1086#1091#1075#1086#1083#1100#1085#1080#1082
      OnExecute = ModeSelRectActionExecute
      OnUpdate = ModeSelRectActionUpdate
    end
    object ActualSizeAction: TAction
      Caption = #1048#1089#1093#1086#1076#1085#1099#1081' '#1088#1072#1079#1084#1077#1088
      OnExecute = ActualSizeActionExecute
      OnUpdate = ActualSizeActionUpdate
    end
    object BestFitAction: TAction
      Caption = #1042#1087#1080#1089#1072#1090#1100' '#1074' '#1086#1082#1085#1086
      OnExecute = BestFitActionExecute
      OnUpdate = BestFitActionUpdate
    end
    object AboutAction: TAction
      Caption = #1054' '#1087#1088#1086#1075#1088#1072#1084#1084#1077'...'
      OnExecute = AboutActionExecute
    end
  end
  object PopupMenu1: TPopupMenu
    Left = 96
    Top = 24
    object N1: TMenuItem
      Action = OpneImageAction
    end
    object N4: TMenuItem
      Caption = '-'
    end
    object N5: TMenuItem
      Action = ModePaneAction
    end
    object N6: TMenuItem
      Action = ModeSelRectAction
    end
    object N7: TMenuItem
      Caption = '-'
    end
    object N8: TMenuItem
      Action = ActualSizeAction
    end
    object N9: TMenuItem
      Action = BestFitAction
    end
    object N2: TMenuItem
      Caption = '-'
    end
    object N10: TMenuItem
      Action = AboutAction
    end
    object N3: TMenuItem
      Action = CloseAction
    end
  end
  object OpenDialog1: TOpenDialog
    Filter = 
      'Jpeg images|*.jpeg;*.jpg|AWP images|*.awp|PNG images|*.png|TIFF ' +
      'images|*.tif;*.tiff|BMP images|*.bmp'
    Left = 144
    Top = 24
  end
end