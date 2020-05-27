#! /usr/bin/env bash

INPUT_SVG="Export.svg"


extract_svg_by_id()
{
    component_id="$1"
    input_file="$INPUT_SVG"
    output_file="$2.svg"

    printf "  exporting \"%s\" ...\n" "$output_file"

    inkscape --export-filename="$output_file" \
             --export-id="$component_id" \
             --export-id-only \
             --export-area-page \
             --export-plain-svg --export-text-to-path --vacuum-defs \
             "$input_file"
}


printf "\n"
inkscape --query-all Export.svg | grep -Eo '^export-[a-z:._-]+'
printf "\n"

extract_svg_by_id "export-background-background" "background/background"

extract_svg_by_id "export-button-curve-linear-on" "button/curve_linear_on"
extract_svg_by_id "export-button-curve-linear-over" "button/curve_linear_over"
extract_svg_by_id "export-button-curve-linear-off" "button/curve_linear_off"

printf "\n"
