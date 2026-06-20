{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    { nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs { inherit system; };
        libcpp = pkgs.stdenv.cc.cc.lib; # for serialport
      in
      {
        devShell =
          with pkgs;
          mkShell {
            buildInputs = [
              # nix
              nixd
              nil
              nixfmt
              # c/c++
              clang-tools # must be before clang to have the correct clangd in PATH
              gcc
              # arduino
              platformio
              platformio-core
              arduino
              arduino-core
              avrdude
              # java
              javaPackages.compiler.openjdk21
              # build tools
              gnumake
              # js
              nodejs
              prettier
              typescript-language-server
              # serialport
              libcpp
              # docs
              plantuml
              mermaid-cli
            ];
            env = {
              LD_LIBRARY_PATH = lib.makeLibraryPath [
                libcpp
              ];
            };
            shellHook = ''
              set -a
              source .env 2>/dev/null
              set +a
            '';
          };
      }
    );
}
