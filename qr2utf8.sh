#!/bin/sh
./qr2 $1 | iconv -f CP437 -t UTF-8
