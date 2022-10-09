include(FetchContent)

function(fetch_dependency name)
  FetchContent_GetProperties(${name})
  if(NOT ${name}_POPULATED)
    message(STATUS "Fetching and configuring ${name}...")
    list(APPEND CMAKE_MESSAGE_INDENT "  ")

    FetchContent_Populate(${name})
    add_subdirectory(
      ${${name}_SOURCE_DIR}
      ${${name}_BINARY_DIR}
      # we have to set EXCLUDE_FROM_ALL so that dependent targets are not
      # included into the install trees.
      EXCLUDE_FROM_ALL)

    list(POP_BACK CMAKE_MESSAGE_INDENT)
  endif()
endfunction(fetch_dependency)

FetchContent_Declare(
  gp-sdk
  # beta version pulling from rank13's git
  GIT_REPOSITORY https://github.com/gp-rank13/gp-sdk-beta.git
  GIT_TAG a624ecf7c8f3c41b4624c9a7bcc4f3141418bd62 # v45

  # 
  GIT_REPOSITORY https://github.com/gigperformer/gp-sdk.git
  GIT_TAG 6c5432518ef42ea0870fb44597e9d1d3780e2f98 # v44
)
fetch_dependency(gp-sdk)

# don't need juice for this build
#FetchContent_Declare(
#  juce
#  GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
#  GIT_TAG 2f980209cc4091a4490bb1bafc5d530f16834e58 # v6.1.6
#)
#fetch_dependency(juce)