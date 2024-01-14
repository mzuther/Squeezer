Write-Output ""

# remove virtual environment
# (https://github.com/orgs/python-poetry/discussions/3690#discussioncomment-5293709)
. "$env:APPDATA\Python\Scripts\poetry.exe" env remove py
Write-Output ""
