rem Visual studioのビルドのプレビルドステップのバッチファイル。
pushd ..\\Game\\

rem Resourceフォルダの作成。
mkDir Resource > NUL 2>&1
if errorlevel 1 goto NEXT0
:NEXT0

pushd Resource

rem modelDataフォルダを作成する。
mkDir .\\modelData > NUL 2>&1
if errorlevel 1 goto NEXT1
:NEXT1

rem spriteフォルダを作成する。
mkDir .\\sprite > NUL 2>&1
if errorlevel 1 goto NEXT2
:NEXT2
popd
popd
exit 0
