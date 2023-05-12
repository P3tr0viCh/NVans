object frmSearch: TfrmSearch
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSizeToolWin
  Caption = #1055#1086#1080#1089#1082
  ClientHeight = 25
  ClientWidth = 144
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'Segoe UI'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  OnActivate = FormActivate
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnHide = FormHide
  DesignSize = (
    144
    25)
  PixelsPerInch = 96
  TextHeight = 17
  object eSearch: TEdit
    Left = 0
    Top = 0
    Width = 144
    Height = 25
    Anchors = [akLeft, akTop, akRight]
    AutoSelect = False
    AutoSize = False
    TabOrder = 0
    OnChange = eSearchChange
    OnKeyPress = eSearchKeyPress
  end
  object Timer: TTimer
    Enabled = False
    Interval = 444
    OnTimer = TimerTimer
    Left = 56
    Top = 8
  end
end
