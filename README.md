# arduino_car_project
三系2019级arduino智能小车项目

IDE选择的是vscode + arduino ，相关配置可以参考博文
https://blog.csdn.net/CCC_122/article/details/97611228?ops_request_misc=&request_id=&biz_id=102&utm_term=vscode%20%20arduino&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduweb~default-1-97611228.first_rank_v2_pc_rank_v29&spm=1018.2226.3001.4187

封装了Move.h用于小车基本的运动，前进后退等一系列的基本动作可以直接使用。（注：封装的move文件不能与.ino文件在同一个文件夹，不然无法编译通过，只能放置于其他的文件夹中，再对c_cpp_properties.json中的includepath添加路径，查询了GitHub中arduino插件讨论区也有存在类似问题）

写了menu对模式进行选择，包括循迹，避障，跟随功能