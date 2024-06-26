object frmOptions: TfrmOptions
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080
  ClientHeight = 354
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
    Height = 306
    Align = alClient
    BevelOuter = bvLowered
    TabOrder = 0
    object PageControl: TPageControl
      Left = 1
      Top = 1
      Width = 574
      Height = 304
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
          Top = 8
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
        object cboxSQLToLog: TCheckBox
          Left = 358
          Top = 56
          Width = 200
          Height = 17
          Caption = #1047#1072#1087#1080#1089#1100' '#1079#1072#1087#1088#1086#1089#1086#1074' '#1082' '#1041#1044' '#1074' '#1083#1086#1075
          TabOrder = 2
        end
        object rgScaleType: TRadioGroup
          Left = 8
          Top = 96
          Width = 344
          Height = 168
          Caption = #1058#1080#1087' '#1074#1077#1089#1086#1074
          Items.Strings = (
            #1054#1090#1082#1083#1102#1095#1077#1085#1086
            #1040#1074#1080#1090#1077#1082': '#1044#1080#1085#1072#1084#1080#1082#1072
            #1040#1074#1080#1090#1077#1082': '#1057#1090#1072#1090#1080#1082#1072
            #1055#1091#1083#1100#1090' '#1088#1091#1095#1085#1086#1075#1086' '#1074#1074#1086#1076#1072)
          TabOrder = 1
        end
        object cboxUseAutoReplace: TCheckBox
          Left = 358
          Top = 16
          Width = 200
          Height = 17
          Caption = #1048#1089#1087#1086#1083#1100#1079#1086#1074#1072#1090#1100' '#1072#1074#1090#1086#1079#1072#1084#1077#1085#1091
          TabOrder = 3
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
          Top = 122
          Width = 53
          Height = 17
          Caption = #1044#1088#1072#1081#1074#1077#1088
          FocusControl = cboxOracleDriver
        end
        object eLocalHost: TLabeledEdit
          Left = 8
          Top = 32
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
          Top = 88
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
          Top = 88
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
          Top = 144
          Width = 240
          Height = 25
          TabOrder = 3
        end
        object btnLocalCheck: TButton
          Left = 8
          Top = 184
          Width = 80
          Height = 32
          Caption = #1055#1088#1086#1074#1077#1088#1082#1072
          TabOrder = 4
          OnClick = btnOracleCheckClick
        end
      end
      object tsOracleDatabase: TTabSheet
        Caption = #1057#1077#1088#1074#1077#1088' Oracle'
        ImageIndex = 1
        ExplicitLeft = 0
        ExplicitTop = 0
        ExplicitWidth = 0
        ExplicitHeight = 0
        object lblOracleLDriver: TLabel
          Left = 8
          Top = 122
          Width = 53
          Height = 17
          Caption = #1044#1088#1072#1081#1074#1077#1088
          FocusControl = cboxOracleDriver
        end
        object eOracleHost: TLabeledEdit
          Left = 8
          Top = 32
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
          Top = 32
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
          Top = 88
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
          Top = 88
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
          Top = 144
          Width = 240
          Height = 25
          TabOrder = 4
        end
        object btnOracleCheck: TButton
          Left = 8
          Top = 184
          Width = 80
          Height = 32
          Caption = #1055#1088#1086#1074#1077#1088#1082#1072
          TabOrder = 5
          OnClick = btnOracleCheckClick
        end
      end
      object tsIsvsDatabase: TTabSheet
        Caption = #1048#1057#1042#1057
        ImageIndex = 4
        ExplicitLeft = -156
        ExplicitTop = -1
        ExplicitWidth = 0
        ExplicitHeight = 0
        object lblIsvsDriver: TLabel
          Left = 8
          Top = 122
          Width = 53
          Height = 17
          Caption = #1044#1088#1072#1081#1074#1077#1088
          FocusControl = cboxOracleDriver
        end
        object eIsvsHost: TLabeledEdit
          Left = 8
          Top = 32
          Width = 240
          Height = 25
          EditLabel.Width = 142
          EditLabel.Height = 17
          EditLabel.Caption = #1040#1076#1088#1077#1089' '#1080#1083#1080' '#1080#1084#1103' '#1089#1077#1088#1074#1077#1088#1072
          LabelSpacing = 4
          TabOrder = 0
        end
        object eIsvsUser: TLabeledEdit
          Left = 8
          Top = 88
          Width = 160
          Height = 25
          EditLabel.Width = 36
          EditLabel.Height = 17
          EditLabel.Caption = #1051#1086#1075#1080#1085
          LabelSpacing = 4
          TabOrder = 1
        end
        object eIsvsPass: TLabeledEdit
          Left = 176
          Top = 88
          Width = 160
          Height = 25
          EditLabel.Width = 46
          EditLabel.Height = 17
          EditLabel.Caption = #1055#1072#1088#1086#1083#1100
          LabelSpacing = 4
          PasswordChar = '#'
          TabOrder = 2
        end
        object cboxIsvsDriver: TComboBox
          Left = 8
          Top = 144
          Width = 240
          Height = 25
          TabOrder = 3
        end
        object btnIsvsCheck: TButton
          Left = 8
          Top = 184
          Width = 80
          Height = 32
          Caption = #1055#1088#1086#1074#1077#1088#1082#1072
          TabOrder = 4
          OnClick = btnOracleCheckClick
        end
      end
      object tsWME: TTabSheet
        Caption = #1055#1091#1083#1100#1090' '#1088#1091#1095#1085#1086#1075#1086' '#1074#1074#1086#1076#1072
        ImageIndex = 3
        ExplicitLeft = 0
        ExplicitTop = 0
        ExplicitWidth = 0
        ExplicitHeight = 0
        object eWMEProgramPath: TLabeledEdit
          Left = 8
          Top = 32
          Width = 512
          Height = 25
          EditLabel.Width = 169
          EditLabel.Height = 17
          EditLabel.Caption = #1055#1091#1090#1100' '#1082' '#1080#1089#1087#1086#1083#1085#1103#1077#1084#1086#1084#1091' '#1092#1072#1081#1083#1091
          LabelSpacing = 4
          TabOrder = 0
        end
        object eWMEProgramParams: TLabeledEdit
          Left = 8
          Top = 88
          Width = 240
          Height = 25
          EditLabel.Width = 69
          EditLabel.Height = 17
          EditLabel.Caption = #1055#1072#1088#1072#1084#1077#1090#1088#1099
          LabelSpacing = 4
          TabOrder = 2
        end
        object btnWMEProgramPath: TButton
          Left = 528
          Top = 31
          Width = 32
          Height = 27
          Caption = '...'
          TabOrder = 1
          OnClick = btnWMEProgramPathClick
        end
      end
    end
  end
  object PanelButtons: TPanel
    Left = 0
    Top = 306
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
  object OpenDialog: TOpenDialog
    DefaultExt = 'exe'
    Filter = #1055#1088#1086#1075#1088#1072#1084#1084#1099'|*.exe|'#1042#1089#1077' '#1092#1072#1081#1083#1099'|*.*'
    Options = [ofHideReadOnly, ofPathMustExist, ofFileMustExist, ofEnableSizing, ofDontAddToRecent]
    Left = 309
    Top = 141
  end
end
