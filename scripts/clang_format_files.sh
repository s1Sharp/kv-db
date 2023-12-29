find ./src -iname *.h -o -iname *.cpp -o -iname *.hpp | xargs clang-format -i
find ./test -iname *.h -o -iname *.cpp -o -iname *.hpp | xargs clang-format -i
