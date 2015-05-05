object mainForm: TmainForm
  Left = 197
  Top = 129
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
  OnClose = FormClose
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
    Left = 40
    Top = 144
  end
  object ActionList1: TActionList
    Left = 80
    Top = 144
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
      Visible = False
      OnExecute = ModePaneActionExecute
      OnUpdate = ModePaneActionUpdate
    end
    object ModeSelRectAction: TAction
      Caption = #1042#1099#1073#1088#1072#1090#1100' '#1087#1088#1103#1084#1086#1091#1075#1086#1083#1100#1085#1080#1082
      Visible = False
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
    object ViewTAnglfAction: TAction
      Caption = #1048#1089#1093#1086#1076#1085#1099#1081' '#1072#1085#1072#1075#1083#1080#1092' '
      OnExecute = ViewTAnglfActionExecute
      OnUpdate = ViewTAnglfActionUpdate
    end
    object ViewOAnglfAction: TAction
      Caption = #1054#1087#1090#1080#1084#1080#1079#1080#1088#1086#1074#1072#1085#1085#1099#1081' '#1072#1085#1072#1075#1083#1080#1092
      OnExecute = ViewOAnglfActionExecute
      OnUpdate = ViewOAnglfActionUpdate
    end
    object ViewGAnglfAction: TAction
      Caption = #1055#1086#1083#1091#1090#1086#1085#1086#1074#1099#1081' '#1072#1085#1072#1075#1083#1080#1092
      OnExecute = ViewGAnglfActionExecute
      OnUpdate = ViewGAnglfActionUpdate
    end
    object ViewCAnglfAction: TAction
      Caption = #1062#1074#1077#1090#1085#1086#1081' '#1072#1085#1072#1075#1083#1080#1092
      OnExecute = ViewCAnglfActionExecute
      OnUpdate = ViewCAnglfActionUpdate
    end
    object ViewSourceAction: TAction
      Caption = #1048#1089#1093#1086#1076#1085#1086#1077' '#1080#1079#1086#1073#1088#1072#1078#1077#1085#1080#1077
      OnExecute = ViewSourceActionExecute
      OnUpdate = ViewSourceActionUpdate
    end
    object ViewLeftAction: TAction
      Caption = #1044#1083#1103' '#1083#1077#1074#1086#1075#1086' '#1075#1083#1072#1079#1072
      OnExecute = ViewLeftActionExecute
      OnUpdate = ViewLeftActionUpdate
    end
    object ViewRightAction: TAction
      Caption = #1044#1083#1103' '#1087#1088#1072#1074#1086#1075#1086' '#1075#1083#1072#1079#1072
      OnExecute = ViewRightActionExecute
      OnUpdate = ViewRightActionUpdate
    end
  end
  object PopupMenu1: TPopupMenu
    Left = 120
    Top = 144
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
    object N11: TMenuItem
      Caption = '-'
    end
    object N12: TMenuItem
      Action = ViewSourceAction
    end
    object N13: TMenuItem
      Action = ViewLeftAction
    end
    object N14: TMenuItem
      Action = ViewRightAction
    end
    object N15: TMenuItem
      Caption = '-'
    end
    object N16: TMenuItem
      Action = ViewTAnglfAction
    end
    object N17: TMenuItem
      Action = ViewOAnglfAction
    end
    object N18: TMenuItem
      Action = ViewGAnglfAction
    end
    object N19: TMenuItem
      Action = ViewCAnglfAction
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
      'images|*.tif;*.tiff|BMP images|*.bmp|JPS  images|*.jps'
    Left = 168
    Top = 144
  end
end
