# dinn_demo_with_HESea

## 编译方法
1. 在src/pke/example下有一个nn_multi_pke.cpp，vscode-CMaketool会把nn_multi_pke识别为一个target，可以单独编译；
2. 或者将nn_multi_pke.cpp可以单独拿出来，适当编写CMakeLists.txt，只要让他能够链接到头文件"palisade.h"就能编译通过；

## 注意事项
1. 函数名我基本上都加了HESea前缀，但也有可能漏掉的
2. 头文件"palisade.h"的名字要不要改一下？？
3. 注意nn_multi_pke.cpp与weights-and-biases文件夹的相对路径，保证程序能够正确寻找到路径。