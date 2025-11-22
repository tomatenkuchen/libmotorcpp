from conan import ConanFile
from conan.tools.build import can_run
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps
from conan.tools.files import copy
from pathlib import Path


class libmotorTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"

    def requirements(self):
        self.tool_requires("cmake/4.1.0")
        self.tool_requires("ninja/1.13.1")
        self.tool_requires("cppcheck/2.18.3")
        self.test_requires("catch2/3.11.0")
        self.requires(self.tested_reference_str)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self, generator = "Ninja")

        # make cmake emit compile commands
        tc.cache_variables["CMAKE_EXPORT_COMPILE_COMMANDS"] = "ON"

        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()

        # copy compile commands file to a location that clangd can find it. since this is the test folder
        # we copy this compile commands file to the source folder's build directory since this is where clangd is looking
        build_path = Path(self.build_folder)
        source_path = Path(self.source_folder)
        copy(self, "compile_commands.json", build_path, source_path / "../build/")

        cmake.build()

    def layout(self):
        cmake_layout(self)

    def test(self):
        if can_run(self):
            cmd = Path(self.cpp.build.bindir)/"tests"
            self.run(cmd, env="conanrun")
