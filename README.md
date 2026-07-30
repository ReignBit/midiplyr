# midiplyr

I originally made a very basic midi player in C++, using my own midi file reader
from scratch.
It was very rudimentary and only supported the minimal needed to get some songs
playing, after leaving it for a while I have now decided to try and build upon
this old project - mainly rewriting it into C and also adding a graphical front
end to it.

## midi-lib
This is the backend, containing the midi file parsing, loading, and synthesizer
stuff.

I would like for this to be 100% my own code, with no external libraries, but we
will see how that goes. I may need some help when it comes to the actual
synthesizing of sound since soundwave math is completely out of my ballpark.


## midi-app
This is the frontend, using SDL2 to show a graphical representation of the track.
I have a vague idea of what I want it to look like - mainly like a retro piano
roll kind of style with the option to change voice banks / synth sounds.