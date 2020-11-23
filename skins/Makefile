mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))

.PHONY: default clean

default : Resources/Skin.xml Resources/background/background.svg Source/skin.h


Resources/Skin.xml : Original/Skin.xml
	@echo
	@echo "Copying \"Skin.xml\"..."
	@mkdir -p ${mkfile_dir}/Resources
	@cp ${mkfile_dir}/Original/Skin.xml ${mkfile_dir}/Resources/Skin.xml
	@echo "Done."
	@echo


Resources/background/background.svg : Original/Skin.svg
	@cd ${mkfile_dir} ; ./inkscape2skin.py Original/Skin.svg Resources


Source/skin.h : Resources/background/background.svg Resources/Skin.xml
	@mkdir -p ${mkfile_dir}/Source
	@cd ${mkfile_dir} ; ./convert_resources.sh


clean :
	@rm -rf ${mkfile_dir}/Resources
	@rm -rf ${mkfile_dir}/Source
