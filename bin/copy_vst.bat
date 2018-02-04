@echo off

@setlocal
@set rsync_path=E:\Documents\System\Tools\rsync\bin
@set rsync_cmd=%rsync_path%\rsync.exe --archive

@set destination=/cygdrive/d/Vst/32bit/radix/
@echo %destination%
@%rsync_cmd% --delete "squeezer"                    "%destination%"
@%rsync_cmd%          "Squeezer (Mono).dll"         "%destination%"
@%rsync_cmd%          "Squeezer (Stereo).dll"       "%destination%"

@set destination=/cygdrive/d/Vst/Categories/Dynamics/Compressor/
@echo %destination%
@%rsync_cmd% --delete "squeezer"                    "%destination%"
@%rsync_cmd%          "Squeezer (Mono).dll"         "%destination%"
@%rsync_cmd%          "Squeezer (Stereo).dll"       "%destination%"

@set destination=/cygdrive/d/Plugins/VST2 (32 bit)/radix/
@echo %destination%
@%rsync_cmd% --delete "squeezer"                    "%destination%"
@%rsync_cmd%          "Squeezer (Mono).dll"         "%destination%"
@%rsync_cmd%          "Squeezer (Stereo).dll"       "%destination%"

@set destination=/cygdrive/d/Plugins/VST3 (32 bit)/radix/
@echo %destination%
@%rsync_cmd% --delete "squeezer"                    "%destination%"
@%rsync_cmd%          "Squeezer (Mono).vst3"        "%destination%"
@%rsync_cmd%          "Squeezer (Stereo).vst3"      "%destination%"

@set destination=/cygdrive/d/Plugins/VST2 (64 bit)/radix/
@echo %destination%
@%rsync_cmd% --delete "squeezer"                    "%destination%"
@%rsync_cmd%          "Squeezer (Mono x64).dll"     "%destination%"
@%rsync_cmd%          "Squeezer (Stereo x64).dll"   "%destination%"

@set destination=/cygdrive/d/Plugins/VST3 (64 bit)/radix/
@echo %destination%
@%rsync_cmd% --delete "squeezer"                    "%destination%"
@%rsync_cmd%          "Squeezer (Mono x64).vst3"    "%destination%"
@%rsync_cmd%          "Squeezer (Stereo x64).vst3"  "%destination%"

@echo.
@pause
