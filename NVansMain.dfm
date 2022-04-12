object Main: TMain
  Left = 0
  Top = 0
  Caption = 'Main'
  ClientHeight = 503
  ClientWidth = 884
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -16
  Font.Name = 'Segoe UI'
  Font.Style = []
  OldCreateOrder = False
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 21
  object Splitter: TSplitter
    Left = 0
    Top = 232
    Width = 884
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
    Top = 479
    Width = 884
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
    ExplicitTop = 280
    ExplicitWidth = 635
  end
  object sgServer: TStringGrid
    Left = 0
    Top = 72
    Width = 884
    Height = 160
    Align = alTop
    ColCount = 2
    DefaultDrawing = False
    RowCount = 2
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goTabs, goFixedColClick]
    TabOrder = 1
    OnDrawCell = sgServerDrawCell
    ExplicitTop = 58
    ExplicitWidth = 880
  end
  object sgLocal: TStringGrid
    Left = 0
    Top = 240
    Width = 884
    Height = 239
    Align = alClient
    ColCount = 2
    DefaultDrawing = False
    RowCount = 2
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goTabs, goFixedColClick]
    TabOrder = 2
    OnDrawCell = sgLocalDrawCell
    ExplicitLeft = 24
    ExplicitTop = 248
    ExplicitWidth = 737
    ExplicitHeight = 97
  end
  object PanelServer: TPanel
    Left = 0
    Top = 0
    Width = 884
    Height = 72
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 0
    DesignSize = (
      884
      72)
    object btnClose: TButton
      Left = 768
      Top = 16
      Width = 100
      Height = 40
      Anchors = [akTop, akRight]
      Caption = #1047#1072#1082#1088#1099#1090#1100
      TabOrder = 2
      OnClick = btnCloseClick
    end
    object eRWNum: TLabeledEdit
      Left = 120
      Top = 21
      Width = 124
      Height = 29
      EditLabel.Width = 102
      EditLabel.Height = 21
      EditLabel.Caption = #1053#1086#1084#1077#1088' '#1087#1086#1077#1079#1076#1072
      EditLabel.WordWrap = True
      LabelPosition = lpLeft
      LabelSpacing = 8
      TabOrder = 0
      OnKeyDown = eRWNumKeyDown
    end
    object btnServerLoad: TButton
      Left = 258
      Top = 16
      Width = 101
      Height = 40
      Caption = #1047#1072#1075#1088#1091#1079#1080#1090#1100
      TabOrder = 1
      OnClick = btnServerLoadClick
    end
  end
  object ApplicationEvents: TApplicationEvents
    OnException = ApplicationEventsException
    Left = 704
    Top = 168
  end
  object ADOConnection1: TADOConnection
    CommandTimeout = 3
    ConnectionTimeout = 3
    Left = 632
    Top = 320
  end
  object ADOQuery1: TADOQuery
    CommandTimeout = 3
    Parameters = <>
    Left = 784
    Top = 352
  end
end
