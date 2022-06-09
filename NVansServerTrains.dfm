object frmServerTrains: TfrmServerTrains
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSizeToolWin
  Caption = #1053#1072#1090#1091#1088#1085#1099#1077' '#1083#1080#1089#1090#1099': '#1089#1087#1080#1089#1086#1082' '#1087#1086#1077#1079#1076#1086#1074
  ClientHeight = 208
  ClientWidth = 352
  Color = clBtnFace
  Constraints.MinHeight = 247
  Constraints.MinWidth = 368
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
    Top = 184
    Width = 352
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
  end
  object sgList: TStringGrid
    Left = 0
    Top = 48
    Width = 200
    Height = 136
    Align = alClient
    ColCount = 2
    DefaultDrawing = False
    RowCount = 2
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goTabs, goFixedColClick]
    TabOrder = 1
    OnDblClick = sgListDblClick
    OnDrawCell = sgListDrawCell
    OnKeyDown = sgListKeyDown
    OnSelectCell = sgListSelectCell
  end
  object PanelTop: TPanel
    Left = 0
    Top = 0
    Width = 352
    Height = 48
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 2
    DesignSize = (
      352
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
      Left = 264
      Top = 8
      Width = 80
      Height = 32
      Anchors = [akTop, akRight]
      Caption = #1047#1072#1082#1088#1099#1090#1100
      TabOrder = 1
      OnClick = btnCloseClick
    end
  end
  object PanelFilter: TPanel
    Left = 200
    Top = 48
    Width = 152
    Height = 136
    Align = alRight
    BevelOuter = bvNone
    TabOrder = 3
    DesignSize = (
      152
      136)
    object pckrFilterDate: TDateTimePicker
      Left = 8
      Top = 0
      Width = 136
      Height = 25
      Date = 44706.439248831020000000
      Time = 44706.439248831020000000
      TabOrder = 0
      OnKeyDown = pckrFilterDateKeyDown
    end
    object btnFilterClear: TButton
      Left = 64
      Top = 96
      Width = 80
      Height = 32
      Anchors = [akRight, akBottom]
      Caption = #1057#1073#1088#1086#1089
      TabOrder = 2
      OnClick = btnFilterClearClick
    end
    object eFilterVanNum: TLabeledEdit
      Left = 8
      Top = 56
      Width = 136
      Height = 25
      EditLabel.Width = 86
      EditLabel.Height = 17
      EditLabel.Caption = #1053#1086#1084#1077#1088' '#1074#1072#1075#1086#1085#1072
      TabOrder = 1
      OnChange = eFilterVanNumChange
      OnKeyDown = pckrFilterDateKeyDown
    end
  end
end
