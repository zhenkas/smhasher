# CMake generated Testfile for 
# Source directory: C:/Users/JKS/source/repos/zhenkas/smhasher
# Build directory: C:/Users/JKS/source/repos/zhenkas/smhasher/SLN
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(VerifyAll "SMHasher" "--test=VerifyAll")
set_tests_properties(VerifyAll PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/JKS/source/repos/zhenkas/smhasher/CMakeLists.txt;757;add_test;C:/Users/JKS/source/repos/zhenkas/smhasher/CMakeLists.txt;0;")
add_test(Sanity "SMHasher" "--test=Sanity")
set_tests_properties(Sanity PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/JKS/source/repos/zhenkas/smhasher/CMakeLists.txt;758;add_test;C:/Users/JKS/source/repos/zhenkas/smhasher/CMakeLists.txt;0;")
add_test(Speed "SMHasher" "--test=Speed")
set_tests_properties(Speed PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/JKS/source/repos/zhenkas/smhasher/CMakeLists.txt;759;add_test;C:/Users/JKS/source/repos/zhenkas/smhasher/CMakeLists.txt;0;")
add_test(Cyclic "SMHasher" "--test=Cyclic")
set_tests_properties(Cyclic PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/JKS/source/repos/zhenkas/smhasher/CMakeLists.txt;760;add_test;C:/Users/JKS/source/repos/zhenkas/smhasher/CMakeLists.txt;0;")
add_test(Zeroes "SMHasher" "--test=Zeroes")
set_tests_properties(Zeroes PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/JKS/source/repos/zhenkas/smhasher/CMakeLists.txt;761;add_test;C:/Users/JKS/source/repos/zhenkas/smhasher/CMakeLists.txt;0;")
add_test(Seed "SMHasher" "--test=Seed")
set_tests_properties(Seed PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/JKS/source/repos/zhenkas/smhasher/CMakeLists.txt;762;add_test;C:/Users/JKS/source/repos/zhenkas/smhasher/CMakeLists.txt;0;")
