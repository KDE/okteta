# PACKAGING RECOMMENDATIONS

You might want to create 4 basic sub-packages from a build of the Okteta
sources with all features enabled, besides the devel-packages.


## 1. Okteta libraries for QWidget-based hex editing widgets

LibOktetaCore and LibOktetaGui provide classes to have hex editing/viewing
QWidgets in Qt-based applications, similar to what is possible with QTextEdit
& QTextDocument.
Supplemented with a Qt Designer plugin for the Okteta widgets.

Proposed package description: "Hex editor/viewer QWidgets libraries"


## 2. Kasten libraries

Kasten is a WIP higher-level framework for composable document-centric
applications. LibKastenCore, LibKastenGui & LibKastenControllers are the
generic libraries. Due to its development state its still part of Okteta
sources and not used in other published software AFAIK. Except for KDevelop,
whose hex editor plugin is done using the Okteta Kasten libraries and
therefore also Kasten libraries.

So for now it makes sense to simply bundle both the generic Kasten libraries
with the Okteta specific Kasten libraries, LibOktetaKastenCore,
LibOktetaKastenGui, & LibOktetaKastenControllers.

All the data files for the Structures tool belong also to this, as well as the
s-m-i extension file.

Known public user of these libs:
* KDevelop for the hex editor plugin

Proposed package description: "High-level hex editor/viewer framework libraries"

Needs: 1.)


## 3. Okteta app

The actual hex editor program itself.

Proposed package description: "Editor for the raw data of files"
(please fix ancient package descriptions "editor for binary files" as this a bit wrongly limiting)

Needs: 1.) 2.)
(does _not_ use 4,)


## 4. KParts plugin

The oktetapart is a plugin for the KParts system. While KParts usage having
declined in Qt5/KF5 era, at least Krusader makes explicit use of the plugin,
other KParts software can get it when requesting a viewer for the MIME/media
type application/octet-stream.

Proposed package description: "Hex editing component for KParts"

Needs: 1.) 2.)
