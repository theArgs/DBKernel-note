## 5 Variables

前面的章节介绍利润如何定义基本的目标和生成构建输出。其本身已经很有用了，但CMake附带了一系列其他功能，带来了巨大的灵活性和便利性。本章介绍了CMake最基本的部分之一，即变量的使用。

### 5.1 Variable Basics 

像任何编程语言一样，变量是在CMake中完成一个任务的基础。定义变量的最基本方法是使用`set()`命令，普通变量可以在CMakeLists.txt文件中定义：

``` cmake
set(varName value... [PARENT_SCOPE])
```

变量的名称`varName`可以包含字母、数字、下划线，字母区分大小写。名称也可以包含字符`./-+`但是在实践中很少见。

在CMake中，变量具有特定的作用域，就像其他语言中的变量的作用域限制在特定的函数、文件上一样。变量不能在它的作用域外读写。与其他语言相比，CMake中的变量作用域更灵活一点，但目前，我们都将变量的作用域视为全局的。第7章和第8章介绍了本地作用域的情况，并展示了如何使用`PARENT_SCOPE`关键字提升变量的可见性。

CMake将所有变量视为字符串，在各种上下文中，变量可能将其解释为不同的类型。在设置变量的值时，如果值中没有空格，则CMake不要求这些值使用引号包围。如果给定多个值，则这些值会用分号分隔，生成的字符串是CMake表示列表的方式。

``` cmake
set(myVar a b c)   # myVar = "a;b;c"
set(myVar a;b;c)   # myVar = "a;b;c"
set(myVar "a b c") # myVar = "a b c"
set(myVar a b;c)   # myVar = "a;b;c"
set(myVar a "b c") # myVar = "a;b c"
```

变量的值使用`${myVar}`来获取，可以在任何需要字符串或变量的地方使用。CMake特别灵活，它可以递归使用此形式指定要设置的另一个变量的名称，此外，CMake在使用变量之前不需要定义变量，如果使用未定义的变量相当于使用空字符串，而没有错误或警告，就像Unix shell脚本一样。

``` cmake
set(foo ab)                # foo   = "ab"
set(bar ${foo}cd)          # bar   = "abcd"
set(baz ${foo} cd)         # baz   = "ab;cd"
set(myVar ba)              # myVar = "ba"
set(big "${${myVar}r}ef")  # big   = "${bar}ef" = "abcdef"
set(${foo} xyz)            # ad    = "xyz"
set(bar ${notSetVar})      # bar   = ""
```

字符串不限于单行，可以包含嵌入的换行符。还可以包含反斜杠转义的引号。

``` cmake
set(myVar "goes here")
set(multiLine "First line ${myVar}
Second line with a \"quoted\" word")
```

如果使用CMake 3.0或更高版本，可使用Lua启发的括号语法来替代引号，内容开头使用`[=[`，结尾使用`]=]`。方括号之间可以出现任意数量的`=`字符，也可以不写，但在开头和结尾必须数目相同。如果开头括号后紧随换行符，则忽略第一个换行符，但后续的换行符则不会，此外，不会执行括号内内容的进一步转换（即不进行变量替换或转义）。

``` cmake
set(multiLine [[
First line
Second line
]])

set(shellScript [=[
#!/bin/bash

[[ -n "${USER}" ]] echo "Have USER"]]
]=])

# Equivalent code without bracket syntax
set(shellScript
"#!/bin/bash

[[ -n \"\${USER}\" ]] echo \"Have USER\"
")
```

正如上面的例子所示，括号语法特别适合定义Unix shell脚本等内容。此类内容使用`${...}`语法实现自己的目的，经常包含引号，使用括号语法意味着这些内容不必转义，这与定义CMake内容的传统引号风格不同。在`[`和`]`之间可以使用任意数量的`=`字符也让嵌入的方括号不会被误解为标记。后续章节包含进一步的示例，这些示例突出了括号语法是更好的方案的情况。

可以通过`unset()`命令或者使用空值调用`set()`命令来取消变量。如下示例是等效的，且如果`myVar`不存在也不会出现错误或警告：

``` cmake
set(myVar)
unset(myVar)
```

除了项目定义的自己使用的变量外，CMake的许多命令的行为也可能收到调用命令时的特定变量的值的影响。这是CMake用于定制命令行为或修改默认值的常见方式，这样就不必为每个命令、目标定义重复这些模式。每个命令的参考文档通常会列出可能影响命令行为的任何变量。后续章节还重点介绍了一些有用的变量，以及它们影响构建方式的信息。

### 5.2 Environment Variables

CMake还允许使用普通变量的形式来检索和设置环境变量的值。环境变量的值是使用特殊形式`$ENV{varName}`来获得，并且可以在任何地方替代常规`${varName}`形式的地方使用。设置环境变量的值的方式和普通变量相同，仅需使用`ENV{varName}`格式：

``` cmake
set(ENV{PATH} "$ENV{PATH}:/opt/myDir")
```

请注意这样的环境变量仅影响当前运行的CMake实例，CMake运行完成后，对环境变量的更改将丢失。特别是对环境变量的更改在构建时不可见，因此，在CMakeLists.txt中设置环境变量很少有用。

### 5.3 Cache Variables

除了上面讨论的普通变量外，CMake还支持缓存变量。与普通变量不同，缓存变量的生存期仅限于处理CMakeLists.txt文件，缓存变量存储在构建目录中名为CMakeCache.txt的特殊文件中，它们在CMake运行时持续存在。

一旦设置，缓存变量将保持设置，直到显式地从缓存中删除它们。缓存变量的检索方式与普通变量完全相同，即使用`${myVar}`的形式，但是`set()`命令在用于设置缓存变量时不同：

``` cmake
set(varName value ... CACHE type "docstring" [FORCE])
```

当存在`CACHE`关键字时，`set()`命令将应用于名为`varName`的缓存变量，而不是普通变量。缓存变量比普通变量携带的信息更多，包括类型和文档字符串。在设置缓存变量时，必须提供这两个变量，尽管`docstring`可以为空。类型和文档字符串都不影响CMake处理变量的方式，它们仅由GUI工具使用，用于更合适的形式向用户呈现变量。CMake在处理过程中总是将变量视为字符串，类型只是为了改善GUI工具中的用户体验。`type`必须是以下之一：

**BOOL**

缓存变量是布尔`on/off`值，GUI工具使用复选框或者类似的方法来表示变量。变量的字符串值将是CMake表示布尔值的某种方式之一（`ON/OFF`、`TRUE/FALSE`、`1/0`等）。

**FILEPATH**

缓存变量表示某个磁盘上文件的路径，GUI工具向用户显式一个文件对话框，用于修改变量的值。

**PATH**

与`FILEPATH`类似，但GUI工具提供了一个对话框，用于选择目录而不是文件。

**STRING**

变量被视为任意字符串，默认情况下，GUI工具使用单行文本编辑部件来操作变量的值。项目可以使用缓存变量属性来为GUI工具提供一组与预定义的值，以便以组合框或类似的形式显式。

**INTERNAL**

该变量不打算向用户提供，内部缓存变量有时用于持久记录项目的内部信息，GUI工具不显示内部变量。

GUI工具通常将`docstring`用作缓存变量的工具提示，或者在选择变量时的简短的说明。`docstring`应很短，由纯文本组成。下面将进一步讨论`FORCE`关键字：

设置布尔缓存变量是一个常见的需要，CMake为此提供了一个单独的命令，开发人员可以使用`option()`代替冗长的`set()`命令：

``` cmake
option(optVar helpString [initialValue])
```

如果省略`initialValue`，则默认值是`OFF`。如果提供初始值，则必须符合`set()`命令接收的布尔值之一。如上命令等同于：

``` cmake
set(optVar initialValue CACHE BOOL helpString)
```

与`set()`相比，`option()`命令更清楚地表达了布尔缓存变量的行为，因此它是首选的命令。

普通变量和缓存变量的一个重要区别是，`set()`命令仅在存在`FORCE`关键字时才覆盖缓存变量，而不是像普通变量一样始终覆盖预先存在的值。`set()`命令在用于定义缓存变量时作用更像是`set-if-not-set`，`option()`命令（没有强制功能）也是这样。这样的主要原因是缓存变量通常是开发人员自定义的，不像普通变量必须在CMakeList.txt中进行硬编码，缓存变量可以通过交互式GUI工具或脚本来修改，不必编辑CMakeLists.txt文件。

一个难以理解的点是：普通变量和缓存变量是分离的，可以有相同名字的普通变量和缓存变量，它们存储不同的值。在这种情况下，CMake在使用`${myVar}`时将检索普通变量的值，而不是缓存变量，换句话说，普通变量的优先级较高。一个例外是，在设置缓存变量的值时，如果在调用`set()`之前该缓存变量不存在，或者使用了`FORCE`选项，则当前作用域的任何普通变量都会被删除。注意，这意味着在第一次和后续运行CMake时会获得不同的行为，因为第一次运行时，缓存变量不存在，但在后续运行中，缓存变量将存在。因此，在第一次运行时，普通变量将被隐藏，但在随后的运行中，它不会被隐藏：

``` cmake
set(myVar foo)                 # Local myVar
set(result ${myVar})           # result = foo
set(myVar bar CACHE STRING "") # Cache myVar

set(result ${myVar})           # First run :      result = bar
                               # Subsequent runs: result = foo

set(myVar fred)
set(result ${myVar})           # result = fred
```

简单地说，`${myVar}`的结果行为将检索分配给`myVar`的最后一个值，无论它是普通变量还是缓存变量。后续章节中进一步讨论了这种行为，解释了变量的作用域如何影响`${myVar}`的返回的值。

### 5.4 Manipulating Cache Variables

使用`set()`和`option()`命令，项目可以为开发人员建立一组有用的自定义变量，可以用于打开或关闭构建的不同部分、设置外部包的路径、修改编译器和链接器的标志等等。后续的章节将介绍缓存变量的这些用途，但首先需要了解操作这些变量的方法，开发人员可以通过两种主要方法完成操作，包括`cmake`命令和使用GUI工具。

#### 5.4.1 Setting Cache Values On the Command Line

CMake允许通过传递给CMake的命令行选项来直接操作缓存变量。主要是`-D`选项，用于定义缓存变量的值。

``` cmake
cmake -D myVar:type=someValue ...
```

`someValue`将替换`myVar`缓存变量的任何以前的值，该行为本质上就像使用带有`CACHE`和`FORCE`选项的`set()`命令一样。命令行选项只需要给出一次，因为它存储在缓存中，以便后续运行，因此不需要每次运行`cmake`时都提供。可以提供多个`-D`选项，以便在`cmake`命令行上一次设置多个变量。

以这种方式定义缓存变量时，它们不必在CMakeLists.txt文件中设置（即不需要相应的`set()`命令）。在命令行上定义的缓存变量具有空的`docstring。`类型也可以省略，在这种情况下，变量将被赋予一个特殊的类似于`INTERNAL`的类型，下面展示了通过命令行设置缓存变量的各种示例：

``` cmake
cmake -D foo:BOOL=ON ...
cmake -D "bar:STRING=This contains spaces" ...
cmake -D hideMe=mysteryValue ...
cmake -D helpers:FILEPATH=subdir/helpers.txt ...
cmake -D helpDir:PATH=/opt/helpThings ...
```

如果缓存变量要设置为包含空格的值，则需要将`-D`选项的整个内容用引号包围。

在`cmake`命令行上不使用类型声明变量时，有一种特殊情况，如果项目的CMakeLists.txt文件尝试设置相同的缓存变量并且指定了类型为`FILEPATH`或者`PATH`，那么如果该缓存变量的值是相对路径，CMake将其视为相对于调用`cmake`的目录，并自动转换为绝对路径。这并不健壮，因为CMake可以从任何目录调用，因此，建议开发人员始终包含类型。

可以使用`-U`选项从缓存中删除变量，必要时可以重复该选项来删除多个变量。注意`-U`选项支持`*`和`?`通配符，但需要注意不要删除超出预期的内容使焕春处于无法构建的状态。一般来说，建议仅删除没有通配符的特定条目除非保证使用的通配符是安全的。

通过命令行设置缓存变量是自动构建脚本和通过cmake命令驱动CMake的重要组成部分。

#### 5.4.2 CMake GUI Tools

暂略。

### 5.5 Debugging Variables And Diagnostics

随着项目变得更加复杂或在定位意外行为时，在CMake运行期间打印诊断信息和变量值会很有用，这通常使用`message()`命令来实现。

``` cmake
message([mode] msg1 [msg2]...)
```

如果指定了多个`msg`，则它们将被连接到一个没有任何分隔符的字符串中，这通常不是开发人员的目录，所以更厂家你的用法是使用单个用引号包围的`msg`，可以保留空格。可以在`msg`中引用变量，这些变量将在打印之间计算值：

``` cmake
set(myVar HiThere)
message("The value of myVar = ${myVar}")
```

这将输出：

``` shell
The value of myVar = HiThere
```

`message()`命令可以接受一个`mode`关键字，该关键字会影响消息的输出方式，某些情况下会出现错误并停止CMake的构建。`mode`可用的值包括：

**STATUS**

附带信息，消息前面通常有两个连字符。

**WARNING**

Cmake警告，通常在支持的地方以红色突出显式（cmake命令控制台或cmake-gui日志区域），将继续处理。

**AUTHOR_WARNING**

与`WARNING`类似，但仅在启用开发人员警告时才显示（需要`cmake`命令行上的`-Wdev`选项）。项目通常不使用这类消息。

**SEND_ERROR**

指示错误消息，如果支持则该消息将以红色突出显式，程序将继续处理，直到配置阶段完成，但不会执行生成。这就像一个允许长须进一步处理的错误，但最终仍然指示失败。

**FATAL_ERROR**

表示硬错误，将打印消息并立即停止程序处理。日志将记录致命`message()命令`的位置。

**DEPRECATION**

用于记录废弃消息的特殊类别。如果`CMAKE_ERROR_DEPRECED`变量的值为布尔真值，则消息被视为错误。如果`CMAKE_WARN_DEPRECED`定义为布尔真值，则该消息被视为警告。如果两个变量都未定义，则不会显示该消息。

如果没有提供`mode`关键字，则消息被视为重要消息，并在不进行任何修改，直接记录。应该注意的是，使用`STATUS`模式和不指定`mode`关键字是不同的。当使用`STATUS`关键字时，消息会和其他CMake消息一样按照正确顺序打印，并在前面添加两个连字符，而如果没有指定`mode`关键字，则不会在前面加上前导连字符，并且消息可能出现乱序。

CMake提供了另一个帮助调试变量的机制是`variable_watch()`命令。这适用于更加复杂的项目，在这些项目中，可能不清楚变量最终获得什么值，当监视变量时，所有读写变量的操作都会被记录。

``` cmake
variable_watch(myVar [command])
```

在绝大多数情况下，只需列出要监视的变量，不需要使用`command`选项。这时会记录对指定变量的所有访问。我们可以指定这个命令，该命令将在每次读写变量时都执行。该命令应该是CMake函数或宏的名称，并传递如下参数：变量名、访问类型、变量的值、当前列表文件和列表文件栈。命令`variable_watch()`基本不使用。

### 5.6 String Handling

随着项目复杂性的增长，也需要实现更多逻辑来管理变量。CMake提供了`string()`命令，该命令提供了广泛有用的字符串处理功能。此命令使得项目能够执行查找和替换操作、正则表达式匹配、大小写转换和其他常见任务。下面仅介绍了一些常用的功能，CMake文档有所有可用操作和行为的说明。

`string()`命令的第一个参数指定要执行的操作，后续参数取决于要请求的操作。这些参数通常需要至少一个输入字符串，由于CMake命令不能返回值，所以有一个结果输出变量。在下面的例子中，输出变量一般命名为`outVar`。

``` cmake
string(FIND inputString subString outVar [REVERSE])
```

`FIND`在`inputString`中查找`subString`，并将找到的子串的索引存储在`outVar`中（第一个字符是0）。如果指定了`REVERSE`，则找到最后一个出现的子串，否则，找第一个出现的子串。如果`subString`没有出现在`inputString`中，则`outVar`被赋值为`-1`。

``` cmake
set(longStr abcdefabcdef)
set(shortBit def)

string(FIND ${longStr} ${shortBit} fwdIndex)
```

替换一个子串使用如下形式：

``` cmake
string(REPLACE matchString replacyWith outVar input [input ...])
```

`REPLACE`操作将`input`串中的所有的匹配`matchString`的子串替换为`replaceWith`，并将结果存储于`outVar`中。当给定多个`input`字符串时，替换之前会将每个字符串直接连接在一起（没有分隔符），这有时会产生意外的结果，大多数情况下只提供一个输入字符串。

`REGEX`操作很好的支持正则表达式，有几个不同的变体可以使用，由第二个参数决定：

``` cmake
string(REGEX MATCH    regex outVar input [input...])
string(REGEX MATCHALL regex outVar input [input...])
string(REGEX REPLACE  regex replaceWith outVar input [input...])
```

要匹配的正则表达式`regex`可以使用基本的正则表达式语法，但不支持例如否定等一些常见功能（完整规范可以参考CMake帮助文档）。输入字符串在替换之前被连接到一起。`MATCH`操作仅查找第一个匹配项，并将其存储在`outVar`中。`MATCHALL`匹配所有的可匹配子串，并将它们作为列表存储在`outVar`中。`REPLACE`将所有`input`串中满足条件的替换为`replaceWith`并将结果存储在`outVar`中。匹配时可以使用常见的符号`\1`、`\2`等，但需要注意反斜杠本身需要在CMake中转义：

``` cmake
set(longStr abcdefabcdef)

string(REGEX MATCHALL "[ace]" matchVar ${longStr})        # a;c;e;a;c;e
string(REGEX REPLACE "([de])" "X\\1Y" replVar ${longStr}) # abcXdYXeYfabcXdYXeYf
```

也可以抽取子串：

``` cmake
string(SUBSTRING input index length outVar)
```

`index`是一个整数，定义要从`input`中提取的子串的开始。最多提取`length`长度的字符，如果长度为`-1`，则返回开始位置到字符串末尾的所有字符。注意在CMake 3.1或更低版本中，如果`length`指向字符串末尾，或报告错误。

字符串长度可以简单地获取，也可很容易地转换为大写或小写，删除字符串开头和结尾处的空格也很简单，这些操作的语法格式类似：

``` cmake
string(LENGTH  input outVar)
string(TOLOWER input outVar)
string(TOUPPER input outVar)
string(STRIP   input outVar)
```

CMake也提供诸如字符串比较、哈希、时间戳等操作，但在日常中并不常见，可以阅读`string()`命令的参考文档来了解详细信息。

### 5.7 Lists

列表在CMake中被大量使用，列表只是一个字符串，列表项之间用分号分隔，使用字符串格式操作单个列表项可能不方便。CMake提供了`list()`命令来简化此任务，与`string()`命令一样，`list()`命令的操作作为第一个参数，第二个参数是要操作的列表，而且必须是变量（即不允许传递字面值）。

最基本的列表操作是计算列表项数量和检索一个或多个条目：

``` cmake
list(LENGTH listVar outVar)
list(GET    listVar index [index...] outVar)
```

``` cmake
# Example
set(myList a b c)
list(LENGTH myList len)       # 3
list(GET myList 2 1 letters)  # c;b
```

追加和插入项目也是常见任务：

``` cmake
list(APPEND listVar item [item...])
list(INSERT listVar index item [item...])
```



### 5.8 Math

### 5.9 Recommended Practices