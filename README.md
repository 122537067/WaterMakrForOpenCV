# 基于opencv 批量图片加日期水印
将图片格式命名为 `yyyy-mm-dd` 或者 `yyyy-mm-dd(n)` 放置相对路径下 <br/>
则根据图片大小自动生成日期于右下角<br/>
***
> ## 运行例子
> * 运行准备（图片重命名）
>  >![操作前](https://github.com/122537067/WaterMakrForOpenCV/blob/master/image/before.png)
> * 运行结果
>  >![操作后](https://github.com/122537067/WaterMakrForOpenCV/blob/master/image/after.png)

***
**注意** *windows 与 macOS 读取文件的方式不一样*
```
//macOS下获取目录下的所有图片（.png .jpg 格式文件）
void getPhoto()
{
    string lastFour; //文件最后四位字符（则文件格式
    string folderName;  //读取到的文件名
    struct dirent *dirp;
    DIR* dir = opendir("./");
    while ((dirp = readdir(dir)) != nullptr)
    {
        if (dirp->d_type == DT_REG)
        {
            // 文件
            printf("%s\n", dirp->d_name);
            folderName = (string)dirp->d_name; //更新文件名
            lastFour = folderName.substr(folderName.size() - 4); //获取文件格式
            if(lastFour == ".jpg" || lastFour==".png")
            {
                photoList[photoCount] = folderName;
                photoCount++;
            }
        } else if (dirp->d_type == DT_DIR) {
            // 文件夹
        }
    }
    closedir(dir);
}
```

