Okteta Gui library
==================
Part of the Okteta project, made within the KDE community.

Copyright: 2003-2023 Friedrich W. H. Kossebau <kossebau@kde.org>

Author/Maintainer: Friedrich W. H. Kossebau <kossebau@kde.org>


Description
-----------
The lib implements two general usable widgets named `Okteta::ByteArrayColumnView` and
`Okteta::ByteArrayRowView`. They interact with the abstract byte array interface
called `Okteta::AbstractByteArrayModel` as offered by the Okteta core library.

As the API of these classes is still in active development there is no guarantee
on the binary or source compatibility between 0.x releases. If there is a change
the namespace postfix will be increased by 1, so different versions of the lib
could be used even in the same process without symbol conflicts, if e.g. plugins
use different versions.


Installing
----------
Installs the lib and the public headers, incl. CamelCase variants.
