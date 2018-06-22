# Coverity
Coverity是一款在线的代码静态分析工具，可用于分析代码中的缺陷，适用于C++、JAVA、JS等各种语言。Coverity可以与Github相连，从而导入Github中的项目进行分析。在Coverity的检测过程中，需要我们先用Coverity提供的工具来运行我们的编译脚本，然后将编译结果上传进行分析。

![](https://github.com/yjjnls/Notes/blob/master/img/coverity1.png)

如上图所示，这里有手动和自动化两种形式，这里我们选用自动化的方式并结合travis来进行检测。

## [Travis CI Integration](https://scan.coverity.com/travis_ci)

![](https://github.com/yjjnls/Notes/blob/master/img/coverity2.png)

首先需要按照如上提示注册github、travis和coverity相关的账户，并在travis中激活github项目。然后在coverity中导入相应的github项目，导入后可以看到项目信息，在项目信息下面有一个`project token`，这个token我们后面会用到。

![](https://github.com/yjjnls/Notes/blob/master/img/coverity3.png)

之后只需要在.travis.yml中添加coverity的信息就可以在提交commit时由travis来出发coverity分析。

```yml
env:
  global:
    # COVERITY_SCAN_TOKEN
    # ** specific to your project **
    - secure: "xxxx"

addons:
  coverity_scan:

    # GitHub project metadata
    # ** specific to your project **
    project:
      name: my_github/my_project
      version: 1.0
      description: My Project

    # Where email notification of build analysis results will be sent
    notification_email: scan_notifications@example.com

    # Commands to prepare for build_command
    # ** likely specific to your build **
    build_command_prepend: ./configure

    # The command that will be added as an argument to "cov-build" to compile your project for analysis,
    # ** likely specific to your build **
    build_command: make

    # Pattern to match selecting branches that will run analysis. We recommend leaving this set to 'coverity_scan'.
    # Take care in resource usage, and consider the build frequency allowances per
    #   https://scan.coverity.com/faq#frequency
    branch_pattern: coverity_scan
```

这里coverity是以addon的形式添加到travis中的运行的，首先需要填写两个编译命令 `build_command_prepend` 和 `build_command`，将我们的编译脚本命令填写到对应指令中去就可以。  


`branch_pattern`代表触发converity的分支，这里可以写master，代表每次master提交都会触发coverity。但是由于coverity提供的检测是有[次数限制](https://scan.coverity.com/faq#frequency)的，一旦converity工具检测到此项目当前正在进行代码分析，就会停止后面的操作，也就是不会执行`build_command_prepend` 和 `build_command`中的操作。如果项目依赖于travis构建版本，那么可以考虑将`branch_pattern`填写为coverity_scan，只有在coverity_scan这个分支上面提交commit后才会触发coverity。


.travis.yml中的编译测试操作一般是写在`script`中的，如果想在master分析执行script中的操作，而在coverity_scan分支中执行coverity检测，那么可以这样写
```yml
script: if [ "${COVERITY_SCAN_BRANCH}" != 1 ]; then make ; fi
```
触发coverity是COVERITY_SCAN_BRANCH这个环境变量会被置为1。


我们一开始提到的`project token`需要用travis提供的工具加密，这样就不会再代码中暴露出来了。

```shell
gem install travis
cd my_project
travis encrypt COVERITY_SCAN_TOKEN=project_token_from_coverity_scan
```

最后是代码分析成功和失败的结果。

![](https://github.com/yjjnls/Notes/blob/master/img/coverity_success.png)

![](https://github.com/yjjnls/Notes/blob/master/img/coverity_fail.png)