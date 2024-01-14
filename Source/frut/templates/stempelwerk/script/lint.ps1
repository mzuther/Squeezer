Write-Output ""

. "$env:APPDATA\Python\Scripts\poetry.exe" run python -m flake8 --config=".\.flake8" src/

If (-Not $?)
{
    exit
}
