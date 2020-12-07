mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))

.PHONY: default clean

default :  frut_widgets_resources.h


frut_widgets_resources.h : Resources/*.svg
	@cd ${mkfile_dir} ; ../../convert_resources.py "Resources" "." "frut::widgets::resources"


clean :
	@rm -f "${mkfile_dir}/frut_widgets_resources.h"
	@rm -f "${mkfile_dir}/frut_widgets_resources.cpp"
