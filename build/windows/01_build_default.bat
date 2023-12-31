@echo off

pushd "%~dp0\..\.."

mkdir _build
cd _build

cmake .. -A x64
cmake --build . --config Release

popd
