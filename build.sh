# Simple script to do an out-of-source cmake build
rm compile_commands.json
mkdir -p build
cd build

cmake ..
make

# Special for command completion
ln -s `pwd`/compile_commands.json ../compile_commands.json
