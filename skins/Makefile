mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))

.PHONY: default clean

default : Resources/Skin.xml Resources/background/background.svg Source/skin.h


Resources/Skin.xml : Original/Skin.xml
	@echo
	@echo "Copying \"Skin.xml\"..."
	@mkdir -p Resources/
	@cp Original/Skin.xml Resources/Skin.xml
	@echo "Done."
	@echo


Resources/background/background.svg : Original/Skin.svg
	@cd ${mkfile_dir} ; ./inkscape2skin.py Original/Skin.svg Resources/


Source/skin.h : Resources/background/background.svg Resources/Skin.xml
	@mkdir -p Source/
	@cd ${mkfile_dir} ; ./convert_resources.sh


clean :
	@rm -f Resources/background/*.svg
	@rm -f Resources/button/*.svg
	@rm -f Resources/label/*.svg
	@rm -f Resources/meter/*.svg
	@rm -f Resources/slider/*.svg
