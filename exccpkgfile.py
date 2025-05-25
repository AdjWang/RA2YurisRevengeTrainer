from collections import defaultdict
from functools import partial
import logging
from multiprocessing import cpu_count
import os
from pathlib import Path
import shutil
try:
    from typing import Self
except ImportError:
    from typing_extensions import Self

from exccpkg import exccpkg, tools

class Config(exccpkg.Config):
    def __init__(self, upstream_cfg: Self | None = None) -> None:
        super().__init__(upstream_cfg)
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
        LDFLAGS["Windows"]["Debug"] = ""
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
            -DCMAKE_INSTALL_LIBDIR=lib """
        self.cmake_common += f'-DCMAKE_C_FLAGS="{CFLAGS["Windows"][self.cmake_build_type]} "'
        self.cmake_common += f'-DCMAKE_CXX_FLAGS="{CXXFLAGS["Windows"][self.cmake_build_type]} "'
        self.cmake_common += f'-DCMAKE_LINK_FLAGS="{LDFLAGS["Windows"][self.cmake_build_type]} "'

        self.rebuild = False


class CMakeCommon:
    @staticmethod
    def download(cfg: Config, url: str, pkg_name: str, ext: str) -> Path:
        package_path = cfg.download_dir / f"{pkg_name}{ext}"
        tools.download(url, package_path)
        shutil.unpack_archive(package_path, cfg.deps_dir)
        return cfg.deps_dir / pkg_name
    
    @staticmethod
    def build(cfg: Config, src_dir: Path, cmake_options: str = "") -> Path:
        build_dir = src_dir / "cmake_build" / cfg.cmake_build_type
        tools.cmake_prepare_build_dir(build_dir, rebuild=cfg.rebuild)
        tools.run_cmd(f"""cmake {cfg.cmake_common} {cmake_options}
                                -G {cfg.generator} -S {src_dir}
                                -B {build_dir}""")
        tools.run_cmd(f"""cmake --build {build_dir}
                                --config={cfg.cmake_build_type}
                                --parallel={cpu_count()}""")
        return build_dir
    
    @staticmethod
    def install(cfg: Config, build_dir: Path) -> None:
        tools.run_cmd(f"""cmake --install {build_dir}
                                --prefix={cfg.install_dir}""")


class AbseilCpp(exccpkg.Package):
    def __init__(self) -> None:
        super().__init__(self.download, self.build, CMakeCommon.install)

    @staticmethod
    def download(cfg: Config) -> Path:
        url = "https://github.com/abseil/abseil-cpp/archive/refs/tags/20240722.0.tar.gz"
        return CMakeCommon.download(cfg, url, "abseil-cpp-20240722.0", ".tar.gz")

    @staticmethod
    def build(cfg: Config, src_dir: Path) -> Path:
        return CMakeCommon.build(cfg, src_dir, "-DABSL_MSVC_STATIC_RUNTIME=ON")


class Boost(exccpkg.Package):
    def __init__(self, pkg_name: str) -> None:
        super().__init__(partial(Boost.download, self), Boost.build, Boost.install)
        self.pkg_name = pkg_name
        self.boost_ver = "1.86.0"

    def download(self, cfg: Config) -> Path:
        download_dir = cfg.deps_dir / f"boost-{self.boost_ver}-{self.pkg_name}"
        if not download_dir.exists():
            tools.run_cmd(f"git clone https://github.com/boostorg/{self.pkg_name}.git {download_dir}")
            cwd = os.getcwd()
            os.chdir(download_dir)
            tools.run_cmd(f"git checkout boost-{self.boost_ver}")
            os.chdir(cwd)
        return download_dir

    @staticmethod
    def build(cfg: Config, src_dir: Path) -> Path:
        return src_dir
    
    @staticmethod
    def install(cfg: Config, build_dir: Path) -> None:
        proj_dir = build_dir
        tools.mkdirp(cfg.install_dir / "include/boost")
        shutil.copytree(proj_dir / "include/boost", cfg.install_dir / "include/boost",
                        dirs_exist_ok=True)


class Freetype(exccpkg.Package):
    def __init__(self) -> None:
        super().__init__(self.download, CMakeCommon.build, CMakeCommon.install)

    @staticmethod
    def download(cfg: Config) -> Path:
        download_dir = cfg.deps_dir / "freetype-2.13.2"
        if not download_dir.exists():
            tools.run_cmd(f"git clone https://github.com/freetype/freetype.git {download_dir}")
            cwd = os.getcwd()
            os.chdir(download_dir)
            tools.run_cmd("git checkout VER-2-13-2")
            os.chdir(cwd)
        return download_dir


class Glfw(exccpkg.Package):
    def __init__(self) -> None:
        super().__init__(self.download, self.build, CMakeCommon.install)

    @staticmethod
    def download(cfg: Config) -> Path:
        url = "https://github.com/glfw/glfw/archive/refs/tags/3.4.tar.gz"
        return CMakeCommon.download(cfg, url, "glfw-3.4", ".tar.gz")

    @staticmethod
    def build(cfg: Config, src_dir: Path) -> Path:
        return CMakeCommon.build(cfg, src_dir, """-DGLFW_BUILD_EXAMPLES=OFF
                                                  -DGLFW_BUILD_TESTS=OFF
                                                  -DGLFW_BUILD_DOCS=OFF""")


class Gsl(exccpkg.Package):
    def __init__(self) -> None:
        super().__init__(self.download, self.build, CMakeCommon.install)

    @staticmethod
    def download(cfg: Config) -> Path:
        url = "https://github.com/microsoft/GSL/archive/refs/tags/v4.2.0.tar.gz"
        return CMakeCommon.download(cfg, url, "GSL-4.2.0", ".tar.gz")

    @staticmethod
    def build(cfg: Config, src_dir: Path) -> Path:
        return CMakeCommon.build(cfg, src_dir, "-DGSL_TEST=OFF")


class Imgui(exccpkg.Package):
    def __init__(self) -> None:
        super().__init__(self.download, self.build, self.install)

    @staticmethod
    def download(cfg: Config) -> Path:
        url = "https://github.com/ocornut/imgui/archive/refs/tags/v1.91.9b-docking.tar.gz"
        return CMakeCommon.download(cfg, url, "imgui-1.91.9b-docking", ".tar.gz")

    @staticmethod
    def build(cfg: Config, src_dir: Path) -> Path:
        # Compile manually in CMakeLists.txt
        return src_dir

    @staticmethod
    def install(cfg: Config, src_dir: Path) -> None:
        # Directly add sources in CMakeLists.txt
        ...


class NlohmannJson(exccpkg.Package):
    def __init__(self) -> None:
        super().__init__(self.download, self.build, self.install)

    @staticmethod
    def download(cfg: Config) -> Path:
        url = "https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp"
        download_path = cfg.download_dir / "nlohmann-json-3.11.3.hpp"
        tools.download(url, download_path)
        output_dir = cfg.deps_dir / "nlohmann-json-3.11.3"
        tools.mkdirp(output_dir)
        shutil.copy(download_path, output_dir)
        return output_dir

    @staticmethod
    def build(cfg: Config, src_dir: Path) -> Path:
        build_dir = src_dir
        return build_dir
    
    @staticmethod
    def install(cfg: Config, build_dir: Path) -> None:
        install_dir = cfg.install_dir / "include/nlohmann"
        tools.mkdirp(install_dir)
        shutil.copy(build_dir / "nlohmann-json-3.11.3.hpp", install_dir / "json.hpp")


class Plutosvg(exccpkg.Package):
    def __init__(self) -> None:
        super().__init__(self.download, self.build, CMakeCommon.install)

    @staticmethod
    def download(cfg: Config) -> Path:
        url_plutovg = "https://github.com/sammycage/plutovg/archive/refs/tags/v1.0.0.tar.gz"
        plutovg_path = CMakeCommon.download(cfg, url_plutovg, "plutovg-1.0.0", ".tar.gz")
        url = "https://github.com/sammycage/plutosvg/archive/refs/tags/v0.0.6.tar.gz"
        plutosvg_path = CMakeCommon.download(cfg, url, "plutosvg-0.0.6", ".tar.gz")
        shutil.rmtree(plutosvg_path / "plutovg")
        shutil.move(plutovg_path, plutosvg_path / "plutovg")
        return plutosvg_path

    @staticmethod
    def build(cfg: Config, src_dir: Path) -> Path:
        return CMakeCommon.build(cfg, src_dir, """-DBUILD_SHARED_LIBS=OFF
                                                  -DPLUTOSVG_BUILD_STATIC=ON
                                                  -DPLUTOSVG_ENABLE_FREETYPE=ON
                                                  -DPLUTOSVG_BUILD_EXAMPLES=OFF""")


class Syringe(exccpkg.Package):
    def __init__(self) -> None:
        super().__init__(self.download, self.build, self.install)

    @staticmethod
    def download(cfg: Config) -> Path:
        url = "https://github.com/Phobos-developers/Syringe/archive/refs/tags/1.0.0.tar.gz"
        return CMakeCommon.download(cfg, url, "syringe-1.0.0", ".tar.gz")

    @staticmethod
    def build(cfg: Config, src_dir: Path) -> Path:
        return src_dir

    @staticmethod
    def install(cfg: Config, build_dir: Path) -> None:
        proj_dir = build_dir
        tools.mkdirp(cfg.install_dir / "include/Syringe")
        # Unfortunately, YRpp has no namespace, import them may conflict with
        # existing names, becareful.
        shutil.copy2(proj_dir / "include/Syringe.h", cfg.install_dir / "include/Syringe")
        shutil.copy2(proj_dir / "include/declaration.hpp", cfg.install_dir / "include/Syringe")


class Toml(exccpkg.Package):
    def __init__(self) -> None:
        super().__init__(self.download, self.build, CMakeCommon.install)

    @staticmethod
    def download(cfg: Config) -> Path:
        url = "https://github.com/marzer/tomlplusplus/archive/refs/tags/v3.4.0.tar.gz"
        return CMakeCommon.download(cfg, url, "tomlplusplus-3.4.0", ".tar.gz")

    @staticmethod
    def build(cfg: Config, src_dir: Path) -> Path:
        return CMakeCommon.build(cfg, src_dir, "-DBUILD_EXAMPLES=OFF")


class Websocketpp(exccpkg.Package):
    def __init__(self) -> None:
        super().__init__(self.download, CMakeCommon.build, CMakeCommon.install)

    @staticmethod
    def download(cfg: Config) -> Path:
        url = "https://github.com/zaphoyd/websocketpp/archive/refs/tags/0.8.2.tar.gz"
        return CMakeCommon.download(cfg, url, "websocketpp-0.8.2", ".tar.gz")


class YRpp(exccpkg.Package):
    def __init__(self) -> None:
        super().__init__(self.download, self.build, self.install)

    @staticmethod
    def download(cfg: Config) -> Path:
        download_dir = cfg.deps_dir / "YRpp"
        if not download_dir.exists():
            tools.run_cmd(f"git clone https://github.com/AdjWang/YRpp.git {download_dir}")
            cwd = os.getcwd()
            os.chdir(download_dir)
            tools.run_cmd("git reset --hard e790ee9e")
            os.chdir(cwd)
        return download_dir

    @staticmethod
    def build(cfg: Config, src_dir: Path) -> Path:
        return src_dir
    
    @staticmethod
    def install(cfg: Config, build_dir: Path) -> None:
        proj_dir = build_dir
        tools.mkdirp(cfg.install_dir / "include/YRpp")
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
        shutil.copytree(proj_dir, cfg.install_dir / "include/YRpp", dirs_exist_ok=True,
                        copy_function=__copy_fn)


def resolve(cfg: Config) -> None:
    tools.mkdirp(cfg.download_dir)
    tools.mkdirp(cfg.deps_dir)
    tools.mkdirp(cfg.install_dir)
    deps = [
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
        Plutosvg(),
        Syringe(),
        Toml(),
        Websocketpp(),
        YRpp(),
    ]
    for dep in deps:
        dep.resolve(cfg)


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO)
    cfg = Config()
    resolve(cfg)
