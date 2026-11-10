# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/jannik/Studium lokal/GSP/ITS-BRD-VSC/Programs/AltKlausurCDSS23/tmp/ITSboard/default/AltKlausurCDSS23.Debug+ITSboard"
  "/Users/jannik/Studium lokal/GSP/ITS-BRD-VSC/Programs/AltKlausurCDSS23/tmp/ITSboard/default/1"
  "/Users/jannik/Studium lokal/GSP/ITS-BRD-VSC/Programs/AltKlausurCDSS23/tmp/ITSboard/default/AltKlausurCDSS23.Debug+ITSboard"
  "/Users/jannik/Studium lokal/GSP/ITS-BRD-VSC/Programs/AltKlausurCDSS23/tmp/ITSboard/default/AltKlausurCDSS23.Debug+ITSboard/tmp"
  "/Users/jannik/Studium lokal/GSP/ITS-BRD-VSC/Programs/AltKlausurCDSS23/tmp/ITSboard/default/AltKlausurCDSS23.Debug+ITSboard/src/AltKlausurCDSS23.Debug+ITSboard-stamp"
  "/Users/jannik/Studium lokal/GSP/ITS-BRD-VSC/Programs/AltKlausurCDSS23/tmp/ITSboard/default/AltKlausurCDSS23.Debug+ITSboard/src"
  "/Users/jannik/Studium lokal/GSP/ITS-BRD-VSC/Programs/AltKlausurCDSS23/tmp/ITSboard/default/AltKlausurCDSS23.Debug+ITSboard/src/AltKlausurCDSS23.Debug+ITSboard-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/jannik/Studium lokal/GSP/ITS-BRD-VSC/Programs/AltKlausurCDSS23/tmp/ITSboard/default/AltKlausurCDSS23.Debug+ITSboard/src/AltKlausurCDSS23.Debug+ITSboard-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/jannik/Studium lokal/GSP/ITS-BRD-VSC/Programs/AltKlausurCDSS23/tmp/ITSboard/default/AltKlausurCDSS23.Debug+ITSboard/src/AltKlausurCDSS23.Debug+ITSboard-stamp${cfgdir}") # cfgdir has leading slash
endif()
