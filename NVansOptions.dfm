object frmOptions: TfrmOptions
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080
  ClientHeight = 360
  ClientWidth = 576
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'Segoe UI'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 17
  object PanelMain: TPanel
    Left = 0
    Top = 0
    Width = 576
    Height = 312
    Align = alClient
    BevelOuter = bvLowered
    TabOrder = 0
    object PageControl: TPageControl
      Left = 1
      Top = 1
      Width = 574
      Height = 310
      ActivePage = tsProgram
      Align = alClient
      TabOrder = 0
      object tsProgram: TTabSheet
        Caption = #1055#1088#1086#1075#1088#1072#1084#1084#1072
        ImageIndex = 3
        ExplicitLeft = 0
        ExplicitTop = 0
        ExplicitWidth = 0
        ExplicitHeight = 0
        object gbOptionsPass: TGroupBox
          Left = 8
          Top = 16
          Width = 344
          Height = 84
          Caption = #1042#1093#1086#1076' '#1074' '#1085#1072#1089#1090#1088#1086#1081#1082#1080
          TabOrder = 0
          object eOptionsPass: TLabeledEdit
            Left = 8
            Top = 48
            Width = 160
            Height = 25
            EditLabel.Width = 46
            EditLabel.Height = 17
            EditLabel.Caption = #1055#1072#1088#1086#1083#1100
            LabelSpacing = 4
            PasswordChar = '#'
            TabOrder = 0
          end
          object eOptionsPass2: TLabeledEdit
            Left = 176
            Top = 48
            Width = 160
            Height = 25
            EditLabel.Width = 95
            EditLabel.Height = 17
            EditLabel.Caption = #1055#1086#1076#1090#1074#1077#1088#1078#1076#1077#1085#1080#1077
            LabelSpacing = 4
            PasswordChar = '#'
            TabOrder = 1
          end
        end
        object cboxUseLocal: TCheckBox
          Left = 8
          Top = 112
          Width = 288
          Height = 17
          Caption = #1048#1089#1087#1086#1083#1100#1079#1086#1074#1072#1090#1100' '#1083#1086#1082#1072#1083#1100#1085#1091#1102' '#1073#1072#1079#1091' '#1076#1072#1085#1085#1099#1093
          TabOrder = 1
        end
      end
      object tsLocalDatabase: TTabSheet
        Caption = #1051#1086#1082#1072#1083#1100#1085#1072#1103' '#1073#1072#1079#1072' '#1076#1072#1085#1085#1099#1093
        ExplicitLeft = 0
        ExplicitTop = 0
        ExplicitWidth = 0
        ExplicitHeight = 0
        object lblLocalDriver: TLabel
          Left = 8
          Top = 136
          Width = 53
          Height = 17
          Caption = #1044#1088#1072#1081#1074#1077#1088
          FocusControl = cboxOracleDriver
        end
        object eLocalHost: TLabeledEdit
          Left = 8
          Top = 40
          Width = 240
          Height = 25
          EditLabel.Width = 142
          EditLabel.Height = 17
          EditLabel.Caption = #1040#1076#1088#1077#1089' '#1080#1083#1080' '#1080#1084#1103' '#1089#1077#1088#1074#1077#1088#1072
          LabelSpacing = 4
          TabOrder = 0
        end
        object eLocalUser: TLabeledEdit
          Left = 8
          Top = 96
          Width = 160
          Height = 25
          EditLabel.Width = 36
          EditLabel.Height = 17
          EditLabel.Caption = #1051#1086#1075#1080#1085
          LabelSpacing = 4
          TabOrder = 1
        end
        object eLocalPass: TLabeledEdit
          Left = 176
          Top = 96
          Width = 160
          Height = 25
          EditLabel.Width = 46
          EditLabel.Height = 17
          EditLabel.Caption = #1055#1072#1088#1086#1083#1100
          LabelSpacing = 4
          PasswordChar = '#'
          TabOrder = 2
        end
        object cboxLocalDriver: TComboBox
          Left = 8
          Top = 160
          Width = 240
          Height = 25
          TabOrder = 3
        end
        object btnLocalCheck: TButton
          Left = 8
          Top = 200
          Width = 80
          Height = 32
          Caption = #1055#1088#1086#1074#1077#1088#1082#1072
          TabOrder = 4
          OnClick = btnOracleCheckClick
        end
      end
      object tsServerOracleDatabase: TTabSheet
        Caption = #1057#1077#1088#1074#1077#1088' Oracle'
        ImageIndex = 1
        ExplicitLeft = 0
        ExplicitTop = 0
        ExplicitWidth = 0
        ExplicitHeight = 0
        object lblOracleLDriver: TLabel
          Left = 8
          Top = 136
          Width = 53
          Height = 17
          Caption = #1044#1088#1072#1081#1074#1077#1088
          FocusControl = cboxOracleDriver
        end
        object eOracleHost: TLabeledEdit
          Left = 8
          Top = 40
          Width = 240
          Height = 25
          EditLabel.Width = 142
          EditLabel.Height = 17
          EditLabel.Caption = #1040#1076#1088#1077#1089' '#1080#1083#1080' '#1080#1084#1103' '#1089#1077#1088#1074#1077#1088#1072
          LabelSpacing = 4
          TabOrder = 0
        end
        object eOracleService: TLabeledEdit
          Left = 256
          Top = 40
          Width = 240
          Height = 25
          EditLabel.Width = 78
          EditLabel.Height = 17
          EditLabel.Caption = #1048#1084#1103' '#1089#1077#1088#1074#1080#1089#1072
          LabelSpacing = 4
          TabOrder = 1
        end
        object eOracleUser: TLabeledEdit
          Left = 8
          Top = 96
          Width = 160
          Height = 25
          EditLabel.Width = 36
          EditLabel.Height = 17
          EditLabel.Caption = #1051#1086#1075#1080#1085
          LabelSpacing = 4
          TabOrder = 2
        end
        object eOraclePass: TLabeledEdit
          Left = 176
          Top = 96
          Width = 160
          Height = 25
          EditLabel.Width = 46
          EditLabel.Height = 17
          EditLabel.Caption = #1055#1072#1088#1086#1083#1100
          LabelSpacing = 4
          PasswordChar = '#'
          TabOrder = 3
        end
        object cboxOracleDriver: TComboBox
          Left = 8
          Top = 160
          Width = 240
          Height = 25
          TabOrder = 4
        end
        object btnOracleCheck: TButton
          Left = 8
          Top = 200
          Width = 80
          Height = 32
          Caption = #1055#1088#1086#1074#1077#1088#1082#1072
          TabOrder = 5
          OnClick = btnOracleCheckClick
        end
      end
    end
  end
  object PanelButtons: TPanel
    Left = 0
    Top = 312
    Width = 576
    Height = 48
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    DesignSize = (
      576
      48)
    object btnOK: TButton
      Left = 400
      Top = 8
      Width = 80
      Height = 32
      Anchors = [akTop, akRight]
      Caption = 'OK'
      Default = True
      TabOrder = 0
      OnClick = btnOKClick
    end
    object btnCancel: TButton
      Left = 488
      Top = 8
      Width = 80
      Height = 32
      Anchors = [akTop, akRight]
      Cancel = True
      Caption = #1054#1090#1084#1077#1085#1072
      ModalResult = 2
      TabOrder = 1
    end
  end
end
