Okteta project
==============
Made within the KDE community.

Copyright: 2003-2023 Friedrich W. H. Kossebau <kossebau@kde.org>

Author/Maintainer: Friedrich W. H. Kossebau <kossebau@kde.org>


Overview
--------
This repository holds the files of the Okteta project. This project is about
viewing and editing of data on the byte level.

It is divided into three parts, the first two are for developing and the last
one for users:
* basic libraries offering Qt-based classes for viewing/editing bytearrays
* Okteta elements for the Kasten framework
* program Okteta and Okteta KPart

The basic libraries, liboktetacore and liboktetagui, contain the classic simple
Qt/KF widgets/classes others want to reuse in their code by linking to these
libraries. Additionally there is a Qt Designer plugin for the two Okteta widgets
from liboktetagui. This part is spread over the subdirs [core/](core/), [gui/](gui/), and
[designer/](designer/).

The Okteta elements for the Kasten framework are specific Okteta classes for
building programs and plugins with the Kasten framework. The elements are
divided into the libraries liboktetakastencore, liboktetakastengui, and
liboktetakastencontroller, all found in the subdir [kasten/](kasten/).
The foundation of the framework Kasten itself is currently found within the
Okteta sources, in the subdirectory [libs/kasten/](libs/kasten/).

The end-user products are the stand-alone program also named Okteta and a KPart
useable e.g. for Konqueror. These are in the subdirs [program/](program/)and [parts/](parts/).


What to find in which subdirectory
----------------------------------
[core/](core/):
  core library for Qt-based bytearray viewer/editor widgets, liboktetacore

[gui/](gui/):
  gui library for QWidget-based bytearray viewer/editor widgets, liboktetagui

[designer/](designer/):
  plugin for Qt Designer to add support for widgets from the Okteta gui library

[kasten/](kasten/):
  Okteta elements for the Kasten framework (currently found in libs/kasten)

[parts/](parts/):
  collection of plugins based on the Okteta libraries

[parts/kpart/](parts/kpart/):
  plugin of the type KPart::ReadWritePart, with BrowserExtension

[program/](program/):
  a standalone hex editor program, based on the Kasten framework

[libs/](libs/):
  neutral utility libs and frameworks, useful outside Okteta, e.g. Kasten
