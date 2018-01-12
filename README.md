##### QR2ANSI 
 
   Inspired by and adapted from: 

    https://gist.github.com/michaelschrijver/8566294

   That version is great, but I needed a version that used CP437 half blocks 
   in order to make the code fit more easily on an 80x24 terminal 

   Compile with: 

    gcc -o qr2 qr2.c -lqrencode

   You will need to pipe the output through iconv for now to get UTF-8 
   compatible output, eg.

   ./qr2 "foobarbaz.com" | iconv -f CP437 -t UTF-8

   Since the output of this program is in obsolete CP437 format. 

