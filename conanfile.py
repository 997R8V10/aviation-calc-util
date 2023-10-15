import os
import glob
from shutil import copyfile
from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps
from conan.tools import files


class AviationcalcConan(ConanFile):
    name = "aviationcalc"
    version = "1.0.20"
    license = "None"
    author = "Prithvi Shivaraman prithvisagar.shivaraman@gmail.com"
    url = "https://github.com/997R8V10/aviation-calc-util"
    description = "Utilities to perform aviation related calculations"
    topics = ("aviation", "calculator", "calculations")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {
        "shared": True,
        "fPIC": True,
        "boost/*:header_only": False,
        "boost/*:multithreading": True,
        "boost/*:without_atomic": True,
        "boost/*:without_chrono": True,
        "boost/*:without_container": True,
        "boost/*:without_context": True,
        "boost/*:without_contract": True,
        "boost/*:without_coroutine": True,
        "boost/*:without_date_time": False,
        "boost/*:without_exception": True,
        "boost/*:without_fiber": True,
        "boost/*:without_filesystem": True,
        "boost/*:without_graph": True,
        "boost/*:without_graph_parallel": True,
        "boost/*:without_iostreams": True,
        "boost/*:without_json": True,
        "boost/*:without_locale": True,
        "boost/*:without_log": True,
        "boost/*:without_math": True,
        "boost/*:without_mpi": True,
        "boost/*:without_nowide": True,
        "boost/*:without_program_options": True,
        "boost/*:without_python": True,
        "boost/*:without_random": True,
        "boost/*:without_regex": True,
        "boost/*:without_serialization": True,
        "boost/*:without_stacktrace": True,
        "boost/*:without_system": True,
        "boost/*:without_test": True,
        "boost/*:without_thread": True,
        "boost/*:without_timer": True,
        "boost/*:without_type_erasure": True,
        "boost/*:without_wave": True,
        "eccodes/*:multithreading": True,
        "eccodes/*:enable_netcdf": False,
        "eccodes/*:enable_jpg": False,
        "eccodes/*:enable_fortran": False,
        "eccodes/*:enable_product_bufr": False,
        "eccodes/*:enable_build_tools": False,
        "eccodes/*:enable_examples": False,
        "eccodes/*:enable_tests": False,
        "eccodes/*:enable_install_eccodes_definitions": True,
        "eccodes/*:enable_install_eccodes_samples": False,
        "eccodes/*:enable_memfs": True
    }
    requires = ["boost/1.76.0", "eccodes/2.22.1.p3"]
    no_copy_source = True
    exports_sources = ["src/*", "include/*", "cmakescripts/*", "CMakeLists.txt"]
    build_policy = "missing"

    def requirements(self):
        if self.settings.os == "Linux":
            self.requires("libcurl/7.87.0")

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

        self.options["boost"].shared = self.options.shared
        self.options["eccodes"].shared = self.options.shared
        self.options["libcurl"].shared = self.options.shared

    # def imports(self):
    #     files.copy(self, "*.dll", dst=os.path.join(self.imports_folder, "bin"), src=os.path.join(self.build_folder, "bin"))
    #     files.copy(self, "*.dylib*", dst=os.path.join(self.imports_folder, "lib"), src=os.path.join(self.build_folder, "lib"))
    #     files.copy(self, "*.lib", dst=os.path.join(self.imports_folder, "lib"), src=os.path.join(self.build_folder, "lib"))
    #     files.copy(self, "*.so*", dst=os.path.join(self.imports_folder, "lib"), src=os.path.join(self.build_folder, "lib"))
    #
    #     with files.chdir(os.path.join(self.imports_folder, "lib")):
    #         for dynamic_lib in glob.iglob("*.so.*", recursive=True):
    #             lib_symlink = "{}.so".format(dynamic_lib.split(".so")[0])  # basename
    #             if os.path.exists(lib_symlink):
    #                 os.remove(lib_symlink)
    #             self.output.info("Copying {} --> {}".format(dynamic_lib, lib_symlink))
    #             copyfile(dynamic_lib, lib_symlink)
    #     #self.copy("*.*", dst="bin/eccodes/definitions", src="data/eccodes/definitions")

    def generate(self):
        # Copy imports
        for dep in self.dependencies.values():
            if dep.package_folder is not None:
                files.copy(self, "*.dll", src=os.path.join(dep.package_folder, "bin"), dst=os.path.join(self.build_folder, "bin"))
                files.copy(self, "*.dylib*", src=os.path.join(dep.package_folder, "lib"), dst=os.path.join(self.build_folder, "lib"))
                files.copy(self, "*.lib", src=os.path.join(dep.package_folder, "lib"), dst=os.path.join(self.build_folder, "lib"))
                files.copy(self, "*.so*", src=os.path.join(dep.package_folder, "lib"), dst=os.path.join(self.build_folder, "lib"))

                with files.chdir(self, os.path.join(self.build_folder, "lib")):
                    for dynamic_lib in glob.iglob("*.so.*", recursive=True):
                        lib_symlink = "{}.so".format(dynamic_lib.split(".so")[0])  # basename
                        if os.path.exists(lib_symlink):
                            os.remove(lib_symlink)
                        self.output.info("Copying {} --> {}".format(dynamic_lib, lib_symlink))
                        copyfile(dynamic_lib, lib_symlink)

        tc = CMakeToolchain(self)

        # Build Type
        if self.settings.build_type == "Debug":
            tc.cache_variables["CMAKE_BUILD_TYPE"] = "Debug"
        else:
            tc.cache_variables["CMAKE_BUILD_TYPE"] = "Release"

        tc.generate()
        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cmake = CMake(self)

        # if cmake.is_multi_configuration:
        #     cmake.definitions["CMAKE_BUILD_TYPE"] = self.settings.build_type

        cmake.configure()

        #if cmake.is_multi_configuration:
        #    cmake.build(target="aviationcalc", args=["--config %s" % self.settings.build_type])
        #else:
        cmake.build(target="aviationcalc")

    def package(self):
        files.copy(self, "*.h", dst=os.path.join(self.package_folder, "include"), src=os.path.join(self.build_folder, "include"))
        files.copy(self, "aviationcalc_exports.h", dst=os.path.join(self.package_folder, "include"), src=os.path.join(self.build_folder, "exports"), keep_path=False)
        files.copy(self, "*.lib", dst=os.path.join(self.package_folder, "lib"), src=self.build_folder, keep_path=False)
        files.copy(self, "*.exp", dst=os.path.join(self.package_folder, "lib"), src=self.build_folder, keep_path=False)
        files.copy(self, "*.dll", dst=os.path.join(self.package_folder, "bin"), src=self.build_folder, keep_path=False)
        files.copy(self, "*.pdb", dst=os.path.join(self.package_folder, "bin"), src=self.build_folder, keep_path=False)
        files.copy(self, "*.ilk", dst=os.path.join(self.package_folder, "bin"), src=self.build_folder, keep_path=False)
        files.copy(self, "*.dylib*", dst=os.path.join(self.package_folder, "lib"), src=self.build_folder, keep_path=False)
        files.copy(self, "*.so", dst=os.path.join(self.package_folder, "lib"), src=self.build_folder, keep_path=False)
        files.copy(self, "*.a", dst=os.path.join(self.package_folder, "lib"), src=self.build_folder, keep_path=False)
        # self.copy("*", dst="bin/eccodes", src="bin/eccodes")

    def package_info(self):
        self.cpp_info.libs = ["aviationcalc"]
