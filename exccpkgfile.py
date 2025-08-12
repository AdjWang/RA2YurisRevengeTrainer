from collections import defaultdict
from functools import partial
import logging
from multiprocessing import cpu_count
import os
from pathlib import Path
import shutil
from typing import override

from exccpkg import exccpkg, tools

class Config:
    def __init__(self) -> None:
        project_dir = Path(__file__).resolve().parents[0]
        self.project_dir = project_dir
        self.deps_dir = self.project_dir / "deps"
        self.download_dir = self.deps_dir / "download"
        self.cmake_build_type = "Debug"
        self.install_dir = self.deps_dir / "out" / self.cmake_build_type
        self.generator = "Ninja"

        CFLAGS = defaultdict(dict)
        CXXFLAGS = defaultdict(dict)
        LDFLAGS = defaultdict(dict)
        CFLAGS["Windows"]["Debug"] = "/MP /utf-8 /EHsc"
        CFLAGS["Windows"]["Release"] = "/MP /utf-8 /Gy /EHsc"
        CXXFLAGS["Windows"]["Debug"] = "/MP /utf-8 /EHsc"
        CXXFLAGS["Windows"]["Release"] = "/MP /utf-8 /Gy /EHsc"
        LDFLAGS["Windows"]["Debug"] = "/OPT:REF /INCREMENTAL:NO"
        LDFLAGS["Windows"]["Release"] = "/OPT:REF /INCREMENTAL:NO"

        if self.cmake_build_type == "Release":
            self.msvc_rt_lib = "MultiThreaded"
        else:
            self.msvc_rt_lib = "MultiThreadedDebug"

        # Use policy CMP0091 with CMAKE_MSVC_RUNTIME_LIBRARY
        # See: https://cmake.org/cmake/help/latest/policy/CMP0091.html
        self.cmake_common = f"""
            -DCMAKE_BUILD_TYPE={self.cmake_build_type}
            -DCMAKE_CXX_STANDARD=23
            -DCMAKE_POLICY_DEFAULT_CMP0091=NEW
            -DCMAKE_MSVC_RUNTIME_LIBRARY={self.msvc_rt_lib}
            -DCMAKE_INSTALL_PREFIX={self.install_dir}
            -DCMAKE_INSTALL_LIBDIR=lib
            -DCMAKE_C_FLAGS="{CFLAGS["Windows"][self.cmake_build_type]}"
            -DCMAKE_CXX_FLAGS="{CXXFLAGS["Windows"][self.cmake_build_type]}"
            -DCMAKE_LINK_FLAGS="{LDFLAGS["Windows"][self.cmake_build_type]}" """
        
        self.dryrun = False
        self.rebuild = True


class CMakeCommon:
    def __init__(self, cfg: Config):
        self.cfg = cfg

    def download(self, url: str, pkg_name: str, ext: str) -> Path:
        package_path = self.cfg.download_dir / f"{pkg_name}{ext}"
        src_path = self.cfg.deps_dir / pkg_name
        tools.download(url, package_path, self.cfg.dryrun)
        tools.unpack(package_path, self.cfg.deps_dir, self.cfg.dryrun)
        return src_path
    
    def build(self, src_dir: Path, cmake_options: str = "") -> Path:
        build_dir = src_dir / "cmake_build" / self.cfg.cmake_build_type
        tools.cmake_prepare_build_dir(build_dir, rebuild=self.cfg.rebuild, dryrun=self.cfg.dryrun)
        tools.run_cmd(f"""cmake {self.cfg.cmake_common} {cmake_options}
                                -G {self.cfg.generator} -S {src_dir}
                                -B {build_dir}""", self.cfg.dryrun)
        tools.run_cmd(f"""cmake --build {build_dir}
                                --config={self.cfg.cmake_build_type}
                                --parallel={cpu_count()}""", self.cfg.dryrun)
        return build_dir
    
    def install(self, build_dir: Path) -> None:
        tools.run_cmd(f"""cmake --install {build_dir}
                                --prefix={self.cfg.install_dir}""", self.cfg.dryrun)


class Context(exccpkg.Context):
    def __init__(self):
        self.cfg = Config()
        self.cmake = CMakeCommon(self.cfg)


class AbseilCpp(exccpkg.Package):
    name = "abseil-cpp"
    version = "20240722.0"

    @override
    def grab(self, ctx: Context) -> Path:
        url = "https://github.com/abseil/abseil-cpp/archive/refs/tags/20240722.0.tar.gz"
        return ctx.cmake.download(url, "abseil-cpp-20240722.0", ".tar.gz")

    @override
    def build(self, ctx: Context, src_dir: Path) -> Path:
        return ctx.cmake.build(src_dir, "-DABSL_MSVC_STATIC_RUNTIME=ON")

    @override
    def install(self, ctx: Context, build_dir: Path) -> None:
        return ctx.cmake.install(build_dir)


class Boost(exccpkg.Package):
    def __init__(self, pkg_name: str) -> None:
        self.name = pkg_name
        self.version = "1.86.0"

    @override
    def grab(self, ctx: Context) -> Path:
        download_dir = ctx.cfg.deps_dir / f"boost-{self.version}-{self.name}"
        if not download_dir.exists():
            tools.run_cmd(f"git clone https://github.com/boostorg/{self.name}.git {download_dir}", ctx.cfg.dryrun)
            cwd = os.getcwd()
            os.chdir(download_dir)
            tools.run_cmd(f"git checkout boost-{self.version}", ctx.cfg.dryrun)
            os.chdir(cwd)
        return download_dir

    @override
    def build(self, ctx: Context, src_dir: Path) -> Path:
        return src_dir
    
    @override
    def install(self, ctx: Context, build_dir: Path) -> None:
        proj_dir = build_dir
        tools.mkdirp(ctx.cfg.install_dir / "include/boost", ctx.cfg.dryrun)
        if not ctx.cfg.dryrun:
            shutil.copytree(proj_dir / "include/boost", ctx.cfg.install_dir / "include/boost",
                            dirs_exist_ok=True)


class Freetype(exccpkg.Package):
    name = "freetype"
    version = "2.13.2"

    @override
    def grab(self, ctx: Context) -> Path:
        download_dir = ctx.cfg.deps_dir / "freetype-2.13.2"
        if not download_dir.exists():
            tools.run_cmd(f"git clone https://github.com/freetype/freetype.git {download_dir}", ctx.cfg.dryrun)
            cwd = os.getcwd()
            os.chdir(download_dir)
            tools.run_cmd("git checkout VER-2-13-2", ctx.cfg.dryrun)
            os.chdir(cwd)
        return download_dir

    @override
    def build(self, ctx: Context, src_dir: Path) -> Path:
        return ctx.cmake.build(src_dir)

    @override
    def install(self, ctx: Context, build_dir: Path) -> None:
        return ctx.cmake.install(build_dir)


class Glfw(exccpkg.Package):
    name = "glfw"
    version = "3.4"

    @override
    def grab(self, ctx: Context) -> Path:
        url = "https://github.com/glfw/glfw/archive/refs/tags/3.4.tar.gz"
        return ctx.cmake.download(url, "glfw-3.4", ".tar.gz")

    @override
    def build(self, ctx: Context, src_dir: Path) -> Path:
        return ctx.cmake.build(src_dir, """-DGLFW_BUILD_EXAMPLES=OFF
                                           -DGLFW_BUILD_TESTS=OFF
                                           -DGLFW_BUILD_DOCS=OFF""")

    @override
    def install(self, ctx: Context, build_dir: Path) -> None:
        return ctx.cmake.install(build_dir)


class Gsl(exccpkg.Package):
    name = "GSL"
    version = "4.2.0"

    @override
    def grab(self, ctx: Context) -> Path:
        url = "https://github.com/microsoft/GSL/archive/refs/tags/v4.2.0.tar.gz"
        return ctx.cmake.download(url, "GSL-4.2.0", ".tar.gz")

    @override
    def build(self, ctx: Context, src_dir: Path) -> Path:
        return ctx.cmake.build(src_dir, "-DGSL_TEST=OFF")

    @override
    def install(self, ctx: Context, build_dir: Path) -> None:
        return ctx.cmake.install(build_dir)


class Imgui(exccpkg.Package):
    name = "imgui"
    version = "1.91.9b-docking"

    @override
    def grab(self, ctx: Context) -> Path:
        url = "https://github.com/ocornut/imgui/archive/refs/tags/v1.91.9b-docking.tar.gz"
        return ctx.cmake.download(url, "imgui-1.91.9b-docking", ".tar.gz")

    @override
    def build(self, ctx: Context, src_dir: Path) -> Path:
        # Compile manually in CMakeLists.txt
        return src_dir

    @override
    def install(self, ctx: Context, build_dir: Path) -> None:
        # Directly add sources in CMakeLists.txt
        ...


class NlohmannJson(exccpkg.Package):
    name = "nlohmann-json"
    version = "3.11.3"

    @override
    def grab(self, ctx: Context) -> Path:
        url = "https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp"
        download_path = ctx.cfg.download_dir / "nlohmann-json-3.11.3.hpp"
        tools.download(url, download_path, ctx.cfg.dryrun)
        output_dir = ctx.cfg.deps_dir / "nlohmann-json-3.11.3"
        tools.mkdirp(output_dir, ctx.cfg.dryrun)
        if not ctx.cfg.dryrun:
            shutil.copy(download_path, output_dir)
        return output_dir

    @override
    def build(self, ctx: Context, src_dir: Path) -> Path:
        build_dir = src_dir
        return build_dir
    
    @override
    def install(self, ctx: Context, build_dir: Path) -> None:
        install_dir = ctx.cfg.install_dir / "include/nlohmann"
        tools.mkdirp(install_dir, ctx.cfg.dryrun)
        if not ctx.cfg.dryrun:
            shutil.copy(build_dir / "nlohmann-json-3.11.3.hpp", install_dir / "json.hpp")


class PlutoSVG(exccpkg.Package):
    name = "plutosvg"
    version = "1.0.0"

    @override
    def grab(self, ctx: Context) -> Path:
        url_plutovg = "https://github.com/sammycage/plutovg/archive/refs/tags/v1.0.0.tar.gz"
        plutovg_path = ctx.cmake.download(url_plutovg, "plutovg-1.0.0", ".tar.gz")
        url = "https://github.com/sammycage/plutosvg/archive/refs/tags/v0.0.6.tar.gz"
        plutosvg_path = ctx.cmake.download(url, "plutosvg-0.0.6", ".tar.gz")
        if not ctx.cfg.dryrun:
            shutil.rmtree(plutosvg_path / "plutovg")
            shutil.move(plutovg_path, plutosvg_path / "plutovg")
        return plutosvg_path

    @override
    def build(self, ctx: Context, src_dir: Path) -> Path:
        return ctx.cmake.build(src_dir, """-DBUILD_SHARED_LIBS=OFF
                                           -DPLUTOSVG_BUILD_STATIC=ON
                                           -DPLUTOSVG_ENABLE_FREETYPE=ON
                                           -DPLUTOSVG_BUILD_EXAMPLES=OFF""")

    @override
    def install(self, ctx: Context, build_dir: Path) -> None:
        return ctx.cmake.install(build_dir)


class Syringe(exccpkg.Package):
    name = "syringe"
    version = "1.0.0"

    @override
    def grab(self, ctx: Context) -> Path:
        url = "https://github.com/Phobos-developers/Syringe/archive/refs/tags/1.0.0.tar.gz"
        return ctx.cmake.download(url, "syringe-1.0.0", ".tar.gz")

    @override
    def build(self, ctx: Context, src_dir: Path) -> Path:
        return src_dir

    @override
    def install(self, ctx: Context, build_dir: Path) -> None:
        proj_dir = build_dir
        tools.mkdirp(ctx.cfg.install_dir / "include/Syringe", ctx.cfg.dryrun)
        if not ctx.cfg.dryrun:
            # Unfortunately, YRpp has no namespace, import them may conflict with
            # existing names, becareful.
            shutil.copy2(proj_dir / "include/Syringe.h", ctx.cfg.install_dir / "include/Syringe")
            shutil.copy2(proj_dir / "include/declaration.hpp", ctx.cfg.install_dir / "include/Syringe")


class Toml(exccpkg.Package):
    name = "toml"
    version = "3.4.0"

    @override
    def grab(self, ctx: Context) -> Path:
        url = "https://github.com/marzer/tomlplusplus/archive/refs/tags/v3.4.0.tar.gz"
        return ctx.cmake.download(url, "tomlplusplus-3.4.0", ".tar.gz")

    @override
    def build(self, ctx: Context, src_dir: Path) -> Path:
        return ctx.cmake.build(src_dir, "-DBUILD_EXAMPLES=OFF")

    @override
    def install(self, ctx: Context, build_dir: Path) -> None:
        return ctx.cmake.install(build_dir)


class Websocketpp(exccpkg.Package):
    name = "websocketpp"
    version = "0.8.2"

    @override
    def grab(self, ctx: Context) -> Path:
        url = "https://github.com/zaphoyd/websocketpp/archive/refs/tags/0.8.2.tar.gz"
        return ctx.cmake.download(url, "websocketpp-0.8.2", ".tar.gz")

    @override
    def build(self, ctx: Context, src_dir: Path) -> Path:
        return ctx.cmake.build(src_dir)

    @override
    def install(self, ctx: Context, build_dir: Path) -> None:
        return ctx.cmake.install(build_dir)


class YRpp(exccpkg.Package):
    name = "YRpp"
    version = "602d3c6"

    @override
    def grab(self, ctx: Context) -> Path:
        download_dir = ctx.cfg.deps_dir / "YRpp"
        if not download_dir.exists():
            tools.run_cmd(f"git clone https://github.com/AdjWang/YRpp.git {download_dir}", ctx.cfg.dryrun)
            cwd = os.getcwd()
            os.chdir(download_dir)
            tools.run_cmd("git reset --hard 602d3c6", ctx.cfg.dryrun)
            os.chdir(cwd)
        return download_dir

    @override
    def build(self, ctx: Context, src_dir: Path) -> Path:
        # Compile manually in CMakeLists.txt
        return src_dir

    @override
    def install(self, ctx: Context, build_dir: Path) -> None:
        proj_dir = build_dir
        tools.mkdirp(ctx.cfg.install_dir / "include/YRpp", ctx.cfg.dryrun)
        # Unfortunately, YRpp has no namespace, import them may conflict with
        # existing names, becareful.
        def __copy_fn(src, dst):
            src_path = Path(src)
            # Skip these folders and files.
            if (".git" in src_path.parts or
                ".gitattributes" in src_path.parts or
                ".gitignore" in src_path.parts):
                return
            shutil.copy2(src, dst)
        if not ctx.cfg.dryrun:
            shutil.copytree(proj_dir, ctx.cfg.install_dir / "include/YRpp", dirs_exist_ok=True,
                            copy_function=__copy_fn)


def collect() -> exccpkg.PackageCollection:
    collection = exccpkg.PackageCollection([
        AbseilCpp(),
        Boost("align"),
        Boost("asio"),
        Boost("assert"),
        Boost("config"),
        Boost("core"),
        Boost("date_time"),
        Boost("mpl"),
        Boost("numeric_conversion"),
        Boost("predef"),
        Boost("smart_ptr"),
        Boost("system"),
        Boost("utility"),
        Boost("winapi"),
        Boost("preprocessor"),
        Boost("static_assert"),
        Boost("throw_exception"),
        Boost("type_traits"),
        Freetype(),
        Glfw(),
        Gsl(),
        Imgui(),
        NlohmannJson(),
        PlutoSVG(),
        Syringe(),
        Toml(),
        Websocketpp(),
        YRpp(),
    ])
    return collection


def resolve(ctx: Context, collection: exccpkg.PackageCollection) -> None:
    tools.mkdirp(ctx.cfg.download_dir)
    tools.mkdirp(ctx.cfg.deps_dir)
    tools.mkdirp(ctx.cfg.install_dir)
    # Override child project's configuration to ensure ABI compatibility.
    collection.resolve(ctx)


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO)
    ctx = Context()
    collection = collect()
    resolve(ctx, collection)
