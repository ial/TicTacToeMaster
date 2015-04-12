solution "TicTacMaster"

obj_dir_root = solution().basedir .. "/build/obj"
bin_dir_root = solution().basedir .. "/build/bin"
gtest_root = solution().basedir .. "/third_party/gtest"
base_dir = solution().basedir

--{{ language "C++"
         --kind "StaticLib"
         --flags { "Optimize" }
         --includedirs { ".", "/usr/local/include", "/where/is/your/lua" }
         --files { "src/**" }
--}}

--premake.tools.gcc.ldflags.flags._Symbols = nil


  objdir ( obj_dir_root )
  targetdir ( bin_dir_root )
  location ( "./build" )
  includedirs { gtest_root .. "/include" }

  configurations { "Debug", "Profile", "Release" }
  --platforms { "Unix", "MacOSX", "Windows" }

  configuration { "gmake" }
    buildoptions { "-g3" } --"-std=c++11" 

  configuration "Debug"
     defines { "DEBUG" }
     flags { "Symbols" }

  configuration "Release"
     defines { "NDEBUG" }
     flags { "Optimize" }

  -- gtest_main static library
  include "third_party/gtest"


  include "logic"
  include "tests"
  include "gui_mac"


  if _ACTION == "clean" then
    os.rmdir("build")
  end

