# Squeezer

Flexible general-purpose audio compressor with a touch of citrus

![Screenshot](./skins/screenshot.png)

## About

*I wrote this compressor to learn all I could about audio compression.
It took me several months to gather all the specs and papers I needed
and convert them into code.*

**Squeezer** is different from all the compressors I know: its knobs
are stepped to fine-tuned *preset* values, but can be changed to
*continuous* values by clicking a small red light located next to
them.  This allows you to find nice settings fast while not preventing
to optimise these settings.

**Squeezer** also adapts easily to many tastes and use cases:

- feed-forward & feed-back design
- linear, logarithmic \& smooth release stage
- optical & FET detectors
- peak & RMS sensing
- hard, medium & soft knee
- flexible side-chain & parallel compression

**Squeezer** has given me a deep insight into compression.  But above
that, it has quickly became my go-to generic compressor!

*I'm currently in the process of finally releasing a binary.  Please
be patient for a little longer...*

<!--
## Documentation

For documentation and further information, please see the directory
[doc][], especially the [manual][].
-->

## Code of conduct

Please read the [code of conduct][COC] before asking for help, filing
bug reports or contributing to this project.  Thanks!

## License

Copyright (c) 2013-2020 [Martin Zuther][]

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

Thank you for using free software!


[Martin Zuther]:  http://www.mzuther.de/
[COC]:            https://github.com/mzuther/Squeezer/tree/master/CODE_OF_CONDUCT.markdown
[doc]:            https://github.com/mzuther/Squeezer/tree/master/doc/
[manual]:         https://github.com/mzuther/Squeezer/raw/master/doc/kmeter.pdf
