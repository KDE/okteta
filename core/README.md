Okteta Core library
===================
Part of the Okteta project, made within the KDE community.

Copyright: 2003-2023 Friedrich W. H. Kossebau <kossebau@kde.org>

Author/Maintainer: Friedrich W. H. Kossebau <kossebau@kde.org>


Description
-----------
The library offers an abstract byte array interface called `Okteta::AbstractByteArrayModel`.
This interface can be subclassed to offer access to different byte arrays
like paged gigabyte big files, video memory or whatever you can imagine
(hopefully). Some example byte array models are included.

As the API of these classes is still in active development there is no guarantee
on the binary or source compatibility between 0.x releases. If there is a change
the namespace postfix will be increased by 1, so different versions of the lib
could be used even in the same process without symbol conflicts, if e.g. plugins
use different versions.


Installing
----------
Installs the lib and the public headers, incl. CamelCase variants.