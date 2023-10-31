
## git commit 规范 
`<type>(<scope>): <subject>`  
<commit类型>(影响范围): 具体描述  
e.g. : `[fix](api): fixed invalid table indexes.`

其中type为必填项，用于指定commit的类型，约定了feat、fix两个主要type，以及docs、style、build、refactor、revert五个特殊type，其余type暂不使用。

### 主要type
update:   更新涉及较多修改，无法归纳  
feat:     增加新功能  
fix:      修复bug  
add:      新增第三方库、资源、配置或文件等  
del:      删除第三方库、资源、配置或文件等   
### 特殊type 
style:    不影响代码含义的改动，例如去掉空行、缩进等    
build:    构造、编译相关的改动 
refactor: 代码、工程结构重构时使用   
revert:   执行git revert打印message   
docs:     改动了文档相关的内容 

### 其他type
perf:     提高性能的改动  
chore:    改变构建流程、或者增加依赖库、工具等，源码以外的其余修改，例如构建过程或辅助工具的变动  