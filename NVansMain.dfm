object Main: TMain
  Left = 0
  Top = 0
  Caption = 'Main'
  ClientHeight = 368
  ClientWidth = 712
  Color = clBtnFace
  Constraints.MinHeight = 407
  Constraints.MinWidth = 728
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'Segoe UI'
  Font.Style = []
  OldCreateOrder = False
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 17
  object Splitter: TSplitter
    Left = 0
    Top = 148
    Width = 712
    Height = 8
    Cursor = crSizeNS
    Align = alTop
    AutoSnap = False
    MinSize = 100
    ExplicitTop = 158
    ExplicitWidth = 880
  end
  object StatusBar: TStatusBar
    Left = 0
    Top = 344
    Width = 712
    Height = 24
    Panels = <
      item
        Width = 200
      end
      item
        Width = 50
      end>
    ParentFont = True
    SimplePanel = True
    UseSystemFont = False
    ExplicitWidth = 704
  end
  object sgServer: TStringGrid
    Left = 0
    Top = 48
    Width = 712
    Height = 100
    Align = alTop
    ColCount = 2
    DefaultColWidth = 32
    DefaultDrawing = False
    RowCount = 2
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goTabs, goFixedColClick]
    TabOrder = 1
    OnDrawCell = sgServerDrawCell
    OnFixedCellClick = sgServerFixedCellClick
    OnKeyDown = sgServerKeyDown
    OnSelectCell = sgServerSelectCell
    ExplicitWidth = 704
  end
  object sgLocal: TStringGrid
    Left = 0
    Top = 196
    Width = 712
    Height = 100
    Align = alClient
    ColCount = 2
    DefaultColWidth = 32
    DefaultDrawing = False
    RowCount = 2
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goTabs, goFixedColClick]
    TabOrder = 3
    OnDblClick = sgLocalDblClick
    OnDrawCell = sgLocalDrawCell
    OnFixedCellClick = sgServerFixedCellClick
    OnKeyDown = sgServerKeyDown
    OnSelectCell = sgLocalSelectCell
    ExplicitWidth = 704
  end
  object PanelServer: TPanel
    Left = 0
    Top = 0
    Width = 712
    Height = 48
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 0
    ExplicitWidth = 704
    DesignSize = (
      712
      48)
    object btnClose: TButton
      Left = 624
      Top = 8
      Width = 80
      Height = 32
      Anchors = [akTop, akRight]
      Caption = #1047#1072#1082#1088#1099#1090#1100
      TabOrder = 5
      OnClick = btnCloseClick
      ExplicitLeft = 616
    end
    object eRWNum: TLabeledEdit
      Left = 112
      Top = 12
      Width = 128
      Height = 25
      EditLabel.Width = 87
      EditLabel.Height = 17
      EditLabel.Caption = #1053#1086#1084#1077#1088' '#1087#1086#1077#1079#1076#1072
      EditLabel.WordWrap = True
      LabelPosition = lpLeft
      LabelSpacing = 8
      TabOrder = 0
      OnKeyDown = eRWNumKeyDown
    end
    object btnServerLoad: TButton
      Left = 248
      Top = 8
      Width = 80
      Height = 32
      Caption = #1047#1072#1075#1088#1091#1079#1080#1090#1100
      TabOrder = 1
      OnClick = btnServerLoadClick
    end
    object btnOptions: TButton
      Left = 528
      Top = 8
      Width = 80
      Height = 32
      Anchors = [akTop, akRight]
      Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080
      TabOrder = 4
      OnClick = btnOptionsClick
      ExplicitLeft = 520
    end
    object btnServerTrains: TButton
      Left = 336
      Top = 8
      Width = 80
      Height = 32
      Caption = #1057#1087#1080#1089#1086#1082
      TabOrder = 2
      OnClick = btnServerTrainsClick
    end
    object btnAbout: TButton
      Left = 424
      Top = 8
      Width = 96
      Height = 32
      Anchors = [akTop, akRight]
      Caption = #1054' '#1087#1088#1086#1075#1088#1072#1084#1084#1077
      TabOrder = 3
      OnClick = btnAboutClick
      ExplicitLeft = 416
    end
  end
  object PanelLocal: TPanel
    Left = 0
    Top = 296
    Width = 712
    Height = 48
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 4
    ExplicitWidth = 704
    object btnLocalLoad: TButton
      Left = 8
      Top = 8
      Width = 80
      Height = 32
      Caption = #1054#1073#1085#1086#1074#1080#1090#1100
      TabOrder = 0
      OnClick = btnLocalLoadClick
    end
    object btnLocalSave: TButton
      Left = 96
      Top = 8
      Width = 80
      Height = 32
      Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
      Enabled = False
      TabOrder = 1
      OnClick = btnLocalSaveClick
    end
    object btnLocalTrains: TButton
      Left = 192
      Top = 8
      Width = 80
      Height = 32
      Caption = #1055#1086#1077#1079#1076#1072
      TabOrder = 2
      OnClick = btnLocalTrainsClick
    end
  end
  object PanelCommon: TPanel
    Left = 0
    Top = 156
    Width = 712
    Height = 40
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 2
    ExplicitWidth = 704
    object btnSaveVanProps: TButton
      Left = 304
      Top = 0
      Width = 112
      Height = 32
      Caption = #1061#1072#1088#1072#1082#1090#1077#1088#1080#1089#1090#1080#1082#1080
      Enabled = False
      TabOrder = 2
      OnClick = btnSaveVanPropsClick
    end
    object btnCopyDataAll: TButton
      Left = 8
      Top = 0
      Width = 112
      Height = 32
      Caption = #1050#1086#1087#1080#1088#1086#1074#1072#1090#1100' '#1074#1089#1105
      Enabled = False
      TabOrder = 0
      OnClick = btnCopyDataAllClick
    end
    object btnCopyDataMass: TButton
      Tag = 1
      Left = 128
      Top = 0
      Width = 160
      Height = 32
      Caption = #1050#1086#1087#1080#1088#1086#1074#1072#1090#1100' '#1090#1086#1083#1100#1082#1086' '#1074#1077#1089
      Enabled = False
      TabOrder = 1
      OnClick = btnCopyDataAllClick
    end
  end
  object ApplicationEvents: TApplicationEvents
    OnException = ApplicationEventsException
    Left = 600
    Top = 80
  end
  object TimerResize: TTimer
    Enabled = False
    Interval = 100
    OnTimer = TimerResizeTimer
    Left = 504
    Top = 80
  end
end
