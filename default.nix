{ pkgs ? import <nixpkgs> {} }:
with pkgs;
mkShell {
  buildInputs = [
    python38
    python38Packages.setuptools
    python38Packages.virtualenv
  ];
  shellHook = ''
    virtualenv --no-setuptools venv
    source venv/bin/activate
    pip install arduinokeywords
  '';
}
