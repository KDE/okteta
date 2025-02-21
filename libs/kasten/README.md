Kasten framework
================
Made within the KDE community.

Copyright: 2007-2025 Friedrich W. H. Kossebau <kossebau@kde.org>

Author/Maintainer: Friedrich W. H. Kossebau <kossebau@kde.org>


Description
-----------
The Kasten framework is another approach for a component-oriented system.
It uses a recursive MVC architecture, emphasizes synchronisation of models, and
lot's more.

It's a work in progress, there are a lot of undocumented temporary solutions
to have the basic system work at all. Do not try to guess the planned
architecture by the existing classes and their methods. There are a lot of
unsketched ideas which will be tried and developed by the time.

For other, currently more advanced (in code ;) systems look for
Sublime in kdevplatform, Kate's doc/view system, or the one used by Calligra.
Once the Kasten framework is in a state where discussions make sense I will let
the world know. :)

As the API of these classes is still in active development there is no guarantee
on the binary or source compatibility between 0.x releases. If there is a change
the namespace postfix will be increased by 1, so different versions of the libraries
could be used even in the same process without symbol conflicts, if e.g. plugins
use different versions.


Installing
----------
Installs the libraries and the public headers, incl. CamelCase variants.
