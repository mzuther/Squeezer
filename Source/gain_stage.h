/* ----------------------------------------------------------------------------

   Squeezer
   ========
   Flexible general-purpose audio compressor with a touch of lemon.

   Copyright (c) 2013-2015 Martin Zuther (http://www.mzuther.de/)

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

---------------------------------------------------------------------------- */

#ifndef __GAIN_STAGE_H__
#define __GAIN_STAGE_H__

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
/**
*/
class GainStage
{
public:
    virtual ~GainStage() {}

    virtual void reset(double dCurrentGainReduction) = 0;
    virtual double processGainReduction(double dGainReductionNew, double dGainReductionIdeal) = 0;
protected:
    GainStage(int nSampleRate) {}
};


#endif  // __GAIN_STAGE_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
