# Squeezer

Flexible general-purpose audio compressor with a touch of citrus

![Screenshot](./skins/screenshot.png)

## About

I wrote this compressor to learn all I could about audio compression.
It took me several months to gather all the specs and papers I needed
and convert this vast amount of information into code.

What makes **Squeezer** different from the compressors I know is the
stepping of all knobs to preset values (it took a loooong time to
fine-tune them).  On clicking a small red light, however, the knobs
become continuous.  That allows you to find good-sounding settings
very fast while not preventing fine-tuning.

**Squeezer** is also very flexible.  Its internal layout can be
changed in many ways: feed-forward / feed-back, linear / S-curve /
log, optical / transistor, RMS / almost peak, three different knees.

By writing and fine-tuning this compressor I have gained deep insight
into audio compressors and really learned how to use them musically.
But not only that, **Squeezer** has quickly become my go-to generic
compressor.

I have not yet released a binary, because I expected **Squeezer**'s
internals to still change and have not yet found the time to write a
manual.  But if you want, you can compile the plug-in yourself.  It's
quite easy.

<!--
## Documentation

For documentation and further information, please see the directory
[doc][], especially the [manual][].
-->

## Code of conduct

Please read the [code of conduct][COC] before asking for help, filing
bug reports or contributing to this project.  Thanks!

## License

Copyright (c) 2013-2019 [Martin Zuther][]

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
