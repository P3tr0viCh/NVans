object Main: TMain
  Left = 0
  Top = 0
  Caption = 'Main'
  ClientHeight = 408
  ClientWidth = 1024
  Color = clBtnFace
  Constraints.MinHeight = 447
  Constraints.MinWidth = 1040
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'Segoe UI'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyDown = FormKeyDown
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 17
  object StatusBar: TStatusBar
    Left = 0
    Top = 384
    Width = 1024
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
  end
  object sgServer: TStringGrid
    Left = 0
    Top = 48
    Width = 1024
    Height = 120
    Align = alTop
    ColCount = 2
    DefaultColWidth = 32
    DefaultDrawing = False
    RowCount = 2
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goTabs, goFixedColClick]
    PopupMenu = PopupMenu
    TabOrder = 1
    OnDrawCell = sgServerDrawCell
    OnFixedCellClick = sgServerFixedCellClick
    OnKeyDown = sgServerKeyDown
    OnMouseDown = sgServerMouseDown
    OnMouseUp = sgServerMouseUp
    OnSelectCell = sgServerSelectCell
    ExplicitLeft = 24
  end
  object sgLocal: TStringGrid
    Left = 0
    Top = 216
    Width = 1024
    Height = 120
    Align = alClient
    ColCount = 2
    DefaultColWidth = 32
    DefaultDrawing = False
    RowCount = 2
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goTabs, goFixedColClick]
    PopupMenu = PopupMenu
    TabOrder = 3
    OnDblClick = sgLocalDblClick
    OnDrawCell = sgLocalDrawCell
    OnFixedCellClick = sgServerFixedCellClick
    OnSelectCell = sgLocalSelectCell
  end
  object PanelServer: TPanel
    Left = 0
    Top = 0
    Width = 1024
    Height = 48
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 0
    DesignSize = (
      1024
      48)
    object lblRWNum: TLabel
      Left = 8
      Top = 8
      Width = 45
      Height = 34
      Caption = #1053#1086#1084#1077#1088' '#1087#1086#1077#1079#1076#1072
      FocusControl = eRWNum
      WordWrap = True
    end
    object lblDateTime: TLabel
      Left = 160
      Top = 7
      Width = 51
      Height = 34
      Caption = #1044#1072#1090#1072' '#1086#1090#1075#1088#1091#1079#1082#1080
      FocusControl = eDateTime
      WordWrap = True
    end
    object btnClose: TButton
      Left = 936
      Top = 8
      Width = 80
      Height = 32
      Anchors = [akTop, akRight]
      Caption = #1047#1072#1082#1088#1099#1090#1100
      TabOrder = 9
      OnClick = btnCloseClick
    end
    object btnServerLoad: TButton
      Left = 336
      Top = 8
      Width = 80
      Height = 32
      Caption = #1047#1072#1075#1088#1091#1079#1080#1090#1100
      TabOrder = 2
      OnClick = btnServerLoadClick
    end
    object btnOptions: TButton
      Left = 840
      Top = 8
      Width = 80
      Height = 32
      Anchors = [akTop, akRight]
      Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080
      TabOrder = 8
      OnClick = btnOptionsClick
    end
    object btnServerTrains: TButton
      Left = 440
      Top = 8
      Width = 72
      Height = 32
      Caption = #1055#1086#1077#1079#1076#1072
      TabOrder = 4
      OnClick = btnServerTrainsClick
    end
    object btnAbout: TButton
      Left = 736
      Top = 8
      Width = 96
      Height = 32
      Anchors = [akTop, akRight]
      Caption = #1054' '#1087#1088#1086#1075#1088#1072#1084#1084#1077
      TabOrder = 7
      OnClick = btnAboutClick
    end
    object btnServerReverse: TButton
      Left = 520
      Top = 8
      Width = 72
      Height = 32
      Caption = #1056#1077#1074#1077#1088#1089
      Enabled = False
      TabOrder = 5
      OnClick = btnServerReverseClick
    end
    object eRWNum: TEdit
      Left = 56
      Top = 12
      Width = 96
      Height = 25
      TabOrder = 0
      OnChange = eRWNumChange
      OnKeyDown = eRWNumKeyDown
    end
    object eDateTime: TEdit
      Left = 216
      Top = 12
      Width = 112
      Height = 25
      Color = clBtnFace
      ReadOnly = True
      TabOrder = 1
      OnKeyDown = eRWNumKeyDown
    end
    object btnServerSaveToFile: TButton
      Left = 608
      Top = 8
      Width = 80
      Height = 32
      Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
      Enabled = False
      TabOrder = 6
      OnClick = btnServerSaveToFileClick
    end
    object btnServerLoadExt: TButton
      Left = 416
      Top = 8
      Width = 16
      Height = 32
      Caption = #9660
      TabOrder = 3
      OnClick = btnServerLoadExtClick
    end
  end
  object PanelLocal: TPanel
    Left = 0
    Top = 336
    Width = 1024
    Height = 48
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 4
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
    Top = 168
    Width = 1024
    Height = 48
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 2
    object btnSaveVanProps: TButton
      Left = 304
      Top = 8
      Width = 112
      Height = 32
      Caption = #1061#1072#1088#1072#1082#1090#1077#1088#1080#1089#1090#1080#1082#1080
      Enabled = False
      TabOrder = 2
      OnClick = btnSaveVanPropsClick
    end
    object btnCopyDataAll: TButton
      Left = 8
      Top = 8
      Width = 112
      Height = 32
      Caption = #1050#1086#1087#1080#1088#1086#1074#1072#1090#1100
      Enabled = False
      TabOrder = 0
      OnClick = btnCopyDataAllClick
    end
    object btnCopyDataMass: TButton
      Tag = 1
      Left = 128
      Top = 8
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
    Left = 584
    Top = 80
  end
  object TimerResize: TTimer
    Enabled = False
    Interval = 100
    OnTimer = TimerResizeTimer
    Left = 872
    Top = 80
  end
  object PopupMenu: TPopupMenu
    AutoHotkeys = maManual
    OnPopup = PopupMenuPopup
    Left = 344
    Top = 80
    object miCopy: TMenuItem
      Caption = #1050#1086#1087#1080#1088#1086#1074#1072#1090#1100
      ShortCut = 16451
      OnClick = miCopyClick
    end
    object miPasteVanNum: TMenuItem
      Caption = #1042#1089#1090#1072#1074#1080#1090#1100' '#1085#1086#1084#1077#1088#1072' '#1074#1072#1075#1086#1085#1086#1074
      ShortCut = 16470
      OnClick = miPasteVanNumClick
    end
    object miClear: TMenuItem
      Caption = #1059#1076#1072#1083#1080#1090#1100
      ShortCut = 46
      OnClick = miClearClick
    end
    object miSelectAll: TMenuItem
      Caption = #1042#1099#1076#1077#1083#1080#1090#1100' '#1074#1089#1105
      ShortCut = 16449
      OnClick = miSelectAllClick
    end
    object miVanToString: TMenuItem
      Caption = 'VanToString'
      Visible = False
      OnClick = miVanToStringClick
    end
  end
  object SaveDialog: TSaveDialog
    DefaultExt = 'csv'
    Filter = 'CSV ('#1088#1072#1079#1076#1077#1083#1080#1090#1077#1083#1080' - '#1079#1072#1087#1103#1090#1099#1077') |*.csv|'#1042#1089#1077' '#1092#1072#1081#1083#1099'|*.*'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofPathMustExist, ofEnableSizing]
    Left = 680
    Top = 80
  end
  object pmServerLoadExt: TPopupMenu
    Left = 440
    Top = 80
    object miServerLoadAsIs: TMenuItem
      Caption = #1047#1072#1075#1088#1091#1079#1080#1090#1100' '#1073#1077#1079' '#1072#1074#1090#1086#1079#1072#1084#1077#1085#1099
      OnClick = miServerLoadAsIsClick
    end
  end
end
