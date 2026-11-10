# roots.cmake
set(CMSIS_PACK_ROOT "/Users/jannik/.cache/arm/packs" CACHE PATH "CMSIS pack root")
cmake_path(ABSOLUTE_PATH CMSIS_PACK_ROOT NORMALIZE OUTPUT_VARIABLE CMSIS_PACK_ROOT)
set(CMSIS_COMPILER_ROOT "/Users/jannik/.vscode/extensions/arm.cmsis-csolution-1.68.0-darwin-arm64/tools/cmsis-toolbox/etc" CACHE PATH "CMSIS compiler root")
cmake_path(ABSOLUTE_PATH CMSIS_COMPILER_ROOT NORMALIZE OUTPUT_VARIABLE CMSIS_COMPILER_ROOT)
set(SOLUTION_ROOT "/Users/jannik/Studium lokal/GSP/ITS-BRD-VSC/Programs/AltklausurRust2" CACHE PATH "CMSIS solution root")
cmake_path(ABSOLUTE_PATH SOLUTION_ROOT NORMALIZE OUTPUT_VARIABLE SOLUTION_ROOT)
