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
            ];
            shellHook = ''
              set -a
              source .env 2>/dev/null
              PATH=$(realpath ./bin):$PATH
              set +a
            '';
          };
      }
    );
}
