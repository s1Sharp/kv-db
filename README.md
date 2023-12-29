# cpp_nn

> entry in vscode and install `ms-vscode-remote.vscode-remote-extensionpack` extention
> then press `ctrl + P`
> then paste `>Dev Containers: Open Folder in Container` and press `ENTER`
###### - it will create devcontainer for build and test

#### than use command to build with default c++ tool set
> make build

#### than use command to build with clang++ tool set
> make clang

#### to run Qt gui application use command
> make run

##### if you got an error, check that you have created `xhost` server on you host machine
> xhost +local:root

#### add option for make `NEED_DOC=<true/false>` for generate doc with doxygen

> ~/.config/QtProject/QtCreator.ini

> ~/.config/QtProject/qtcreator/qtversion.xml