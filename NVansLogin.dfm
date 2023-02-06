object frmLogin: TfrmLogin
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080
  ClientHeight = 112
  ClientWidth = 240
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'Segoe UI'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  PixelsPerInch = 96
  TextHeight = 17
  object btnOK: TButton
    Left = 64
    Top = 72
    Width = 80
    Height = 32
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 1
    OnClick = btnOKClick
  end
  object btnCancel: TButton
    Left = 152
    Top = 72
    Width = 80
    Height = 32
    Cancel = True
    Caption = #1054#1090#1084#1077#1085#1072
    ModalResult = 2
    TabOrder = 2
  end
  object ePass: TLabeledEdit
    Left = 8
    Top = 32
    Width = 224
    Height = 25
    EditLabel.Width = 46
    EditLabel.Height = 17
    EditLabel.Caption = #1055#1072#1088#1086#1083#1100
    LabelSpacing = 4
    PasswordChar = '#'
    TabOrder = 0
  end
end
