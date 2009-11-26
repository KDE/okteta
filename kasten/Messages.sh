#! /bin/sh
$EXTRACTRC `find . -name \*.rc -o -name \*.ui -o -name \*.kcfg` >> rc.cpp
$XGETTEXT `find . -type d -name template -prune -o -name \*.cpp -print` -o $podir/liboktetakasten.pot
