# CMake generated Testfile for 
# Source directory: F:/Invisible_Girl_Game/External_Libraries/tinyxml2-master
# Build directory: F:/Invisible_Girl_Game/External_Libraries/tinyxml2-master/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(xmltest "F:/Invisible_Girl_Game/External_Libraries/tinyxml2-master/build/Debug/xmltest.exe")
  set_tests_properties(xmltest PROPERTIES  PASS_REGULAR_EXPRESSION ", Fail 0" WORKING_DIRECTORY "F:/Invisible_Girl_Game/External_Libraries/tinyxml2-master" _BACKTRACE_TRIPLES "F:/Invisible_Girl_Game/External_Libraries/tinyxml2-master/CMakeLists.txt;48;add_test;F:/Invisible_Girl_Game/External_Libraries/tinyxml2-master/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(xmltest "F:/Invisible_Girl_Game/External_Libraries/tinyxml2-master/build/Release/xmltest.exe")
  set_tests_properties(xmltest PROPERTIES  PASS_REGULAR_EXPRESSION ", Fail 0" WORKING_DIRECTORY "F:/Invisible_Girl_Game/External_Libraries/tinyxml2-master" _BACKTRACE_TRIPLES "F:/Invisible_Girl_Game/External_Libraries/tinyxml2-master/CMakeLists.txt;48;add_test;F:/Invisible_Girl_Game/External_Libraries/tinyxml2-master/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(xmltest "F:/Invisible_Girl_Game/External_Libraries/tinyxml2-master/build/MinSizeRel/xmltest.exe")
  set_tests_properties(xmltest PROPERTIES  PASS_REGULAR_EXPRESSION ", Fail 0" WORKING_DIRECTORY "F:/Invisible_Girl_Game/External_Libraries/tinyxml2-master" _BACKTRACE_TRIPLES "F:/Invisible_Girl_Game/External_Libraries/tinyxml2-master/CMakeLists.txt;48;add_test;F:/Invisible_Girl_Game/External_Libraries/tinyxml2-master/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(xmltest "F:/Invisible_Girl_Game/External_Libraries/tinyxml2-master/build/RelWithDebInfo/xmltest.exe")
  set_tests_properties(xmltest PROPERTIES  PASS_REGULAR_EXPRESSION ", Fail 0" WORKING_DIRECTORY "F:/Invisible_Girl_Game/External_Libraries/tinyxml2-master" _BACKTRACE_TRIPLES "F:/Invisible_Girl_Game/External_Libraries/tinyxml2-master/CMakeLists.txt;48;add_test;F:/Invisible_Girl_Game/External_Libraries/tinyxml2-master/CMakeLists.txt;0;")
else()
  add_test(xmltest NOT_AVAILABLE)
endif()
