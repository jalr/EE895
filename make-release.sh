#!/usr/bin/env bash

version="$(sed -n -r 's/^version=(.*)/\1/p' library.properties)"
git tag "v$version"

files="$(git ls-files src examples README.md LICENSE library.properties keywords.txt)"

zip -r "Arduino_Library-$version.zip" $files
tar czf "Arduino_Library-$version.tar.gz" $files
