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
        
        # Desktop entry для приложения
        desktopItem = pkgs.makeDesktopItem {
          name = pname;
          exec = pname;
          icon = pname;  # используем нашу собственную иконку
          desktopName = "OGL GL Demo";
          comment = "OpenGL демонстрация с SDL3";
          genericName = "3D Graphics Demo";
          categories = [ "Graphics" "Development" "Education" ];
          keywords = [ "opengl" "graphics" "demo" "3d" "sdl3" "cglm" ];
          terminal = false;
          startupNotify = true;
        };
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
            
            # Установка исполняемого файла
            mkdir -p $out/bin
            cp bin/${pname} $out/bin/
            chmod +x $out/bin/${pname}
            
            # Установка desktop entry
            mkdir -p $out/share/applications
            cp ${desktopItem}/share/applications/*.desktop $out/share/applications/
            
            # Установка иконки
            mkdir -p $out/share/icons/hicolor/scalable/apps
            cp ${pname}-icon.svg $out/share/icons/hicolor/scalable/apps/${pname}.svg
            
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
