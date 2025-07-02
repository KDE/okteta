add_library(OktetaFramesPrint STATIC)
set_target_properties(OktetaFramesPrint PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
target_link_libraries(OktetaFramesPrint
    PUBLIC
        Qt5::Gui
    PRIVATE
        KF5::CoreAddons
        Qt5::PrintSupport
)

okteta_library_sources(OktetaFramesPrint
    PRIVATE
        PrintInfo
        AbstractFrameRenderer
        FramesToPaperPrinter
        HeaderFooterFrameRenderer
)
