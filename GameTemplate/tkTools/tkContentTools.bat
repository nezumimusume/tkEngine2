echo on
setlocal
set TK_TOOLS=%~dp0
set PATH=Python\Python36
pushd %TK_TOOLS%..\Game\
%TK_TOOLS%\Python\Python36\python %TK_TOOLS%\tkContentTools.py %1 %2
popd
