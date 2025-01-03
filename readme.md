# 新生导游系统

使用C++完成的新生导游系统，通过 ``json`` 与前端进行交流。实现景点与路径的增删改查，完成各种限制交通方式的最短路并记录路径。



# 编译指令

```shell
git clone https://github.com/coolarec/Freshman-tour-guide-system.git

cd newStudents

g++ -std=c++17 -Wall -I./include main.cpp schoolMap.cpp -o newStudents
```