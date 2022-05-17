Okteta and the framework Kasten is designed in a very modular way, with a lot
of interfaces. So it should be easily extendable. While there isn't yet a real
plugin system, new plugins/modules can be simply added directly in the code for now.

Find below a list of places where Okteta can be extended:

Basic Okteta libraries
----------------------

`Okteta::AbstractByteArrayModel` - not yet documented

`Okteta::CharCodec`/`Okteta::ValueCodec` - not yet documented


Okteta Kasten elements
----------------------

Binary Filter - see [kasten/controllers/view/libbytearrayfilter/README_developers.md](kasten/controllers/view/libbytearrayfilter/README_developers.md)

_Checksum/Hashsum - see [kasten/controllers/view/libbytearraychecksum/README_developers.md](kasten/controllers/view/libbytearraychecksum/README_developers.md)

Encoder (for Export) - not yet documented


Kasten framework
----------------

`Kasten::AbstractModel`/`Kasten::AbstractDocument`/`Kasten::AbstractSynchronizer` - not yet documented

`Kasten::AbstractView` - not yet documented

`Kasten::AbstractXmlGuiController` - not yet documented

`Kasten::AbstractTool`/`Kasten::AbstractTool(Inline)View` - not yet documented

and some more...
