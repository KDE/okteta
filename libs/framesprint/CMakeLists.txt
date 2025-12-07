add_library(OktetaFramesPrint STATIC)
set_target_properties(OktetaFramesPrint PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
target_link_libraries(OktetaFramesPrint
    PUBLIC
        Qt6::Gui
    PRIVATE
        KF6::CoreAddons
        Qt6::PrintSupport
)

okteta_library_sources(OktetaFramesPrint
    PRIVATE
        PrintInfo
        AbstractFrameRenderer
        FramesToPaperPrinter
        HeaderFooterFrameRenderer
)
