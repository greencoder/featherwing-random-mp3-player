# featherwing-random-mp3-player
A shuffling MP3 player using the Adafruit FeatherWing Music Maker

I have a project in mind that will play music files with no interface, just a power switch. I
used an (https://www.adafruit.com/product/3357)[Adafruit Music Maker FeatherWing] on a Feather Board to
implement the MP3 player portion.

This code will read all the MP3 files in the *root* directory of the SD card and continuously play them in random
order. It will skip over directories and hidden files. (like the `.Spotlight-V100` MacOS puts on the card)

Because of memory limitations, it's not possible to read all the names of the files into an array and sample from it.
Instead, I count the number of MP3 files on the SD card, then I choose a random number between `0` and `count` to
pick a song to play. There is no checking to see if the file has been recently played, that would be a nice feature
to add. (Pull requests gladly accepted!)

Limitations:
* The MP3 files must be in the root of the SD card, they cannot be in directories
* The MP3 filenames must be in 8.3 filename format (8 or less characters)
* The MP3 filename must end with a lowercase `.mp3` extension
* The songs are chosen randomly so it's possible to play the same song repeatedly
