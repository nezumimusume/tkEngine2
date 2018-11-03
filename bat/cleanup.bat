@echo on
@rem クリーンアップ
@setlocal
@set CURRENT_DIR=%~dp0
pushd ..\

call .\Sample\cleanup.bat

popd

@echo クリーンアップ終了

