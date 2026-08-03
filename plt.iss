[Setup]
AppName=Plt
AppVerName=BTNRH PLT 5.16
AppPublisher=Boys Town Nationial Research Hospital
AppPublisherURL=http://www.boystownhospital.org
AppSupportURL=http://audres.org/rc/plt/
AppUpdatesURL=http://audres.org/rc/plt/
DefaultDirName={autopf}\BTNRH\PLT
DefaultGroupName=BTNRH
ChangesAssociations=Yes

[Tasks]
Name: "desktopicon"; Description: "Create a &desktop icon"; GroupDescription: "Additional icons:"; MinVersion: 4,4

[Files]
Source: "VS18\Release\wplt.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "VS18\wplt.chm"; DestDir: "{app}"; Flags: ignoreversion skipifsourcedoesntexist
Source: "demo\*"; DestDir: "{app}"; Flags: ignoreversion skipifsourcedoesntexist

[Icons]
Name: "{group}\Plt"; Filename: "{app}\wplt.exe"
Name: "{userdesktop}\Plt"; Filename: "{app}\wplt.exe"; MinVersion: 4,4; Tasks: desktopicon

[Registry]
Root: HKCR; Subkey: ".plt"; ValueType: string; ValueName: ""; ValueData: "BTNRHPltFile"; Flags: uninsdeletevalue
Root: HKCR; Subkey: "BTNRHPltFile"; ValueType: string; ValueName: ""; ValueData: "BTNRH Plt File"; Flags: uninsdeletekey
Root: HKCR; Subkey: "BTNRHPltFile\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\WPLT.EXE,0"
Root: HKCR; Subkey: "BTNRHPltFile\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\WPLT.EXE"" ""%1"""


[Run]
Filename: "{app}\wplt.exe"; Description: "Launch Plt?"; Flags: nowait postinstall skipifsilent

