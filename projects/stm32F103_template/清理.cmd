@echo off

rem 设置代码页为 UTF-8
chcp 65001 >nul

echo 开始清理工程
echo.

echo 删除文件夹DebugConfig

if exist "DebugConfig" (
    rem 删除 DebugConfig 文件夹及其所有内容
    rd /s /q "DebugConfig"
) 
echo ok
echo.

echo 删除文件夹Listings

if exist "Listings" (
    rem 删除 Listings 文件夹及其所有内容
    rd /s /q "Listings"
) 
echo ok
echo.

echo 删除文件夹Objects

if exist "Objects" (
    rem 删除 Objects 文件夹及其所有内容
    rd /s /q "Objects"
) 
echo ok
echo.

echo 删除当前文件夹下所有后缀为 .scvd 的文件

if exist "*.scvd" (
    
    del /q *.scvd
    
)

echo 删除当前文件夹下所有后缀为 *.uvguix.* 的文件

if exist "*.uvguix.*" (
    
    del /q *.uvguix.*
    
)

echo ok
echo.

rem pause