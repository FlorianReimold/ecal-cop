@echo off

pushd "%~dp0\..\.."

mkdir _build_default
cd _build_default

cmake .. -A x64
cmake --build . --parallel --config Release
ctest -C Release -V

popd
