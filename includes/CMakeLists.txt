set( oktetacore_CamelCase_HDRS
  Range
  NumberRange

  Address
  AddressRange
  Size
  Byte
  AbstractByteArrayModel
  Versionable
  Bookmarkable
  ChangesDescribable
  ChangeHistory

  ByteArrayModel
  #FileByteArrayModel
  PieceTableByteArrayModel

  Character
  ArrayChangeMetrics
  ByteArrayChange

  Bookmark
  BookmarksConstIterator

  WordByteArrayService
  ValueCodec
  CharCodec
)

set( oktetagui_CamelCase_HDRS
  LinePosition
  Line
  LinePositionRange
  LineRange

  AbstractColumnStylist
  AbstractColumnRenderer
  ColumnsView

  AbstractByteArrayColumnRenderer
  CharByteArrayColumnRenderer
  ValueByteArrayColumnRenderer
  ByteArrayRowColumnRenderer
  OffsetColumnRenderer
  BorderColumnRenderer

  AbstractByteArrayView
  ByteArrayColumnView

  OffsetFormat
  ByteArrayTableRanges
  ByteArrayRowView
  ByteArrayTableLayout
  WidgetColumnStylist
)


set( okteta_CamelCase_HDRS
  ${oktetacore_CamelCase_HDRS}
  ${oktetagui_CamelCase_HDRS}
)

install( FILES ${okteta_CamelCase_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/KDE/Okteta${OKTETALIBS_ABI_VERSION}
  COMPONENT Devel
)
