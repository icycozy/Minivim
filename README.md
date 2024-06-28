基础：
Normal mode: 支持 up，down，left，right，shortcuts: 其中dd会删除整行但不会删除行号，":" 转入 Command mode，"i" 转入 Insert mode
Insert mode: 支持 up，down，left，right，backspace(delete)，enter(换行)，tab，插入
Command mode: 支持 w，q，wq，q！指令，backspace

Extension：
1. Command History
2. Line Number and Jump: 屏幕空余四位作行号，jump line之后光标会停留在command window，退出command mode后即可见到光标在制定行行首
3. Line Wrapping: break模式屏幕最右侧空余，便于insert mode插入字符