Write-Output ""

. "$env:APPDATA\Python\Scripts\poetry.exe" run python -m pytest --ignore=".\.git" $args

If (-Not $?)
{
    exit
}

. ".\script\lint.ps1"
