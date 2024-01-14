Write-Output ""

# install poetry
(Invoke-WebRequest -Uri https://install.python-poetry.org -UseBasicParsing).Content | py -

Write-Output ""
Write-Output "------------------------------------------------------------------------"
Write-Output ""

# create virtual environment
. "$env:APPDATA\Python\Scripts\poetry.exe" env use py
. "$env:APPDATA\Python\Scripts\poetry.exe" env info

Write-Output ""
Write-Output "------------------------------------------------------------------------"
Write-Output ""

# install dependencies
. "$env:APPDATA\Python\Scripts\poetry.exe" install --with dev --sync
Write-Output ""
