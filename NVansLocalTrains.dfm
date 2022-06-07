object frmLocalTrains: TfrmLocalTrains
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSizeToolWin
  Caption = #1042#1077#1089#1099': c'#1087#1080#1089#1086#1082' '#1087#1086#1077#1079#1076#1086#1074
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
  object PanelTop: TPanel
    Left = 0
    Top = 0
    Width = 352
    Height = 48
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 0
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
    Width = 352
    Height = 136
    Align = alClient
    ColCount = 2
    DefaultDrawing = False
    FixedCols = 0
    RowCount = 2
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goTabs, goFixedColClick]
    TabOrder = 2
    OnDblClick = sgListDblClick
    OnDrawCell = sgListDrawCell
    OnSelectCell = sgListSelectCell
    ExplicitWidth = 200
  end
end
