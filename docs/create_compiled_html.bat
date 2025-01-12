@echo off
setlocal
cd %~dp0
:: Download from https://wkhtmltopdf.org/downloads.html
set "PATH=%PATH%;C:\Program Files\wkhtmltopdf\bin;"
wkhtmltopdf ATRC_C-CPP.html ATRC_C-CPP.pdf
endlocal