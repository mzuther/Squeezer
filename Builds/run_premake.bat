@echo off

@rem ---------------------------------------------------------------------------
@rem
@rem  Squeezer
@rem  ========
@rem  Flexible general-purpose audio compressor with a touch of lemon.
@rem
@rem  Copyright (c) 2013-2015 Martin Zuther (http://www.mzuther.de/)
@rem
@rem  This program is free software: you can redistribute it and/or modify
@rem  it under the terms of the GNU General Public License as published by
@rem  the Free Software Foundation, either version 3 of the License, or
@rem  (at your option) any later version.
@rem
@rem  This program is distributed in the hope that it will be useful,
@rem  but WITHOUT ANY WARRANTY; without even the implied warranty of
@rem  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
@rem  GNU General Public License for more details.
@rem
@rem  You should have received a copy of the GNU General Public License
@rem  along with this program.  If not, see <http://www.gnu.org/licenses/>.
@rem
@rem  Thank you for using free software!
@rem
@rem ---------------------------------------------------------------------------

@set python_path="%PROGRAMFILES%\Python33"
@set visual_toolkit="Windows7.1SDK"


@echo.
@premake4 --file=visual_studio.lua --os=windows vs2010
@"%python_path%\python.exe" visual_studio_fix.py "%visual_toolkit%"

@echo.
@premake4 --cc=gcc --os=linux gmake

@echo.
@pause
