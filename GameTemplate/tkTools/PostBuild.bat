rem Visual studioのビルドのポストビルドステップのバッチファイル。
pushd ..\\Game\\
rem ddsファイルをResourceの下にコピー。
xcopy /Y /D *.dds .\Resource\*

popd
