from conans import ConanFile, CMake


class AviationcalcConan(ConanFile):
    name = "aviationcalc"
    version = "0.1"
    license = "<Put the package license here>"
    author = "Prithvi Shivaraman prithvisagar.shivaraman@gmail.com"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "Utilities to perform aviation related calculations"
    topics = ("aviation", "calculator", "calculations")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {
        "shared": True,
        "fPIC": True,
        "boost:multithreading": True,
        "boost:without_atomic": True,
        "boost:without_chrono": True,
        "boost:without_container": True,
        "boost:without_context": True,
        "boost:without_contract": True,
        "boost:without_coroutine": True,
        "boost:without_date_time": True,
        "boost:without_exception": True,
        "boost:without_fiber": True,
        "boost:without_filesystem": False,
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
        "boost:without_system": False,
        "boost:without_test": True,
        "boost:without_thread": True,
        "boost:without_timer": True,
        "boost:without_type_erasure": True,
        "boost:without_wave": True
    }
    requires = ["boost/1.76.0", "strawberryperl/5.28.1.1@conan/stable"]
    generators = ["cmake", "cmake_find_package", "cmake_paths", "txt"]
    no_copy_source = True
    exports_sources = ["src/*", "include/*", "cmakescripts/*", "CMakeLists.txt"]
    build_policy = "missing"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

        self.options["boost"].shared = self.options.shared

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.dylib*", dst="bin", src="lib")
        self.copy("*.lib", dst="lib", src="lib")

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build(target="aviationcalc")

        # Explicit way:
        #self.run('cmake %s/hello %s'
        #         % (self.source_folder, cmake.command_line))
        #self.run("cmake --build . %s" % cmake.build_config)

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
        self.copy("*", dst="bin/eccodes", src="bin/eccodes")

    def package_info(self):
        self.cpp_info.libs = ["aviationcalc"]
