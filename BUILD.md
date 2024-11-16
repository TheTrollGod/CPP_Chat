# Instrucions on how to build the application

## Ensure build tools are installed

### Linux
```bash
sudo apt update
sudo apt install build-essential gcc g++ cmake -y
```
Verify build tools are installed
```bash
make --version
gcc --version
g++ --version
```

Ensure that cmake is at least version 3.29 for compatability with this project
```bash
cmake --version
```

Install 3.29
```bash
#remove previous cmake install
sudo apt remove cmake

wget https://github.com/Kitware/CMake/releases/download/v3.29.2/cmake-3.29.2-linux-x86_64.sh
chmod +x cmake-3.29.2-linux-x86_64.sh
sudo ./cmake-3.29.2-linux-x86_64.sh --prefix=/usr/local --exclude-subdir
cmake --version
# If bash cannot find cmake in your path
echo 'export PATH=/usr/local/bin:$PATH' >> ~/.bashrc
source ~/.bashrc
cmake --version
```


## Build the project

### Linux
```bash
mkdir build
cd build
cmake ..
make
```

# Clion specifcs

## Add the project to be compiled by CLion
- Select "Current File" buy the build options and select "Edit configuations"
- Select "Add new" and add a new "CMake Application"
- Ensure that the target and executable are selected as the current project
- Double check that "Build" is set under the 
