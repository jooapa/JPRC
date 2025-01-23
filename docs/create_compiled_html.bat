@echo off
setlocal
cd %~dp0
:: Download from https://wkhtmltopdf.org/downloads.html
set "PATH=%PATH%;C:\Program Files\wkhtmltopdf\bin;"
wkhtmltopdf --load-error-handling ignore index.htm api_reference.htm installation_guide.htm styles.css ATRC_C-CPP.pdf

set "PATH=;C:\Program Files (x86)\HTML Help Workshop;%PATH%"

hhc ATRC_C-CPP.hhp
endlocal