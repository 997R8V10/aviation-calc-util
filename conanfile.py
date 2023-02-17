from conans import ConanFile, CMake, tools
import os
import glob
from shutil import copyfile


class AviationcalcConan(ConanFile):
    name = "aviationcalc"
    version = "1.0.6"
    license = "None"
    author = "Prithvi Shivaraman prithvisagar.shivaraman@gmail.com"
    url = "https://gitlab.com/vatsim-tools/aviation-calc-util"
    description = "Utilities to perform aviation related calculations"
    topics = ("aviation", "calculator", "calculations")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {
        "shared": True,
        "fPIC": True,
        "boost:header_only": False,
        "boost:multithreading": True,
        "boost:without_atomic": True,
        "boost:without_chrono": True,
        "boost:without_container": True,
        "boost:without_context": True,
        "boost:without_contract": True,
        "boost:without_coroutine": True,
        "boost:without_date_time": False,
        "boost:without_exception": True,
        "boost:without_fiber": True,
        "boost:without_filesystem": True,
        "boost:without_graph": True,
        "boost:without_graph_parallel": True,
        "boost:without_iostreams": True,
        "boost:without_json": True,
        "boost:without_locale": True,
        "boost:without_log": True,
        "boost:without_math": True,
        "boost:without_mpi": True,
        "boost:without_nowide": True,
        "boost:without_program_options": True,
        "boost:without_python": True,
        "boost:without_random": True,
        "boost:without_regex": True,
        "boost:without_serialization": True,
        "boost:without_stacktrace": True,
        "boost:without_system": True,
        "boost:without_test": True,
        "boost:without_thread": True,
        "boost:without_timer": True,
        "boost:without_type_erasure": True,
        "boost:without_wave": True,
        "eccodes:multithreading": True,
        "eccodes:enable_netcdf": False,
        "eccodes:enable_jpg": False,
        "eccodes:enable_fortran": False,
        "eccodes:enable_product_bufr": False,
        "eccodes:enable_build_tools": False,
        "eccodes:enable_examples": False,
        "eccodes:enable_tests": False,
        "eccodes:enable_install_eccodes_definitions": True,
        "eccodes:enable_install_eccodes_samples": False,
        "eccodes:enable_memfs": True
    }
    requires = ["boost/1.76.0", "eccodes/2.22.1"]
    generators = ["cmake", "cmake_find_package", "cmake_paths", "txt"]
    no_copy_source = True
    exports_sources = ["src/*", "include/*", "cmakescripts/*", "CMakeLists.txt"]
    build_policy = "missing"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

        self.options["boost"].shared = self.options.shared
        self.options["eccodes"].shared = self.options.shared

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.dylib*", dst="lib", src="lib")
        self.copy("*.lib", dst="lib", src="lib")
        self.copy("*.so*", dst="lib", src="lib")

        with tools.chdir(os.path.join(self.imports_folder, "lib")):
            for dynamic_lib in glob.iglob("*.so.*", recursive=True):
                lib_symlink = "{}.so".format(dynamic_lib.split(".so")[0])  # basename
                if os.path.exists(lib_symlink):
                    os.remove(lib_symlink)
                self.output.info("Copying {} --> {}".format(dynamic_lib, lib_symlink))
                copyfile(dynamic_lib, lib_symlink)
        #self.copy("*.*", dst="bin/eccodes/definitions", src="data/eccodes/definitions")

    def build(self):
        cmake = CMake(self)

        if cmake.is_multi_configuration:
            cmake.definitions["CMAKE_BUILD_TYPE"] = self.settings.build_type

        cmake.configure()

        if cmake.is_multi_configuration:
            cmake.build(target="aviationcalc", args=["--config %s" % self.settings.build_type])
        else:
            cmake.build(target="aviationcalc")

    def package(self):
        self.copy("*.h", dst="include", src="include")
        self.copy("aviationcalc_exports.h", dst="include", src="exports", keep_path=False)
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.exp", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.pdb", dst="bin", keep_path=False)
        self.copy("*.ilk", dst="bin", keep_path=False)
        self.copy("*.dylib*", dst="lib", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)
        # self.copy("*", dst="bin/eccodes", src="bin/eccodes")

    def package_info(self):
        self.cpp_info.libs = ["aviationcalc"]
