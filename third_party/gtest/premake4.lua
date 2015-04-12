  project "gtest"
    defines { "GTEST_HAS_PTHREAD=0" } -- otherwise we must link with pthread below
    language "C++"
    kind "StaticLib"

    includedirs { gtest_root }
    files { gtest_root .. "/src/gtest-all.cc", gtest_root .. "/src/gtest_main.cc" }