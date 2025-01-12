@echo off
if not exist temp mkdir temp
set "PATH=%PATH%;C:\Program Files\wkhtmltopdf\bin;"
wkhtmltopdf ATRC_C-CPP.html ATRC_C-CPP.pdf
