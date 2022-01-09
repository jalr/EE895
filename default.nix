{ pkgs ? import <nixpkgs> {} }:
with pkgs;
mkShell {
  buildInputs = [
    astyle
    python38
    python38Packages.setuptools
    python38Packages.virtualenv
    zip
  ];
  shellHook = ''
    virtualenv --no-setuptools venv
    source venv/bin/activate
    pip install arduinokeywords
  '';
}
