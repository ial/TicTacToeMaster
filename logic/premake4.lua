
--{{ language "C++"
         --kind "StaticLib"
         --flags { "Optimize" }
         --includedirs { ".", "/usr/local/include", "/where/is/your/lua" }
         --files { "src/**" }
--}}


--solution "TicTacMaster"
cur_proj_dir = "logic"

project "Logic"
    objdir ( obj_dir_root .. "/logic" )

    language ("c++")

    kind ("StaticLib")

    buildoptions {"-std=c++11"}
    
    includedirs { os.getenv("BOOST_ROOT") }

    files {
      project().basedir .. "/*.cpp",
      cur_proj_dir .. "/*.h"
    }

