mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))


default : Default/background/background.svg


Default/background/background.svg : Default/Default.svg
	@cd ${mkfile_dir} ; ./inkscape2skin.py "Default/Default.svg" "Default/"


clean :
	@rm -f Default/background/*.svg
	@rm -f Default/button/*.svg
	@rm -f Default/label/*.svg
	@rm -f Default/meter/*.svg
	@rm -f Default/slider/*.svg
