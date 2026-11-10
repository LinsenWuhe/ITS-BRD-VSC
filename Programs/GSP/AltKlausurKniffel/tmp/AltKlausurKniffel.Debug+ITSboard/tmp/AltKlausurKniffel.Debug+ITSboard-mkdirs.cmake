# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/jannik/Studium lokal/GSP/ITS-BRD-VSC/Programs/AltKlausurKniffel/tmp/AltKlausurKniffel.Debug+ITSboard"
  "/Users/jannik/Studium lokal/GSP/ITS-BRD-VSC/Programs/AltKlausurKniffel/tmp/1"
  "/Users/jannik/Studium lokal/GSP/ITS-BRD-VSC/Programs/AltKlausurKniffel/tmp/AltKlausurKniffel.Debug+ITSboard"
  "/Users/jannik/Studium lokal/GSP/ITS-BRD-VSC/Programs/AltKlausurKniffel/tmp/AltKlausurKniffel.Debug+ITSboard/tmp"
  "/Users/jannik/Studium lokal/GSP/ITS-BRD-VSC/Programs/AltKlausurKniffel/tmp/AltKlausurKniffel.Debug+ITSboard/src/AltKlausurKniffel.Debug+ITSboard-stamp"
  "/Users/jannik/Studium lokal/GSP/ITS-BRD-VSC/Programs/AltKlausurKniffel/tmp/AltKlausurKniffel.Debug+ITSboard/src"
  "/Users/jannik/Studium lokal/GSP/ITS-BRD-VSC/Programs/AltKlausurKniffel/tmp/AltKlausurKniffel.Debug+ITSboard/src/AltKlausurKniffel.Debug+ITSboard-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/jannik/Studium lokal/GSP/ITS-BRD-VSC/Programs/AltKlausurKniffel/tmp/AltKlausurKniffel.Debug+ITSboard/src/AltKlausurKniffel.Debug+ITSboard-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/jannik/Studium lokal/GSP/ITS-BRD-VSC/Programs/AltKlausurKniffel/tmp/AltKlausurKniffel.Debug+ITSboard/src/AltKlausurKniffel.Debug+ITSboard-stamp${cfgdir}") # cfgdir has leading slash
endif()
