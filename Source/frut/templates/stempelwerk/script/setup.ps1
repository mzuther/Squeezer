Write-Output ""
. "$env:APPDATA\Python\Scripts\poetry.exe" show

Write-Output ""
Write-Output "------------------------------------------------------------------------"
Write-Output ""

. "$env:APPDATA\Python\Scripts\poetry.exe" show --only dev
Write-Output ""
