cur_proj_dir = "gui_mac"

project "GuiMac"
    objdir ( obj_dir_root .. "/gui_mac" )

    language ("c++")

    kind ("ConsoleApp")
    --kind("WindowedApp")
    
    includedirs { gtest_root .. "/include", base_dir, os.getenv("BOOST_ROOT") }
    --libdirs { os.getenv("GTEST_DIR") .. "" }

    configuration { "gmake" }
        buildoptions { "-fobjc-arc" }

    links { "Logic",
        "Foundation.framework", "AppKit.framework", "Cocoa.framework" }

    files {
      project().basedir .. "/*.cpp",
      project().basedir .. "/*.h",
      project().basedir .. "/*.mm",
      project().basedir .. "/*.m",      
    }

    --dest_dir = bin_dir_root .. "/TicTacBoardCocoa/Release/TicTacBoardCocoa.app/Contents/MacOS"
    postbuildcommands {
        --"xcodebuild"
        --"xcodebuild -project '" .. project().basedir .. "/TicTacBoardCocoa/TicTacBoardCocoa.xcodeproj' OBJROOT='" .. obj_dir_root .. "/TicTacBoardCocoa' SYMROOT='" .. bin_dir_root .. "/TicTacBoardCocoa'",
        --"xcodebuild -project '" .. project().basedir .. "/TicTacBoardCocoa.xcodeproj' OBJROOT='" .. obj_dir_root .. "/TicTacBoardCocoa' SYMROOT='" .. bin_dir_root .. "/TicTacBoardCocoa'",
        --"xcodebuild -project " .. project().basedir
        --"mkdir -p " .. dest_dir,
        --"cp " .. bin_dir_root .. "/GuiMac " .. dest_dir .. "/TicTacBoardCocoa"
    }