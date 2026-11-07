{
  description = "ThornLib - portable C99 library template";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

  outputs =
    { self, nixpkgs }:
    let
      systems = [
        "x86_64-linux"
        "aarch64-linux"
        "x86_64-darwin"
        "aarch64-darwin"
      ];

      forAllSystems = f: nixpkgs.lib.genAttrs systems (system: f (import nixpkgs { inherit system; }));
    in
    {
      devShells = forAllSystems (pkgs: {
        default = pkgs.mkShell {
          packages =
            with pkgs;
            [
              cmake
              ninja
              gcc
              clang
              clang-tools
              pkg-config
              doxygen
            ]
            ++ pkgs.lib.optionals pkgs.stdenv.isLinux [
              gdb
              valgrind
            ];
        };
      });

      packages = forAllSystems (pkgs: {
        default = pkgs.stdenv.mkDerivation {
          pname = "thorn";
          version = "0.1.0";
          src = pkgs.lib.cleanSource ./.;
          nativeBuildInputs = [ pkgs.cmake ];
          meta = {
            description = "Portable C99 library template";
            license = pkgs.lib.licenses.gpl3Only;
          };
        };
      });
    };
}
