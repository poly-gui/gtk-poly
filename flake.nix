{
  description = "C++ support library for NanoPack";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs?tag=24.05";
    nanopack.url = "git+file:./lib/nanopack?ref=main&submodules=1";
  };

  outputs = { self, nixpkgs, nanopack, ... }:
    let
      supportedSystems = [ "x86_64-linux" "x86_64-darwin" "aarch64-linux" "aarch64-darwin" ];

      forAllSystems = nixpkgs.lib.genAttrs supportedSystems;

      nixpkgsFor = forAllSystems (system: import nixpkgs { inherit system; });
    in
    {
      packages = forAllSystems (system:
        let
          pkgs = nixpkgsFor.${system};
        in
        rec {
          libgtkpoly = pkgs.stdenv.mkDerivation {
            pname = "libgtkpoly";
            version = "0.1.0";
            src = ./.;
            nativeBuildInputs = [
              pkgs.cmake
              pkgs.pkg-config
              pkgs.util-linux
            ];
            buildInputs = [
              nanopack.packages.${system}.libnanopack
              pkgs.pcre2
              pkgs.libselinux
              pkgs.libsepol
              pkgs.fribidi
              pkgs.libthai
              pkgs.libdatrie
              pkgs.expat
              pkgs.xorg.libXdmcp
              pkgs.lerc
              pkgs.gtkmm4
            ];
          };

          default = libgtkpoly;
        }
      );

      devShells = forAllSystems (system:
        let
          pkgs = nixpkgsFor.${system};
        in
        {
          default = pkgs.mkShell {
            packages = [
              nanopack.packages.${system}.default
              pkgs.cmake
              pkgs.clang-tools
              pkgs.gtkmm4
            ];
          };
        }
      );
    };
}
