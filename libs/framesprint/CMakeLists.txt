add_library(framesprint STATIC)
set_target_properties(framesprint PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
target_link_libraries(framesprint
    PUBLIC
        Qt6::Gui
    PRIVATE
        KF6::CoreAddons
        Qt6::PrintSupport
)

okteta_library_sources(framesprint
    PRIVATE
        PrintInfo
        AbstractFrameRenderer
        FramesToPaperPrinter
        HeaderFooterFrameRenderer
)
