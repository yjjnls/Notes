<!-- istio.doczh.cn -->
# Service Mesh
Service Mesh 提供了一种透明的、与编程语言无关的方式，使网络配置、安全配置以及遥测等操作能够灵活而简便地实现自动化。从本质上说，它解耦了服务的开发与运维工作。如果你是一名开发者，那么在部署新服务，或是修改现有服务的时候，就无需担心这些操作会对你的分布式系统带来哪些运维层面的影响。

与之类似，运维人员可以放心地对服务之间的运维控制进行变更，而无需重新部署服务或是修改服务的源代码。处于服务与底层网络之间的这一层基础设施通常被称为 Service Mesh。

## 基本结构
![](https://mmbiz.qpic.cn/mmbiz_png/UicsouxJOkBdze6ibMeNuCLxhORheRXauuqlS4icDUN9lib8gE4t8azmvdfm7US2eAEtjOduqPJdrNjPiawJeWiaNaGg/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)
Istio 的架构并不复杂，其核心组件只有四个。  
首先是名为 Envoy 的网络代理，用来协调服务网格中所有服务的出入站流量，并提供服务发现、负载均衡、限流熔断等能力，还可以收集大量与流量相关的性能指标；  
其次是名为 Mixer 收集器，用来从 Envoy 代理收集流量特征和性能指标；  
然后是名为 Pilot 的控制器，用来将流量协调的策略和规则发送到 Envoy 代理；  
最后是名为 Istio-Auth 的身份认证组件，用来做服务间访问的安全控制。

Envoy成为数据面板，而istio则称为控制面板
