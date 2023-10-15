from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps
from conan.tools import files
import os

class EccodesConan(ConanFile):
	name = "eccodes"
	version = "2.22.1.p3"
	license = "Apache Licence Version 2.0"
	author = "ecwmf"
	url = "https://github.com/ecmwf/eccodes"
	description = """ 	ecCodes is a package developed by ECMWF which provides an application programming interface and a set of tools for decoding and encoding messages in the following formats:

						WMO FM-92 GRIB edition 1 and edition 2
						WMO FM-94 BUFR edition 3 and edition 4
						WMO GTS abbreviated header (only decoding)

						A useful set of command line tools provide quick access to the messages. C, Fortran 90 and Python interfaces provide access to the main ecCodes functionality.

						ecCodes is an evolution of GRIB API. It is designed to provide the user with a simple set of functions to access data from several formats with a key/value approach.

						Documentation can be found here: https://software.ecmwf.int/wiki/display/ECC/ecCodes+Home"""
	topics = ("ecwmf", "eccodes")
	settings = "os", "compiler", "build_type", "arch"
	options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "multithreading": [True, False],
		"enable_memfs": [True, False],
		"enable_netcdf": [True, False],
		"enable_jpg": [True, False],
		"enable_fortran": [True, False],
		"enable_product_bufr": [True, False],
		"enable_build_tools": [True, False],
		"enable_examples": [True, False],
		"enable_tests": [True, False],
		"enable_install_eccodes_definitions": [True, False],
		"enable_install_eccodes_samples": [True, False],
		"enable_aec": [True, False],
		"enable_python2": [True, False],
		"enable_extra_tests": [True, False]
    }
	default_options = {
		"shared": False,
		"fPIC": True,
		"multithreading": False,
		"enable_memfs": False,
		"enable_netcdf": False,
		"enable_jpg": False,
		"enable_fortran": False,
		"enable_product_bufr": False,
		"enable_build_tools": False,
		"enable_examples": False,
		"enable_tests": False,
		"enable_install_eccodes_definitions": True,
		"enable_install_eccodes_samples": False,
		"enable_aec": False,
		"enable_python2": False,
		"enable_extra_tests": False
	}
	tool_requires = []
	build_policy = "missing"

	def config_options(self):
		if self.settings.os == "Windows":
			del self.options.fPIC
			self.tool_requires.append("strawberryperl/5.28.1.1")

	def source(self):
		url = "https://confluence.ecmwf.int/download/attachments/45757960/eccodes-2.22.1-Source.tar.gz"
		files.download(self, url, "eccodes.tar.gz")
		files.unzip(self, "eccodes.tar.gz")
		#rename(self, "eccodes-2.22.1-Source", "eccodes")
		
		# This small hack might be useful to guarantee proper /MT /MD linkage
		# in MSVC if the packaged project doesn't have variables to set it
		# properly
		files.replace_in_file(self, "eccodes-2.22.1-Source/CMakeLists.txt", "project( eccodes VERSION 2.22.1 LANGUAGES C )",
							  '''set(CMAKE_CONFIGURATION_TYPES Release)
project( eccodes VERSION 2.22.1 LANGUAGES C )
#include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
#conan_basic_setup(KEEP_RPATHS)''')

	def generate(self):
		tc = CMakeToolchain(self)

		# Configure options
		if self.options.multithreading:
			tc.cache_variables["ENABLE_ECCODES_THREADS"] = "OFF" if self.settings.os == "Windows" else "ON"
			tc.cache_variables["ENABLE_ECCODES_OMP_THREADS"] = "ON" if self.settings.os == "Windows" else "OFF"
		else:
			tc.cache_variables["ENABLE_ECCODES_THREADS"] = "OFF"
			tc.cache_variables["ENABLE_ECCODES_OMP_THREADS"] = "OFF"

		tc.cache_variables["ENABLE_NETCDF"] = "ON" if self.options.enable_netcdf else "OFF"
		tc.cache_variables["ENABLE_JPG"] = "ON" if self.options.enable_jpg else "OFF"
		tc.cache_variables["ENABLE_FORTRAN"] = "ON" if self.options.enable_fortran else "OFF"
		tc.cache_variables["ENABLE_PRODUCT_BUFR"] = "ON" if self.options.enable_product_bufr else "OFF"
		tc.cache_variables["ENABLE_BUILD_TOOLS"] = "ON" if self.options.enable_build_tools else "OFF"
		tc.cache_variables["ENABLE_EXAMPLES"] = "ON" if self.options.enable_examples else "OFF"
		tc.cache_variables["ENABLE_TESTS"] = "ON" if self.options.enable_tests else "OFF"
		tc.cache_variables["ENABLE_INSTALL_ECCODES_DEFINITIONS"] = "ON" if self.options.enable_install_eccodes_definitions else "OFF"
		tc.cache_variables["ENABLE_INSTALL_ECCODES_SAMPLES"] = "ON" if self.options.enable_install_eccodes_samples else "OFF"
		tc.cache_variables["ENABLE_MEMFS"] = "ON" if self.options.enable_memfs else "OFF"
		tc.cache_variables["ENABLE_AEC"] = "ON" if self.options.enable_aec else "OFF"
		tc.cache_variables["ENABLE_PYTHON2"] = "ON" if self.options.enable_python2 else "OFF"
		tc.cache_variables["ENABLE_EXTRA_TESTS"] = "ON" if self.options.enable_extra_tests else "OFF"

		# RPath Setup
		if self.settings.os == "Macos":
			tc.cache_variables["CMAKE_BUILD_WITH_INSTALL_RPATH"] = "1"
			tc.cache_variables["CMAKE_INSTALL_RPATH"] = "@loader_path"
		elif self.settings.os == "Linux":
			tc.cache_variables["CMAKE_BUILD_WITH_INSTALL_RPATH"] = "1"
			tc.cache_variables["CMAKE_INSTALL_RPATH"] = "$ORIGIN"

		# LE Check
		tc.cache_variables["IEEE_LE"] = 1

		# Build Type
		tc.cache_variables["CMAKE_BUILD_TYPE"] = "Release"

		tc.generate()
		deps = CMakeDeps(self)
		deps.generate()

	def build(self):
		# Replace some files on windows builds
		if self.settings.os == "Windows":
			definitions_cmake_patch = """--- definitions/CMakeLists.txt	Thu Jun 17 05:22:21 2021
+++ definitions/CMakeLists.txt	Wed Sep 01 10:36:26 2021
@@ -43,12 +43,12 @@
 endif()
 
 # link to the definitions. See GRIB-786
-file( MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${INSTALL_DATA_DIR} )
-if( NOT EXISTS "${CMAKE_BINARY_DIR}/${ECCODES_DEFINITION_SUFF}" )
-  execute_process( COMMAND "${CMAKE_COMMAND}" "-E" "create_symlink"
-    "${CMAKE_CURRENT_SOURCE_DIR}"
-    "${CMAKE_BINARY_DIR}/${ECCODES_DEFINITION_SUFF}" )
-endif()
+#file( MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${INSTALL_DATA_DIR} )
+#if( NOT EXISTS "${CMAKE_BINARY_DIR}/${ECCODES_DEFINITION_SUFF}" )
+#  execute_process( COMMAND "${CMAKE_COMMAND}" "-E" "create_symlink"
+#    "${CMAKE_CURRENT_SOURCE_DIR}"
+#    "${CMAKE_BINARY_DIR}/${ECCODES_DEFINITION_SUFF}" )
+#endif()
 
 # copy the definitions to the build directory
 #file(    COPY ${definition_files} ${table_files} ${text_files}
"""
			ifssamples_cmake_path="""--- ifs_samples/CMakeLists.txt	Thu Jun 17 05:22:21 2021
+++ ifs_samples/CMakeLists.txt	Wed Sep 01 12:33:52 2021
@@ -10,13 +10,13 @@
 
   endif()
 
-  # To symlink the samples to the build-dir for developers
-  execute_process( COMMAND ${CMAKE_COMMAND} -E make_directory ${destination_build} )
-  foreach( _file ${_samples} )
-    get_filename_component(_filename ${_file} NAME)
-    execute_process( COMMAND ${CMAKE_COMMAND} -E create_symlink
-       ${_file} ${destination_build}/${_filename} )
-  endforeach()
+  ## To symlink the samples to the build-dir for developers
+  #execute_process( COMMAND ${CMAKE_COMMAND} -E make_directory ${destination_build} )
+  #foreach( _file ${_samples} )
+  #  get_filename_component(_filename ${_file} NAME)
+  #  execute_process( COMMAND ${CMAKE_COMMAND} -E create_symlink
+  #     ${_file} ${destination_build}/${_filename} )
+  #endforeach()
 endfunction()
 
 add_subdirectory( grib1 )
"""
			check_os_patch="""--- cmake/ecbuild_check_os.cmake	Tue May 11 09:42:16 2021
+++ cmake/ecbuild_check_os.cmake	Wed Sep 01 00:26:00 2021
@@ -426,26 +426,26 @@
 
   set( EC_OS_NAME "windows" )
 
-  find_program( BASH_EXE NAMES bash
-                         DOC "Used under Windows for fixing symlinks and running unit tests" )
-
-  if( NOT BASH_EXE )
-      ecbuild_critical("Could not find program 'bash'. Specify the location with -DBASH_EXE=C:/...")
-  endif()
-
-  ecbuild_warn( "CMake doesn't support symlinks on Windows. "
-                "Replacing all symlinks with copies." )
-  execute_process( COMMAND ${BASH_EXE} -c "${ECBUILD_MACROS_DIR}/ecbuild_windows_replace_symlinks.sh"
-                   WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
-                   RESULT_VARIABLE CMD_RESULT
-                   OUTPUT_VARIABLE CMD_OUTPUT
-                   ERROR_VARIABLE  CMD_ERROR )
-  ecbuild_debug( "Windows Replace Symlinks res=[${CMD_RESULT}] "
-                 "output=[${CMD_OUTPUT}] error=[${CMD_ERROR}]" )
-  if( CMD_RESULT )
-    ecbuild_critical( "Failed to replace windows symlinks. "
-                      "output=[${CMD_OUTPUT}] error=[${CMD_ERROR}]" )
-  endif()
+  #find_program( BASH_EXE NAMES bash
+  #                       DOC "Used under Windows for fixing symlinks and running unit tests" )
+  #
+  #if( NOT BASH_EXE )
+  #    ecbuild_critical("Could not find program 'bash'. Specify the location with -DBASH_EXE=C:/...")
+  #endif()
+  #
+  #ecbuild_warn( "CMake doesn't support symlinks on Windows. "
+  #              "Replacing all symlinks with copies." )
+  #execute_process( COMMAND ${BASH_EXE} -c "cmake/ecbuild_windows_replace_symlinks.sh"
+  #                 WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
+  #                 RESULT_VARIABLE CMD_RESULT
+  #                 OUTPUT_VARIABLE CMD_OUTPUT
+  #                 ERROR_VARIABLE  CMD_ERROR )
+  #ecbuild_debug( "Windows Replace Symlinks res=[${CMD_RESULT}] "
+  #               "output=[${CMD_OUTPUT}] error=[${CMD_ERROR}]" )
+  #if( CMD_RESULT )
+  #  ecbuild_critical( "Failed to replace windows symlinks. "
+  #                    "output=[${CMD_OUTPUT}] error=[${CMD_ERROR}]" )
+  #endif()
 
 endif()
 
"""
			files.patch(self, base_path="eccodes-2.22.1-Source", patch_string=definitions_cmake_patch, fuzz=True)
			files.patch(self, base_path="eccodes-2.22.1-Source", patch_string=ifssamples_cmake_path, fuzz=True)
			files.patch(self, base_path="eccodes-2.22.1-Source", patch_string=check_os_patch, fuzz=True)
		# Add RPath Patch
		ecbuild_declare_project_patch = """--- cmake/ecbuild_declare_project.cmake	Tue May 11 08:42:16 2021
+++ cmake/ecbuild_declare_project.cmake	Fri Feb 17 00:44:46 2023
@@ -171,25 +171,12 @@
 
   # correctly set CMAKE_INSTALL_RPATH
 
-  if( ENABLE_RPATHS )
-
-    if( ENABLE_RELATIVE_RPATHS )
-
-      file( RELATIVE_PATH relative_rpath ${${PROJECT_NAME}_FULL_INSTALL_BIN_DIR} ${${PROJECT_NAME}_FULL_INSTALL_LIB_DIR} )
-      # ecbuild_debug_var( relative_rpath )
-
-      ecbuild_append_to_rpath( ${relative_rpath} )
-
-    else() # make rpaths absolute
-
-      if( IS_ABSOLUTE ${INSTALL_LIB_DIR} )
-        ecbuild_append_to_rpath( "${INSTALL_LIB_DIR}" )
-      else()
-        ecbuild_append_to_rpath( "${CMAKE_INSTALL_PREFIX}/${INSTALL_LIB_DIR}" )
-      endif()
-
-    endif()
-
+  if (APPLE)
+    set(CMAKE_BUILD_WITH_INSTALL_RPATH 1)
+    set(CMAKE_INSTALL_RPATH "@loader_path")
+  elseif(UNIX)
+  	set(CMAKE_BUILD_WITH_INSTALL_RPATH 1)
+  	set(CMAKE_INSTALL_RPATH "$ORIGIN")
   endif()
 
   # make sure nothing breaks if INSTALL_LIB_DIR is not lib
"""
		files.patch(self, base_path="eccodes-2.22.1-Source", patch_string=ecbuild_declare_project_patch, fuzz=True)

		cmake = CMake(self)

		cmake.configure(build_script_folder="eccodes-2.22.1-Source")

		#if cmake.is_multi_configuration:
		#	cmake.build(target="eccodes", args=["--config %s" % self.settings.build_type])
		#else:
		cmake.build(target="eccodes", build_type="Release")

	def package(self):
		files.copy(self, "*.h", dst=os.path.join(self.package_folder, "include"), src=self.build_folder, keep_path=False)
		files.copy(self, "*.lib", dst=os.path.join(self.package_folder, "lib"), src=self.build_folder, keep_path=False)
		files.copy(self, "*.exp", dst=os.path.join(self.package_folder, "lib"), src=self.build_folder, keep_path=False)
		files.copy(self, "*.dll", dst=os.path.join(self.package_folder, "bin"), src=self.build_folder, keep_path=False)
		files.copy(self, "*.pdb", dst=os.path.join(self.package_folder, "bin"), src=self.build_folder, keep_path=False)
		files.copy(self, "*.ilk", dst=os.path.join(self.package_folder, "bin"), src=self.build_folder, keep_path=False)
		files.copy(self, "*.dylib*", dst=os.path.join(self.package_folder, "lib"), src=self.build_folder, keep_path=False)
		files.copy(self, "*.so", dst=os.path.join(self.package_folder, "lib"), src=self.build_folder, keep_path=False)
		files.copy(self, "*.a", dst=os.path.join(self.package_folder, "lib"), src=self.build_folder, keep_path=False)

		if not self.options.enable_memfs:
			files.copy(self, "*.def", dst=os.path.join(self.package_folder, "data/eccodes/definitions"), src=os.path.join(self.build_folder, "eccodes-2.22.1-Source/definitions"))
			files.copy(self, "*.table", dst=os.path.join(self.package_folder, "data/eccodes/definitions"), src=os.path.join(self.build_folder, "eccodes-2.22.1-Source/definitions"))
			files.copy(self, "*.pl", dst=os.path.join(self.package_folder, "data/eccodes/ifs_samples"), src=os.path.join(self.build_folder, "eccodes-2.22.1-Source/ifs_samples"))
			files.copy(self, "*.tmpl", dst=os.path.join(self.package_folder, "data/eccodes/ifs_samples"), src=os.path.join(self.build_folder, "eccodes-2.22.1-Source/ifs_samples"))

		if self.options.enable_examples:
			files.copy(self, "*.sh", dst=os.path.join(self.package_folder, "data/eccodes/examples"), src=os.path.join(self.build_folder, "eccodes-2.22.1-Source/examples"))

	def package_info(self):
		self.cpp_info.libs = ["eccodes"]
		if self.options.enable_memfs:
			self.cpp_info.libs.append("eccodes_memfs")