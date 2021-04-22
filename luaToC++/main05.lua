local trie = require "Trie"

print("hhhhhhhh")
local trie = trie.create()


trie:insert("习近平")
trie:insert("平近习")
trie:insert("xjp")
trie:insert("习太子")
trie:insert("习明泽")
trie:insert("老习")
trie:insert("温家宝")
trie:insert("温加宝")
trie:insert("温x")
trie:insert("温jia宝")
trie:insert("温宝宝")



local x = trie:match("习近平按暴奸时暴奸发放")

print(x)

print(trie:print())
