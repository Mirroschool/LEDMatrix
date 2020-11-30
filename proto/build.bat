mkdir dart
mkdir dart\pixel
mkdir dart\clock

mkdir nanopb
mkdir nanopb\pixel
mkdir nanopb\clock

protoc --plugin=protoc-gen-nanopb=C:\Users\stas\Documents\nanopb\generator\protoc-gen-nanopb.bat --nanopb_out=nanopb\pixel pixel.proto
protoc --plugin=protoc-gen-nanopb=C:\Users\stas\Documents\nanopb\generator\protoc-gen-nanopb.bat --nanopb_out=nanopb\clock clock.proto

protoc --plugin=protoc-gen-dart=C:\Users\stas\Documents\flutter\.pub-cache\bin\protoc-gen-dart.bat --dart_out=dart\pixel pixel.proto
protoc --plugin=protoc-gen-dart=C:\Users\stas\Documents\flutter\.pub-cache\bin\protoc-gen-dart.bat --dart_out=dart\clock clock.proto
