# GenshinDPScalculator|原神配队DPS计算器
A GenshinDPS calculator based on Qt6CMake|基于Qt6CMake开发的原神DPS计算器

基于C++和Qt6+CMake写的DPS计算程序
可以加入预设角色和配队计算DPS，可保存本地。截止2026/5/1，目前已经扩展大量本地功能，仍然是开发版本。
因为随机度高的原因，无武器和圣遗物系统，取而代之的是，将武器和圣遗物的面板增益自己写进角色预设里，如果是团队增益就不要写进角色，写进配队中的团队增益。
目前仅限于Windows系统可用，需要安装 VC++ 运行库。release中包含VC_redist.x64.exe。

Copyright © 2026 Paperfly_ovo 
All rights reserved.

This software uses Qt under the Qt Educational License.
Not for commercial use.

See: https://www.qt.io/licensing/
See: https://whu.edu.cn/

Contact me through 2024302141145@whu.edu.cn

***UPDATE***

**26/4/30**
*GenshinDPSCalculator_Portble_Dev_ver26.4.30.1*

1.第一次发布，实现基本逻辑，通过手动输入角色面板属性，技能时间和倍率，队伍增益和队伍输入序列，计算DPS。

2.完善基本逻辑。

**26/4/30**
*GenshinDPSCalculator_Portble_Dev_ver26.4.30.2*

1.~移除了配队功能中的时间选项~，仅由**角色页面配置**动作时间，优化逻辑和~时间不一致的bug~。

2.队伍配置中增益团队的选项改为**下拉框**，便于选取角色。

**26/5/1**
*GenshinDPScalculator_Portble_Dev_ver26.5.1.1*

1.增加了角色等级，怪物等级，无视防御，减防buff用于引入**减防乘区**。

2.角色预设**UI优化**，变为可拉框缩短长度，天赋列表加长，并且加入了元素类型选择，修复了~元素增伤无法生效的bug~。

3.配队buff优化，可以将目标角色指定为**任意角色或全体**，**扩展了增益类型**。

4.从**代码逻辑上优化**了指针使用，改为值传递，避免极端数值的情况下直接报错闪退。
