#!/usr/bin/env python3

import argparse
import os
import platform
import shutil

from os import PathLike
from subprocess import Popen, PIPE
from types import *
from typing import *

if platform.system() == "Windows":
    exe_ext = ".exe"
else:
    exe_ext = ""

assets_dir = ""
output_dir = ""

dxc = ""

verbose = False
rebuild = False
purge = False

shader_includedirs = ["game/gpu"]


def main():
    root = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))

    def parse_arguments():
        parser = argparse.ArgumentParser(description="Builds the assets for the game")
        parser.add_argument(
            "-a",
            "--assets-dir",
            default=os.path.join(root, "assets"),
            help="Path to the assets directory (default: assets directory two above the script directory)",
        )
        parser.add_argument(
            "-o",
            "--output-dir",
            default=os.path.join(root, "assets", "out"),
            help="Path to the output directory (default: assets_dir/out)",
        )
        parser.add_argument(
            "-v",
            "--verbose",
            action="store_true",
            default=False,
            help="Whether to print tool output (default: false)",
        )
        parser.add_argument(
            "-r",
            "--rebuild",
            action="store_true",
            default=False,
            help="Whether to rebuild assets (default: false)",
        )
        parser.add_argument(
            "-p",
            "--purge",
            action="store_true",
            default=False,
            help="Whether to purge the output directory (default: false)",
        )

        return parser.parse_args()

    def build_asset(function: FunctionType, src: List[PathLike], dest: PathLike):
        os.makedirs(os.path.dirname(dest), exist_ok=True)
        dest_exists = os.access(dest, os.F_OK)
        if dest_exists:
            src_stat = os.stat(src)
            dest_stat = os.stat(dest)
            newer = src_stat.st_mtime > dest_stat.st_mtime
        else:
            newer = False
        if rebuild or (newer and dest_exists) or not dest_exists:
            command = function(src, dest)
            print(f"{command} (newer: {newer}, dest_exists: {dest_exists})")
            os.chmod(command[0], 755)
            process = Popen(
                command,
                stdout=PIPE,
                stderr=PIPE,
            )
            stdout, stderr = process.communicate()
            stdout = bytes.decode(stdout, encoding="utf-8")
            stderr = bytes.decode(stderr, encoding="utf-8")
            if verbose:
                print(f"Output:\n{stdout}\n{stderr}")
            if process.returncode != 0:
                print(f"building {src} failed: exit code {process.returncode} 0x{process.returncode:X}")
                exit(1)
        else:
            print(f"Skipping {dest} (newer: {newer}, dest_exists: {dest_exists})")

    args = parse_arguments()
    assets_dir = args.assets_dir
    output_dir = args.output_dir
    verbose = args.verbose
    rebuild = args.rebuild
    purge = args.purge

    print(
        f"{'(Re)b' if rebuild else 'B'}uilding assets in {assets_dir}"
    )

    def find_tool(name: AnyStr) -> str:
        return shutil.which(name)

    dxc = find_tool("dxc")

    if purge:
        print(f"Purging {output_dir}")
        shutil.rmtree(output_dir)

    shader_flags = [f"-I{os.path.join(root, dir)}" for dir in shader_includedirs]

    for root, _, files in os.walk(os.path.join(assets_dir, "shaders")):
        for file in files:
            if file.endswith(".hlsl"):
                name = os.path.splitext(file)[0]
                for shader in [
                    ("VertexMain", "vs_6_0"),
                    ("FragmentMain", "ps_6_0"),
                ]:
                    entry = shader[0]
                    model = shader[1]
                    extension = model[0:2]
                    build_asset(
                        lambda src, dest: [
                            dxc,
                            "-E",
                            entry,
                            "-T",
                            model,
                            src,
                            "-Fo",
                            dest,
                        ] + shader_flags,
                        os.path.join(root, file),
                        os.path.join(
                            root.replace(assets_dir, output_dir),
                            "directx12",
                            f"{name}.{extension}.cso",
                        ),
                    )
                    build_asset(
                        lambda src, dest: [
                            dxc,
                            "-E",
                            entry,
                            "-T",
                            model,
                            "-spirv",
                            "-DSPIRV",
                            src,
                            "-Fo",
                            dest,
                        ] + shader_flags,
                        os.path.join(root, file),
                        os.path.join(
                            root.replace(assets_dir, output_dir),
                            "vulkan",
                            f"{name}.{extension}.spv",
                        ),
                    )

    if os.path.exists(os.path.join(assets_dir, "fonts")):
        try:
            shutil.copytree(
                os.path.join(assets_dir, "fonts"), os.path.join(output_dir, "fonts")
            )
        except FileExistsError:
            pass


if __name__ == "__main__":
    main()