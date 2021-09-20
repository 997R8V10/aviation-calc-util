from conans import ConanFile, CMake, tools


class EccodesConan(ConanFile):
	name = "eccodes"
	version = "2.22.1"
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
	build_requires = []
	generators = ["cmake", "txt"]
	build_policy = "missing"

	def config_options(self):
		if self.settings.os == "Windows":
			del self.options.fPIC
			self.build_requires.append("strawberryperl/5.28.1.1")

	def source(self):
		url = "https://confluence.ecmwf.int/download/attachments/45757960/eccodes-2.22.1-Source.tar.gz"
		tools.download(url, "eccodes.tar.gz")
		tools.untargz("eccodes.tar.gz")
		tools.rename("eccodes-2.22.1-Source", "eccodes")
		
		# This small hack might be useful to guarantee proper /MT /MD linkage
		# in MSVC if the packaged project doesn't have variables to set it
		# properly
		tools.replace_in_file("eccodes/CMakeLists.txt", "project( eccodes VERSION 2.22.1 LANGUAGES C )",
							  '''project( eccodes VERSION 2.22.1 LANGUAGES C )
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()''')

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
			tools.patch(base_path="eccodes", patch_string=definitions_cmake_patch, fuzz=True)
			tools.patch(base_path="eccodes", patch_string=ifssamples_cmake_path, fuzz=True)
			tools.patch(base_path="eccodes", patch_string=check_os_patch, fuzz=True)

		cmake = CMake(self)

		# Configure options
		if self.options.multithreading:
			cmake.definitions["ENABLE_ECCODES_THREADS"] = "OFF" if self.settings.os == "Windows" else "ON"
			cmake.definitions["ENABLE_ECCODES_OMP_THREADS"] = "ON" if self.settings.os == "Windows" else "OFF"
		else:
			cmake.definitions["ENABLE_ECCODES_THREADS"] = "OFF"
			cmake.definitions["ENABLE_ECCODES_OMP_THREADS"] = "OFF"

		cmake.definitions["ENABLE_NETCDF"] = "ON" if self.options.enable_netcdf else "OFF"
		cmake.definitions["ENABLE_JPG"] = "ON" if self.options.enable_jpg else "OFF"
		cmake.definitions["ENABLE_FORTRAN"] = "ON" if self.options.enable_fortran else "OFF"
		cmake.definitions["ENABLE_PRODUCT_BUFR"] = "ON" if self.options.enable_product_bufr else "OFF"
		cmake.definitions["ENABLE_BUILD_TOOLS"] = "ON" if self.options.enable_build_tools else "OFF"
		cmake.definitions["ENABLE_EXAMPLES"] = "ON" if self.options.enable_examples else "OFF"
		cmake.definitions["ENABLE_TESTS"] = "ON" if self.options.enable_tests else "OFF"
		cmake.definitions["ENABLE_INSTALL_ECCODES_DEFINITIONS"] = "ON" if self.options.enable_install_eccodes_definitions else "OFF"
		cmake.definitions["ENABLE_INSTALL_ECCODES_SAMPLES"] = "ON" if self.options.enable_install_eccodes_samples else "OFF"
		cmake.definitions["ENABLE_MEMFS"] = "ON" if self.options.enable_memfs else "OFF"
		cmake.definitions["ENABLE_AEC"] = "ON" if self.options.enable_aec else "OFF"
		cmake.definitions["ENABLE_PYTHON2"] = "ON" if self.options.enable_python2 else "OFF"
		cmake.definitions["ENABLE_EXTRA_TESTS"] = "ON" if self.options.enable_extra_tests else "OFF"
		
		# LE Check
		cmake.definitions["IEEE_LE"] = 1

		cmake.configure(source_folder="eccodes")

		if cmake.is_multi_configuration:
			cmake.build(target="eccodes", args=["--config %s" % self.settings.build_type])
		else:
			cmake.build(target="eccodes")

	def package(self):
		self.copy("*.h", dst="include", keep_path=False)
		self.copy("*.lib", dst="lib", keep_path=False)
		self.copy("*.exp", dst="lib", keep_path=False)
		self.copy("*.dll", dst="bin", keep_path=False)
		self.copy("*.pdb", dst="bin", keep_path=False)
		self.copy("*.ilk", dst="bin", keep_path=False)
		self.copy("*.dylib*", dst="lib", keep_path=False)
		self.copy("*.so", dst="lib", keep_path=False)
		self.copy("*.a", dst="lib", keep_path=False)

		if not self.options.enable_memfs:
			self.copy("*.def", dst="data/eccodes/definitions", src="eccodes/definitions")
			self.copy("*.table", dst="data/eccodes/definitions", src="eccodes/definitions")
			self.copy("*.pl", dst="data/eccodes/ifs_samples", src="eccodes/ifs_samples")
			self.copy("*.tmpl", dst="data/eccodes/ifs_samples", src="eccodes/ifs_samples")

		if self.options.enable_examples:
			self.copy("*.sh", dst="data/eccodes/examples", src="eccodes/examples")

	def package_info(self):
		self.cpp_info.libs = ["eccodes"]