{
  description = "oglgl - OpenGL приложение на C с SDL3";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        pname = "oglgl";
      in
      {
        packages.default = pkgs.stdenv.mkDerivation {
          inherit pname;
          version = "1.0.0";
          src = ./.;

          nativeBuildInputs = with pkgs; [
            pkg-config
            gcc
            gnumake
            bear
            clang-tools
            lldb
            mono  # для shader_minifier.exe
          ];

          buildInputs = with pkgs; [
            sdl3
            mesa
            libGL
            libGLU
            cglm
          ];

          # Правильная buildPhase для использования Makefile
          buildPhase = ''
            runHook preBuild
            make all
            runHook postBuild
          '';

          installPhase = ''
            runHook preInstall
            mkdir -p $out/bin
            cp bin/${pname} $out/bin/
            chmod +x $out/bin/${pname}
            runHook postInstall
          '';

          meta = with pkgs.lib; {
            description = "OpenGL приложение с SDL3";
            license = licenses.mit;
            platforms = platforms.linux;
          };
        };

        apps.default = {
          type = "app";
          program = "${self.packages.${system}.default}/bin/oglgl";
        };

        devShells.default = pkgs.mkShell {
          nativeBuildInputs = with pkgs; [
            pkg-config
            gcc
            gnumake
            bear
            clang-tools
            lldb
            gdb
            valgrind
            strace
            mono  # для shader_minifier.exe
          ];

          buildInputs = with pkgs; [
            sdl3
            mesa
            libGL
            libGLU
            cglm
          ];

          shellHook = ''
            echo "Среда разработки oglgl готова!"
            echo "Доступные команды:"
            echo "  make all    - собрать проект"
            echo "  make run    - запустить проект"
            echo "  make clean  - очистить сборку"
            echo "  nix run     - запустить через nix"
          '';
        };
      });
}
