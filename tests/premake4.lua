
--{{ language "C++"
         --kind "StaticLib"
         --flags { "Optimize" }
         --includedirs { ".", "/usr/local/include", "/where/is/your/lua" }
         --files { "src/**" }
--}}


--solution "TicTacMaster"
cur_proj_dir = "tests"

project "Tests"
    objdir ( obj_dir_root .. "/tests" )

    language ("c++")

    kind ("ConsoleApp")
    --kind("WindowedApp")
    buildoptions { "-std=c++11" }
    
    includedirs { gtest_root .. "/include", base_dir, os.getenv("BOOST_ROOT") }
    --libdirs { os.getenv("GTEST_DIR") .. "" }

    links { "gtest" }

    files {
      project().basedir .. "/*.cpp", project().basedir .. "/*.h",
      solution().basedir .. "/logic/*.cpp", solution().basedir .. "/logic/*.h",
    }

