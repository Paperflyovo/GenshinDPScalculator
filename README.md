# GenshinDPScalculator
A GenshinDPS calculator based on Qt6CMake

这是我基于C++和Qt6+CMake自己写的DPS计算程序
可以加入预设角色和配队计算DPS，可保存本地。目前功能非常有限。仍然是开发版本。
无武器和圣遗物系统，武器和圣遗物的面板增益需要自己写进角色预设里，如果是团队增益就不要写进角色，写进团队增益。
目前仅限于Windows系统可用，需要安装 VC++ 运行库。VC_redist.x64.exe。

Copyright © 2026 Paperfly_ovo 
All rights reserved.

This software uses Qt under the Qt Educational License.
Not for commercial use.
See: https://www.qt.io/licensing/
See: whu.edu.cn
Contact me through 2024302141145@whu.edu.cn

***UPDATE***

**26/4/30**
*GenshinDPSCalculator_Portble_Dev_ver26.4.30.1*
第一次发布，实现基本逻辑，通过手动输入角色面板属性，技能时间和倍率，队伍增益和队伍输入序列，计算DPS。完善基本逻辑。

**26/4/30**
*GenshinDPSCalculator_Portble_Dev_ver26.4.30.2*
1.~移除了配队功能中的时间选项~，仅由**角色页面配置**动作时间，优化逻辑和~时间不一致的bug~
2.队伍配置中增益团队的选项改为**下拉框**，便于选取角色。
