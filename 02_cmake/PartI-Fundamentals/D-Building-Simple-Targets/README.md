## 4 Building Simple Targets 

如上一章所示，在Cmake中可以简单的定义一个可执行文件。下面的例子给出了可执行文件的目标名称，并列出了需要编译的源文件列表。

``` cmake
add_executable(myApp main.cpp)
```

这个命令假定了开发人员希望构建基本的控制台可执行文件，但CMake还允许开发人员定义其他类型的可执行文件，例如Apple平台上的应用程序包和Windows GUI应用程序。本章讨论提供给`add_executable()`的用来指定这些详细信息的其他选项。

除了可执行文件，开发人员还经常需要构建和链接库。CMake支持几种不同类型的库，包括静态库、动态库、模块库和框架库。CMake还提供了非常强大的功能，用于管理目标之间的依赖关系和库的连接方式，都将在本章进行讨论。本章所涵盖的概念在本书的其余部分中被广泛使用，同时还给出了有关于库和目标的一些变量和属性的非常基本的用法，以支持这些CMake功能。

### 4.1 Executables

相对于上一章基本的`add_executable()`命令更加复杂的一种形式如下：

``` cmake
add_executable(targetName [WIN32] [MACOSX_BUNDLE]
               [EXCLUDE_FROM_ALL]
               source1 [source2 ...]
)
```

与此前相比唯一的区别是一些新的可选项。

**WIN32**

当在Windows平台上构建可执行文件时，这个选项指示CMake将可执行文件构建为Windows GUI应用程序。在实践中，这个选项意味着它将使用`WinMain()`作为程序的开始而不是`main()`，此外它将与`/SUBSYSTEM:WINDOWS`选项链接。在其他平台上，`WIN32`选项将被忽略。

**MACOSX_BUNDLE**

此选项指示CMake在Apple平台上构建应用程序捆绑包，不仅适用于macOS，也适用于iOS等其他苹果平台。此选项的确切效果因平台而异，例如，在macOS上，应用程序捆绑包具有非常特殊的目录结构，而在iOS上，目录结构是扁平的。CMake还将为捆绑吧生成一个基本的Info.plist文件，这些详细信息将在以后介绍。在非Apple平台上，`MACOSX_BUNDLEg`关键字将被忽略。

**EXCLUDE_FROM_ALL**

有时，项目定义了很多目标，但默认情况下，仅需构建其中的一些目标。如果在构建时没有具体的指定，则会构建默认的`ALL`目标（取决于使用的CMake生成器，名称可能略有不同，例如Xcode的`ALL_BUILD`）。如果使用`EXCLUDE_FROM_ALL`选项定义了可执行文件，则默认`ALL`目标中不会包含该可执行文件。只有当可执行文件是由构建命令显式请求的，或者作为某个默认`ALL`目标的依赖项时才会生成该可执行文件。从`ALL`中扣除目标的一种常见情况是该可执行文件只是一个开发人员工具，只是偶尔需要。

除了上述内容外，`add_executable()`命令还有其他形式，不定义要构建的新的可执行文件或目标，而是用于生成对现有可执行文件或目标的一种引用，将在以后的章节进行介绍。

### 4.2 Defining Labraries

创建简单的可执行文件时构建系统的基本需求。对于较大的项目，创建和使用库的能力对于保持项目的可管理性也至关重要。CMake支持构建各种不同类型的库，处理许多平台差异，但仍然支持每种库的本机特性。库目标是使用`add_library()`命令定义的，命令最基本的形式如下：

``` cmake
add_library(targetName [STATIC | SHARED | MODULE]
            [EXCLUDE_FROM_ALL]
            source1 [source2 ...]
)
```

这个形式和`add_executable()`命令定义简单可执行文件的形式类似。`targetName`用于在CMakeLists.txt文件中引用库，默认情况下，构建在文件系统上的库的名字基于此目标名字派生。`EXCLUDE_FROM_ALL`关键字的效果与`add_executable()`命令中的效果完全相同。即阻止库被包含在默认的`ALL`目标中。要生成的库的类型由其余的关键字`STATIC`、`SHARED`、`MODULE`中的一个指定。

**STATIC**

指定静态库或文档，在Windows上，默认库的名称是`targetName.lib`，而在类Unix平台上，它通常是`libtargetName.a`。

**SHARED**

指定共享库或动态链接库，在Windows上，默认库的名称是`targetName.so`，在Apple平台上是`libtargetName.dylib`，在其他类Unix平台上是`libtargetName.so`。在Apple平台上，共享库也可标记为框架（frameworks）。

**MODULE**

指定一个类似共享库的库，但不直接链接到库或可执行文件，而是在运行时动态加载。这些通常是插件或者用户决定是否加载的可选组件。在Windows中，不会为`DLL`创建导入库。

可以省略要构建的库类型的关键字。除非项目特别需要某种类型的库，否则一般情况下我们不指定它的类型，而是在构建项目时，将选择权留给开发人员。在这种情况下，库将是`STATIC`或者`SHARED`，由CMake变量`BUILD_SHARED_LIBS`的值决定。如果`BUILD_SHARED_LIBS`的值是`true`，则库目标将是共享库，否则是静态库。后续章节中详细介绍了变量的使用方式，目前设置此变量的一种方法是在cmake命令行中包含`-D`选项：

``` shell
cmake -DBUILD_SHARED_LIBS=YES /path/to/source
```

也可以在CMakeLists.txt文件中任何`add_library()`命令之前设置它的值，但若想更改时就要修改文件（没有那么灵活）。

``` cmake
set(BUILD_SHARED_LIBS YES)
```

就像对可执行文件一样，库目标也可以引用不是由项目创建的一些现有的二进制文件或目标。还支持另一种类型的伪库用于不创建静态库的情况下收集对象文件。这些都会在以后的章节中进行讨论。

### 4.3 Linking Targets

在考虑项目中的目标时，开发人员通常习惯于从“库A需要库B，所以A与B链接”的方式来思考，一个库需要另一个库的想法非常简单，这也是考虑库处理的传统方式。但实际上，库之间存在几种不同类型的依赖关系：

**PRIVATE**

私有依赖指明库A在其内部实现中使用了库B。任何其他链接到库A的东西都不需要知道关于B的信息，因为它是A的内部实现细节。

**PUBLIC**

公共依赖指明库A不仅在内部使用库B，而且在其接口中也使用B。这意味着A不能在没有B的情况下使用，因此任何使用A的东西也将直接依赖于B。一个例子是在库A中定义一个函数，该函数至少有一个参数是在库B中定义和实现的类型，因此代码不能在不提供来自于B的参数类型的情况下调用A的函数。

**INTERFACE**

接口依赖指定为了使用库A，还必须使用库B。与公共依赖不同之处在于，库A内部不需要B，它只在A的接口中使用B。这种依赖很有用的一个场景是使用目标表示仅标头库的依赖关系时。

CMake使用`target_link_libraries()`命令支持这些复杂的依赖关系，命令的一般形式为：

``` cmake
target_link_libraries(targetName
     <PRIVATE|PUBLIC|INTERFACE> item1 [item2 ...]
    [<PRIVATE|PUBLIC|INTERFACE> item3 [item4 ...]]
    ...
)
```

这允许项目精确定义一个库如何依赖其他库。CMake负责管理整个库的链中的依赖关系，例如如下的示例：

``` cmake
add_library(collector src1.cpp)
add_library(algo src2.cpp)
add_library(engine src3.cpp)
add_library(ui src4.cpp)
add_executable(myApp main.cpp)

target_link_libraries(collector
    PUBLIC ui
    PRIVATE algo engine
)
target_link_libraries(myApp PRIVATE collector)
```

在这个示例中，`ui`库以`PUBLIC`形式链接到`collector`库中，因此即使`myApp`仅直接链接到`collector`库，`myApp`也将与库`ui`链接。另一方面，`algo`和`engine`库以`PRIVATE`形式链接到`collector`中，所以`myApp`将不会直接链接这两个库。后续章节会讨论一些静态库的其他行为，这些行为可能导致进一步的链接以满足依赖关系，包括循环依赖等。

后续的章节展示了一些其他的`target_...()`命令进一步增强目标之间的依赖信息。这些命令允许编译器/链接器选项和头文件搜索路径在由`target_link_libraries()`命令链接时，从一个目标传递到另一个目标。这些功能使得CMakeLists.txt文件更加简单和健壮。

后续的章节还讨论了更复杂的源目录层次，在这种情况下，与`target_link_libraries()`命令一起使用的`targetName`必须在此命令调用位置相同目录下的`add_executable()`或`add_library()`命令定义。

### 4.4 Linking Non-targets

在上一节中，链接的所有项都是现有的CMake目标，但`target_link_libraries()`命令比这更加灵活，除了CMake目标外，以下的内容也可以作为`target_link_libraries()`命令中的项。

**库文件的完整路径**

CMake将库文件添加到链接器命令中。如果更改了库文件，CMake将感知到该更改并重新链接目标。主机，从CMake 3.3版本开始，链接器命令始终使用完整路径，但在3.3版本之前，某些情况下CMake可能让链接器去搜索库文件（例如将`/usr/lib/libfoo.so`替换为`-lfoo`）。

**纯库名称**

如果只给出了库的名称，而没有路径，则链接器命令将搜索该库（例如，`foo`将变为`-lfoo`或`foo.lib，具体取决于平台`），这种方式常见于系统库。

**链接标记**

作为特殊情况，以`-l`或者`-framework`以外的连字符开头的项目将被视为要添加到链接器命令中的标志。CMake文档警告这些项目应该仅用于私人项目，并不安全。

除上述内容外，由于历史原因，任何项目前可能带有关键字`debug`、`optimized`或`general`关键字。这些关键字的影响是根据构建类型是否为调试构建，进一步细化何时包含关键字后面的项目。如果项目前有`debug`关键字，那个只有构建类型是`debug`时才会添加它。如果项目前有`optimized`关键字，则只有不使用`debug`构建时，才会添加该项目。`general`关键字指定应该为所有的构件配置添加该项，这也是不使用关键字时的默认行为。对于新项目，不应该使用`debug`、`optimized`、`general`关键字，因为有更清晰健壮的方法可以使用现代CMake实现同样的目的。

### 4.5 Recommended Practices

目标的名称不需要与项目名称相关。教程和示例中常见使用变量作为项目名称，并将该变量作为可执行文件的名称：

``` cmake
set(projectName MyExample)
project(${projectName})
add_executable(${projectName} ...)
```

这只适用于最基本的项目，并且会养成坏的习惯。即使它们有相同的前缀，项目名称和可执行文件名也是分离的。不应通过变量而是直接设置项目名称，要根据目标的作用而不是它所属的项目来选择目标名称，我们要假设项目最终需要定义多个目标。这在处理更复杂的多项目目标时是很重要的。

当为库命名目标时，不要使用以`lib`开头或结尾的名称。在许多平台上（除了Windows外的几乎所有平台）上，在构建实际库名称时，将自动添加库前缀，以使其符合平台规范。如果目标已经以`lib`开头，则生成的库文件名将是`liblib`的形式，应该认为这是一个错误。

除非有强烈的理由，否则尽量避免为库具体指定`STATIC`或者`SHARED`关键字。这允许在整个项目范围内使用静态库或动态库的策略选择时具有更大的灵活性，可以使用`BUILD_SHARED_LIBS`关键字来更改库类型默认值，而不必修改每个`add_library()`命令。

使用`target_link_libraries()`命令时要使用指定`PRIVATE`、`PUBLIC`或`INTERFACE`关键字。随着项目的增长，这三个关键字对如何处理目标之间的依赖关系有更大的影响。从项目开始也让开发人员尽早思考目标之间的依赖关系，有助于更早地突出项目中的结构问题。