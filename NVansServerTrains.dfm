object frmServerTrains: TfrmServerTrains
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSizeToolWin
  Caption = #1053#1072#1090#1091#1088#1085#1099#1077' '#1083#1080#1089#1090#1099': '#1089#1087#1080#1089#1086#1082' '#1087#1086#1077#1079#1076#1086#1074
  ClientHeight = 280
  ClientWidth = 448
  Color = clBtnFace
  Constraints.MinHeight = 319
  Constraints.MinWidth = 464
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'Segoe UI'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 17
  object StatusBar: TStatusBar
    Left = 0
    Top = 256
    Width = 448
    Height = 24
    Panels = <
      item
        Width = 160
      end
      item
        Width = 50
      end>
    ParentFont = True
    UseSystemFont = False
    ExplicitWidth = 352
  end
  object sgList: TStringGrid
    Left = 0
    Top = 48
    Width = 264
    Height = 208
    Align = alClient
    ColCount = 2
    DefaultDrawing = False
    RowCount = 2
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goFixedColClick]
    TabOrder = 1
    OnDblClick = sgListDblClick
    OnDrawCell = sgListDrawCell
    OnKeyDown = sgListKeyDown
    OnSelectCell = sgListSelectCell
    ExplicitWidth = 200
  end
  object PanelTop: TPanel
    Left = 0
    Top = 0
    Width = 448
    Height = 48
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 0
    ExplicitWidth = 352
    DesignSize = (
      448
      48)
    object btnUpdate: TButton
      Left = 8
      Top = 8
      Width = 80
      Height = 32
      Caption = #1054#1073#1085#1086#1074#1080#1090#1100
      TabOrder = 0
      OnClick = btnUpdateClick
    end
    object btnClose: TButton
      Left = 360
      Top = 8
      Width = 80
      Height = 32
      Anchors = [akTop, akRight]
      Caption = #1047#1072#1082#1088#1099#1090#1100
      TabOrder = 2
      OnClick = btnCloseClick
      ExplicitLeft = 264
    end
    object btnClear: TButton
      Left = 96
      Top = 8
      Width = 80
      Height = 32
      Caption = #1057#1073#1088#1086#1089
      TabOrder = 1
      OnClick = btnClearClick
    end
  end
  object PanelFilter: TPanel
    Left = 264
    Top = 48
    Width = 184
    Height = 208
    Align = alRight
    BevelOuter = bvNone
    TabOrder = 2
    ExplicitLeft = 168
    object lblFilterDate: TLabel
      Left = 8
      Top = 0
      Width = 83
      Height = 17
      Caption = #1044#1072#1090#1072' '#1086#1090#1075#1088#1091#1079#1082#1080
      FocusControl = pckrFilterDate
    end
    object pckrFilterDate: TDateTimePicker
      Left = 8
      Top = 22
      Width = 168
      Height = 25
      Date = 29674.439248831020000000
      Time = 29674.439248831020000000
      TabOrder = 0
      OnKeyDown = pckrFilterDateKeyDown
    end
    object eFilterVanNum: TLabeledEdit
      Left = 8
      Top = 78
      Width = 168
      Height = 25
      EditLabel.Width = 86
      EditLabel.Height = 17
      EditLabel.Caption = #1053#1086#1084#1077#1088' '#1074#1072#1075#1086#1085#1072
      MaxLength = 8
      TabOrder = 1
      OnChange = eFilterVanNumChange
      OnKeyDown = pckrFilterDateKeyDown
    end
    object eFilterInvoiceNum_1: TLabeledEdit
      Left = 8
      Top = 134
      Width = 168
      Height = 25
      EditLabel.Width = 86
      EditLabel.Height = 17
      EditLabel.Caption = #1042#1072#1075#1086#1085#1085#1099#1081' '#1083#1080#1089#1090
      TabOrder = 2
      Visible = False
      OnChange = eFilterVanNumChange
      OnKeyDown = pckrFilterDateKeyDown
    end
  end
end
