find ./src -regex '.*\.\(h\|cpp\|hpp\|cc\|cxx\)' -exec clang-format -style=file -i --verbose {} \;
