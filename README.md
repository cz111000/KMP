#何为KMP?
---
KMP算法是拿来处理字符串匹配的。例如：

A、B两人对暗号。
A：“天王盖地虎？” B：“额...宝塔镇河妖！”

A需要判断B回答的话里有没有“宝塔镇河妖”，来验证B是否是自己人

(有时A还需要知道B回答暗号所在位置)

此时就需要高效的KMP算法出马！

#KMP思想
---
困扰本人多年的看毛片算法经过一年终于掌握。特此记录。

###高效移动

看看下面这个栗子：

我们用 i 标记 A 字符串中进行匹配字符的位置，用 j 标记 B 字符串中进行匹配字符的位置

`i == 0 1 2 3 4 5 6 7 8 9 10 …`

`A = "a b a b a b a c b a b …"`

`B = "a b a b a c b"`

`j == 0 1 2 3 4 5 6`

显而易见，当 `i= j =5` 时A、B字符串中字符出现差异，接下来需将B字符串后移1位，且使 `i = 1; j = 0` 重新匹配？

不！我们有更高效的KMP算法，观察发现在字符 `'c'` 之前的字符串 `"ababa"` 存在相同的子串 `"aba"` 即 `B[0~2] = B[2~4]` 而 `B[3] != B[5]`

所以我们可以直接将B字符串后移2位继续匹配，提高效率减少无用的移动，即

`A = "a b a b a b a c b a b …"`

`----B = "a b a b a c b"`

那么问题就落在了**如何找到每个字符之前最大的匹配子串长度？**

###寻找相同子串

我们只要循环遍历B字符串，分别看前 0 个字符，前 1 个字符，前 2 个字符， 3 个 ... 最后到前 6 个。再将最长对称字符串的长度记录进数组next中即可

`B[0] = 'a'` 的前 0 个字符中无字符元素，为特殊情况，故

`next[0] = -1`

`B[1] = 'b'` 的前 1 个字符中仅有字符 `a` ，为特殊情况，故

`next[1] = 0`

`B[2] = 'a'` 的前 2 个字符中有{`a`} {`b`}，无相同子串，故

`next[2] = 0`

`B[3] = 'b'` 的前 3 个字符中有{`a`,`ab`} {`a`,`ba`}，存在相同子串{`a`}，故

`next[3] = 1`

`B[4] = 'a'` 的前 4 个字符中有{`a`,`ab`,`aba`} {`b`,`ab`,`bab`}，存在相同子串{`ab`}，故

`next[4] = 2`

`B[5] = 'c'` 的前 5 个字符中有{`a`,`ab`,`aba`,`abab`} {`a`,`ba`,`aba`,`baba`}，存在相同子串{`ab`}，故

`next[5] = 3`

`B[6] = 'b'` 的前 6 个字符中有{`a`,`ab`,`aba`,`abab`,`ababa`} {`c`,`ac`,`bac`,`abac`,`babac`}，无相同子串，故

`next[6] = 0`

即

`B === "a b a b a c b"`

`next= -1 0 0 1 2 3 0`

#KMP代码
---
按照下面的规则实现(k表示当前进行匹配的字符位置)：

a、若 `next[k - 1] == 0` 即之前对称程度为0的时候，只要将当前字符`B[k]`与字符串第1个字符 `B[0]` 进行比较。因为前面是0，说明前面不对称，如果多加了一个字符，要对称的话最多是 `B[k] == B[0]` 

b、按照a推理，若 `next[k - 1] == 1` 即之前对称程度为1，只要将当前字符`B[k]`与字符串第2个字符 `B[1]` 进行比较，因为前面是1，说明前面已匹配1长度，如果多加了一个字符，这个字符与第2个字符相等 `B[k] == B[1]` ，对称程度就为2 `next[1] = 2` 

c、同理，若 `next[k - 1] == 2` 即之前对称程度为2，只要将当前字符`B[k]`与字符串第3个字符 `B[2]` 进行比较...

当然不可能一直对称下去，若 `B[k] != B[n]` 即下一字符不相等，就无法继承之前的对称性，说明当前对称长度达最大值，但不能肯定对称性为 0 ！**需要回头再找对称性**

如下字符串，括号只是用来说明问题：

`B === "(a g c t a g c a)g c t a g c t"`

`next = -1 0 0 0 0 1 2 3 1 2 3 4 5 6 7`

可以看到第一个括号末尾 a g c a 的对称程度分别为：0，1，2，3。但紧随其后的字符g没有继承前面的对称程度，需重新计算。

![image](http://hi.csdn.net/attachment/201108/29/0_1314610574Rjbs.gif)


通过上述规则得到前缀next数组的求解代码：

```cpp
void makeNext(char* search, int* next)
{
    int length = -1;
	while (search[++length] != '\0');
	next[0] = -1;	//特殊值，当取得此值主串序号i++
	next[1] = 0;	//必定无匹配的子字符串
	int j = next[2 - 1];	//i从2开始，故j为next[i - 1]的值进行判断
	for (int i = 2;i < length;i++) {
		while ( j != 0 && search[j] != search[i - 1])	//如果j为0说明此字符之前无任何匹配字符串，如果出现匹配直接进入下方if，否则继续递推到子匹配字符串判断子匹配字符串后一个字符是否与此字符匹配
			j = next[j];
		if (search[j] == search[i - 1])	//如果j为0说明此字符之前无任何匹配字符串，那么直接判断此字符串是否和开头单个字符匹配
			j++;
		next[i] = j;
	}
}
```
整体代码详见代码文件

##参考文献
---
*[KMP算法详解](http://www.matrix67.com/blog/archives/115)*

*[字符串匹配的KMP算法](http://www.ruanyifeng.com/blog/2013/05/Knuth%E2%80%93Morris%E2%80%93Pratt_algorithm.html)*

*[KMP算法前缀next数组的通俗解释](http://blog.csdn.net/yearn520/article/details/6729426)*
