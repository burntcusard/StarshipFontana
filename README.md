# Starship Fontana #

Yes, it's still called that until I get around to renaming EVERYTHING. This README will be updated severely as well.

## Installation ##

After installing the correct libraries, I have been using this method to install/run on my PC:
Compile:
g++ -g -c src/*.cpp -I /usr/include/sigc++-2.0 -I /usr/lib/x86_64-linux-gnu/sigc++-2.0/include/ -std=c++11
Link:
g++ -o StarshipFontana *.o -lSDL -lSDL_image
Run:
./StarshipFontana

I assume on a correctly setup Fedora machine, you'll only have to use g++ -g -c src/*.cpp -std=c++11 to compile.

## Credits ##
The placeholder sprites (not the "P" and the coin) that are still left in come directly from 
[SpriteLib](http://www.widgetworx.com/widgetworx/portfolio/spritelib.html) and are used
under the terms of the [CPL 1.0](http://opensource.org/licenses/cpl1.0.php).


The event system is based on [Libsigc++](http://libsigc.sourceforge.net/)
and is used under the [LGPL](http://www.gnu.org/copyleft/lgpl.html).
