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

# Import the SDK
include(FetchContent)
FetchContent_Declare(
  gp-sdk
  GIT_REPOSITORY https://github.com/gigperformer/gp-sdk.git
  # GIT_TAG a50f8e4a2ce3ce6a7599beac307804a62649eb33 # v44
  # GIT_TAG 3d4fa49c3d6f96851961893ce51896b4450e59ae # v46
  # GIT_TAG 23c6583ff0137a620a421f6378bb2f6fc761de61 # 50
  # GIT_TAG 383ca00e99972fbedd9e71ae4d342a658de273c1 # 51
  # GIT_TAG 897b3452b99f2d7fc02502d0c90d89cd54b51c7e
  # GIT_TAG 26bc1ed2568b94e1c95ca2348f03afcbb0d8fb16 # 4.7 release
  # GIT_TAG 756ecd249f1ff00f3d3a5955769408f74c9fa453 # SDK 60
  # GIT_TAG d131f0d31c8814d003c02dd6587b22933113e9dd
  GIT_TAG 46b462ab8f4a434ce73f58ac4138600f83925789 # sdk 62
  # GIT_REPOSITORY https://github.com/WidnerM/gp-sdk.git
  # GIT_TAG 1e62325f8ec5b36a94f3cfe0b5395f325e149479 # SDK 62
  # GIT_TAG 3f42e8fb304b491f272674c278ab5a4494f56703 # SDK 63
  # GIT_TAG 5028e6b3df74b651fa7f60bcc73f94ee7b539e02
  # GIT_REPOSITORY https://github.com/daveboulden/gp-sdk.git
  # GIT_TAG c0ef1e550bd8dbf84614a7c9b52cfaa05490bb3b
  # GIT_TAG eed42b2c52745c5a12189386930fdbe8e662b0b2

)
FetchContent_MakeAvailable(gp-sdk)

# Import Juce
FetchContent_Declare(
  juce
  GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
  # GIT_TAG 2f980209cc4091a4490bb1bafc5d530f16834e58 # v6.1.6
  GIT_TAG 29396c22c93392d6738e021b83196283d6e4d850 # v8.0.12
)
fetch_dependency(juce)
