# Travis
![](http://www.138dish.cn/web/images/bg2017121901.png)

Travis CI 提供的是持续集成服务（Continuous Integration，简称 CI）。它绑定 Github 上面的项目，只要有新的代码，就会自动抓取。然后，提供一个运行环境，执行测试，完成构建，还能部署到服务器。

持续集成指的是只要代码有变更，就自动运行构建和测试，反馈运行结果。确保符合预期以后，再将新代码"集成"到主干。

持续集成的好处在于，每次代码的小幅变更，就能看到运行结果，从而不断累积小的变更，而不是在开发周期结束时，一下子合并一大块代码。

## 使用准备
Travis CI 只支持 Github，不支持其他代码托管服务。这意味着，你必须满足以下条件，才能使用 Travis CI。

*   拥有 GitHub 帐号
*   该帐号下面有一个项目
*   该项目里面有可运行的代码
*   该项目还包含构建或测试脚本

首先，访问官方网站 [travis-ci.org](https://www.travis-ci.org/)，点击右上角的个人头像，使用 Github 账户登入 Travis CI。

Travis 会列出 Github 上面你的所有仓库，以及你所属于的组织。此时，选择你需要 Travis 帮你构建的仓库，打开仓库旁边的开关。一旦激活了一个仓库，Travis 会监听这个仓库的所有变化。

![](http://www.138dish.cn/web/images/bg2017121902.png)

### .travis.yml
Travis 要求项目的根目录下面，必须有一个.travis.yml文件。这是配置文件，指定了 Travis 的行为。该文件必须保存在 Github 仓库里面，一旦代码仓库有新的 Commit，Travis 就会去找这个文件，执行里面的命令。

Travis 默认提供的运行环境，请参考[官方文档](https://docs.travis-ci.com/user/languages)。目前一共支持31种语言，以后还会不断增加。

## 运行流程
Travis 的运行流程很简单，任何项目都会经过两个阶段。

*   install 阶段：安装依赖
*   script 阶段：运行脚本

### install

install字段用来指定安装脚本。

```yml
install: ./install-dependencies.sh
```
如果有多个脚本，可以写成下面的形式。

```yml
install:
  - command1
  - command2
```
上面代码中，如果command1失败了，整个构建就会停下来，不再往下进行。

如果不需要安装，即跳过安装阶段，就直接设为true。

```yml
install: true
```

### script

script字段用来指定构建或测试脚本。

```yml
script: bundle exec thor build
```
如果有多个脚本，可以写成下面的形式。

```yml
script:
  - command1
  - command2
```
注意，script与install不一样，如果command1失败，command2会继续执行。但是，整个构建阶段的状态是失败。

如果command2只有在command1成功后才能执行，就要写成下面这样。

```yml
script: command1 && command2
```

使用travis构建项目的一些例子：
*   [c/c++](https://docs.travis-ci.com/user/languages/c/)
*   [nodejs](https://docs.travis-ci.com/user/languages/javascript-with-nodejs/)


