# Squeezer

*Flexible general-purpose audio compressor with a touch of citrus*

![Screenshot](./doc/include/images/squeezer.png)

## About

*I wrote this compressor to learn all I could about audio compression.
It took me several months to gather all the specs and papers I needed
and convert them into code.*

**Squeezer** is different from all the compressors I know: its knobs
are stepped to fine-tuned *preset* values, but can be changed to
*continuous* values by clicking a small orange light located next to
them.  This allows you to find nice settings fast while not preventing
optimisation of settings.

**Squeezer** adapts easily to many tastes and use cases:

- feed-forward & feed-back design
- linear, logarithmic & smooth release stage
- optical & FET detectors
- peak & RMS sensing
- hard, medium & soft knee
- flexible side-chain & parallel compression

**Squeezer** has given me a deep insight into compression.  But above
that, it has quickly become my go-to generic compressor!

*I hope that you have as much fun with this compressor as I had when I
coded it!*

## FAQ

### Squeezer crashes when the plug-in is loaded / opened

1. Squeezer requires a processor which supports the SSE2 instruction
   set (if you run at Windows 8 and above, it does).  On Windows, you
   might also have to install the [Visual C++ Redistributable for
   Visual Studio 2017][VC++ Redist].

2. Squeezer comes with a folder called `squeezer`.  This folder must be
   located in the same folder as the plug-in, so please copy it along
   and try again!

### Some hosts (FL Studio, Bitwig) do not assign VST2 channels correctly

- As an example, a stereo plug-in might only output a mono signal.

- VST2 does not support side-chains.  I have found a way around this
  limitation, but unfortunately some hosts will behave erratically.

- VST3 supports side-chains, so if your DAW supports it, use the VST3
  plug-ins instead.  As an alternative, I have created VST2 plug-ins
  without external side-chain inputs.  They are distinguished by the
  words `no side-chain` in their name and should fix
  channel-assignment problems.

### Where are the LV2 plug-ins?

- The last release with LV2 plug-ins was version **v2.5.3**.  You can
  still download them from the "Releases" page.

### The stand-alone version does not work

- Unfortunately, I cannot do anything about it as I did not code this
  part of the application.  The stand-alone works well for me - try
  using another sound card or deleting the settings file.

## Documentation

For documentation and further information, please see the [manual][]
and the directory [doc][].

## Code of conduct

Please read the [code of conduct][COC] before asking for help, filing
bug reports or contributing to this project.  Thanks!

## License

Copyright (c) 2013-2022 Martin Zuther

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

## VST2 plug-ins

Technically, my VST2 plug-ins are not free software.  However, I
chose to provide them for your convenience.  All other binaries really
are free software in the sense of the Free Software Foundation.

*VST is a trademark of Steinberg Media Technologies GmbH, registered
in Europe and other countries.*


[COC]:            https://github.com/mzuther/Squeezer/tree/master/CODE_OF_CONDUCT.markdown
[doc]:            https://github.com/mzuther/Squeezer/tree/master/doc/
[manual]:         https://github.com/mzuther/Squeezer/raw/master/doc/squeezer.pdf
[VC++ Redist]:    https://www.visualstudio.com/downloads/
