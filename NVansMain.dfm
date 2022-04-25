object Main: TMain
  Left = 0
  Top = 0
  Caption = 'Main'
  ClientHeight = 368
  ClientWidth = 704
  Color = clBtnFace
  Constraints.MinHeight = 407
  Constraints.MinWidth = 720
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
  PixelsPerInch = 96
  TextHeight = 17
  object Splitter: TSplitter
    Left = 0
    Top = 192
    Width = 704
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
    Width = 704
    Height = 24
    Panels = <
      item
        Width = 200
      end
      item
        Width = 50
      end>
    ParentFont = True
    UseSystemFont = False
  end
  object sgServer: TStringGrid
    Left = 0
    Top = 48
    Width = 704
    Height = 144
    Align = alTop
    ColCount = 2
    DefaultDrawing = False
    RowCount = 2
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goTabs, goFixedColClick]
    TabOrder = 1
    OnDrawCell = sgServerDrawCell
    OnFixedCellClick = sgServerFixedCellClick
    OnKeyDown = sgServerKeyDown
  end
  object sgLocal: TStringGrid
    Left = 0
    Top = 200
    Width = 704
    Height = 144
    Align = alClient
    ColCount = 2
    DefaultDrawing = False
    RowCount = 2
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goTabs, goFixedColClick]
    TabOrder = 2
    OnDrawCell = sgLocalDrawCell
  end
  object PanelServer: TPanel
    Left = 0
    Top = 0
    Width = 704
    Height = 48
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 0
    DesignSize = (
      704
      48)
    object btnClose: TButton
      Left = 616
      Top = 8
      Width = 80
      Height = 32
      Anchors = [akTop, akRight]
      Caption = #1047#1072#1082#1088#1099#1090#1100
      TabOrder = 3
      OnClick = btnCloseClick
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
      Left = 520
      Top = 8
      Width = 80
      Height = 32
      Anchors = [akTop, akRight]
      Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080
      TabOrder = 2
      OnClick = btnOptionsClick
    end
    object btnServerList: TButton
      Left = 336
      Top = 8
      Width = 80
      Height = 32
      Caption = #1057#1087#1080#1089#1086#1082
      TabOrder = 4
      OnClick = btnServerListClick
    end
  end
  object ApplicationEvents: TApplicationEvents
    OnException = ApplicationEventsException
    Left = 608
    Top = 104
  end
end
