а{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = with pkgs; [
    # Build tools
    cmake
    pkg-config
    gcc
    gdb
    gnumake
    clang
    
    # Graphics libraries
    sdl3
    mesa
    libGL
    libGLU
    
    # Math library
    cglm
    
    # Development tools
    valgrind
    strace
    pkg-config
    mono
  ];

  nativeBuildInputs = with pkgs; [
    bear  # для compile_commands.json
    clang-tools  # для clangd
    lldb
  ];

  shellHook = ''
    export PATH=$PATH:/home/wntkys/projes/oglgl
  '';
}
