# Builds the API documentation with Doxygen and the doxygen-awesome-css theme: cmake --build <dir> --target docs
find_package(Doxygen REQUIRED OPTIONAL_COMPONENTS dot)

include(FetchContent)
FetchContent_Declare(doxygen-awesome-css
        GIT_REPOSITORY https://github.com/jothepro/doxygen-awesome-css.git
        GIT_TAG v2.3.4)
FetchContent_MakeAvailable(doxygen-awesome-css)
set(THEME_DIR "${doxygen-awesome-css_SOURCE_DIR}")

set(DOXYGEN_PROJECT_NAME "Numeric Utility")
set(DOXYGEN_PROJECT_NUMBER "${PROJECT_VERSION}")
set(DOXYGEN_PROJECT_BRIEF "${PROJECT_DESCRIPTION}")
set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${PROJECT_SOURCE_DIR}/README.md")
set(DOXYGEN_FILE_PATTERNS "*.hpp" "*.md")
set(DOXYGEN_EXTRACT_ALL YES)
set(DOXYGEN_EXTRACT_PRIVATE NO)
set(DOXYGEN_JAVADOC_AUTOBRIEF YES)
set(DOXYGEN_MARKDOWN_SUPPORT YES)
# The comments write math as LaTeX, e.g. $d(n)$.
set(DOXYGEN_USE_MATHJAX YES)
set(DOXYGEN_MATHJAX_VERSION MathJax_3)
set(DOXYGEN_STRIP_FROM_PATH "${PROJECT_SOURCE_DIR}/include")
set(DOXYGEN_STRIP_FROM_INC_PATH "${PROJECT_SOURCE_DIR}/include")
set(DOXYGEN_SOURCE_BROWSER YES)
set(DOXYGEN_ALPHABETICAL_INDEX NO)
# The export macro is defined only in a generated header; give Doxygen its value.
set(DOXYGEN_ENABLE_PREPROCESSING YES)
set(DOXYGEN_MACRO_EXPANSION YES)
set(DOXYGEN_EXPAND_ONLY_PREDEF YES)
set(DOXYGEN_PREDEFINED "NUMERIC_UTILITY_API=")
set(DOXYGEN_WARN_IF_UNDOCUMENTED NO)
set(DOXYGEN_WARN_AS_ERROR NO)

# doxygen-awesome-css settings.
set(DOXYGEN_GENERATE_TREEVIEW YES)
set(DOXYGEN_DISABLE_INDEX NO)
set(DOXYGEN_FULL_SIDEBAR NO)
set(DOXYGEN_HTML_COLORSTYLE LIGHT)
set(DOXYGEN_HTML_EXTRA_STYLESHEET "${THEME_DIR}/doxygen-awesome.css"
        "${THEME_DIR}/doxygen-awesome-sidebar-only.css")
set(DOXYGEN_HTML_EXTRA_FILES "${THEME_DIR}/doxygen-awesome-darkmode-toggle.js"
        "${THEME_DIR}/doxygen-awesome-fragment-copy-button.js"
        "${THEME_DIR}/doxygen-awesome-paragraph-link.js"
        "${THEME_DIR}/doxygen-awesome-interactive-toc.js")
set(DOXYGEN_HTML_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/header.html")
if(TARGET Doxygen::dot)
    set(DOXYGEN_HAVE_DOT YES)
    set(DOXYGEN_DOT_IMAGE_FORMAT svg)
    set(DOXYGEN_DOT_TRANSPARENT YES)
    set(DOXYGEN_INTERACTIVE_SVG YES)
endif()

doxygen_add_docs(docs
        "${PROJECT_SOURCE_DIR}/README.md"
        "${PROJECT_SOURCE_DIR}/include/numeric_utility"
        COMMENT "Generating API documentation")
